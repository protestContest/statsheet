OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(entrypoint)

MEMORY
{
  ROM   : ORIGIN = 0x08000000, LENGTH = 32M   /* Contents of the cart. Read only, slow access. */
  IWRAM : ORIGIN = 0x03000000, LENGTH = 32K   /* Fast RAM. Used for the stack and globals. */
  EWRAM : ORIGIN = 0x02000000, LENGTH = 256K  /* Slow RAM. Used for large global buffers and the heap. */
}

/*
The stack for each processor mode sit at the top of IWRAM. Some scratch memory is reserve for BIOS
functions, which also use the supervisor stack. The IRQ stack and userspace stack follow.

0x03008000  ┌─────────────────────┐  Top of IWRAM
            │  BIOS (0x20 bytes)  │  Reserved by GBA BIOS
0x03007FE0  ├─────────────────────┤  __STACK_END__
            │  SVC stack (0x40)   │  Supervisor mode (BIOS calls)
0x03007FA0  ├─────────────────────┤  __STACK_IRQ_END__
            │  IRQ stack (0xA0)   │  Interrupt handlers
0x03007F00  ├─────────────────────┤  __STACK_USR_END__
            │                     │
            │  User stack         │
            │  (grows down)       │
            │                     │
            ├─────────────────────┤  __STACK_START__ (minimum)
            │  .iwram, .data,     │
            │  .bss (grows up)    │
0x03000000  └─────────────────────┘  Bottom of IWRAM
*/

/* Calculate stack addresses. Stacks are set up on startup. */
__BIOS_SCRATCH_SIZE__   = 0x20;
__STACK_SVC_SIZE__      = 0x40;
__STACK_IRQ_SIZE__      = 0xA0;
__STACK_USR_SIZE_MIN__  = 0x200; /* Minimum size, the real size may be bigger */

__STACK_END__       = ORIGIN(IWRAM) + LENGTH(IWRAM) - __BIOS_SCRATCH_SIZE__;
__STACK_SVC_END__   = __STACK_END__;
__STACK_SVC_START__ = __STACK_SVC_END__ - __STACK_SVC_SIZE__;
__STACK_IRQ_END__   = __STACK_SVC_START__;
__STACK_IRQ_START__ = __STACK_IRQ_END__ - __STACK_IRQ_SIZE__;
__STACK_USR_END__   = __STACK_IRQ_START__;
__STACK_USR_START__ = __STACK_USR_END__ - __STACK_USR_SIZE_MIN__;
__STACK_START__     = __STACK_USR_START__;

SECTIONS
{
  /* ROM header and startup code */
  .gba_crt0 : ALIGN(4)
  {
      KEEP (*(.gba_crt0))
  } > ROM

  /* Program instructions */
  .text : ALIGN(4)
  {
    *(.text)
    *(.text*)
    *(.glue_7)  /* ARM/Thumb interworking */
    *(.glue_7t) /* ARM/Thumb interworking */
  } > ROM

  /* Read-only data (strings, constants) */
  .rodata : ALIGN(4)
  {
    *(.rodata)
    *(.rodata*)
  } > ROM

  /* Packed resources */
  .res (READONLY) : ALIGN(4)
  {
    KEEP (*(.res))
  } > ROM

  /* Uninitialized globals (not in ROM) */
  .bss (NOLOAD) : ALIGN(4)
  {
      *(.bss)
      *(.bss*)
      *(COMMON)
      . = ALIGN(4);
  } > IWRAM

  /* Calculate .bss addresses. */
  __IWRAM_BSS_START__ = ADDR(.bss);
  __IWRAM_BSS_SIZE__ = SIZEOF(.bss);
  __IWRAM_BSS_END__ = __IWRAM_BSS_START__ + __IWRAM_BSS_SIZE__;

  /* Initialized globals */
  .data : ALIGN(4)
  {
    __DATA_START__ = .;
    *(.data)
    *(.data*)
    . = ALIGN(32);
    __DATA_END__ = .;
  } > IWRAM AT> ROM

  /* Calculate data addresses. Copied to RAM on startup. */
  __DATA_SIZE__ = __DATA_END__ - __DATA_START__;
  __DATA_LMA__ = LOADADDR(.data); /* load memory address of .data in ROM */

  /* Explicit IWRAM data */
  .iwram : ALIGN(4)
  {
    __IWRAM_START__ = .;
    *(.iwram)
    *(.iwram*)
    *iwram.*(.text*)
    *iwram.*(.data*)
    . = ALIGN(32);
    __IWRAM_END__ = .;
  } > IWRAM AT> ROM

  /* Calculate .iwram addresses. Copied to RAM on startup. */
  __IWRAM_SIZE__ = __IWRAM_END__ - __IWRAM_START__;
  __IWRAM_LMA__ = LOADADDR(.iwram); /* load memory address of .iwram in ROM */

  /* Check that we left enough space for the stack */
  ASSERT(__IWRAM_END__ <= __STACK_START__, "Not enough free IWRAM for stack")

  /* Calculate actual stack size: the rest of IWRAM */
  __STACK_USR_SIZE__ = __STACK_USR_END__ - __IWRAM_END__;

  /* Uninitialized globals for EWRAM (not in ROM) */
  .sbss (NOLOAD) : ALIGN(4)
  {
    *(.sbss)
    *(.sbss*)
    . = ALIGN(4);
  } > EWRAM

  /* Explicit EWRAM data */
  .ewram : ALIGN(4)
  {
    __EWRAM_START__ = .;
    *(.ewram)
    *(.ewram*)
    *ewram.*(.text*)
    *ewram.*(.data*)
    . = ALIGN(32);
    __EWRAM_END__ = .;
  } > EWRAM AT > ROM

  /* Calculate .ewram addresses. Copied to RAM on startup. */
  __EWRAM_SIZE__ = __EWRAM_END__ - __EWRAM_START__;
  __EWRAM_LMA__ = LOADADDR(.ewram); /* load memory address of .ewram in ROM */

  /* The heap uses the rest of EWRAM */
  __HEAP_START__  = __EWRAM_END__;
  __HEAP_END__    = ORIGIN(EWRAM) + LENGTH(EWRAM);
  __HEAP_SIZE__   = __HEAP_END__ - __HEAP_START__;
}
