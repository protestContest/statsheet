# Statsheet

![Screenshot 1](https://github.com/protestContest/statsheet/blob/main/support/statsheet-0.png?raw=true)
![Screenshot 2](https://github.com/protestContest/statsheet/blob/main/support/statsheet-1.png?raw=true)

A D&D 5e character sheet app for the Game Boy Advance.

The "game" consists of a collection of screens:

- [x] An overview of basic character stats (HP, spell slots, ability scores, etc)
- [x] A page listing character skills
- [x] A reference of known spells
- [ ] A combat screen for making attacks
- [ ] A prepared spells screen for casting spells
- [ ] A generic dice-rolling screen

Each character stat is editable. Stats can be recalculated automatically from other stats. Attacks
and spells can be automatically performed according to the correct stats.

## Building

Requires the ARM EABI toolchain (`arm-none-eabi-gcc`).

```sh
make        # Build ROM to dist/statsheet.gba
make clean  # Clean build artifacts
make run    # Build and open ROM (macOS)
```

## Project Structure

```
src/          Game source
  kit/        Core framework (canvas, input, sprites, text)
  views/      UI components (buttons, labels, tabs, stat views)
  pages/      App screens (overview, skills, spellbook)
  main.c      Entry point
  stat.c      Stat VM interpreter
  spell.c     Spell database
inc/          Headers
res/          Resources (sprites, fonts, stat definitions)
tools/        Build tools
  rmake/      Resource packer
  romfix/     Writes a correct ROM header
```

## How the ROM is Built

The link script, `src/sys/gba_cart.ld`, describes the GBA memory layout. It defines the addresses
for ROM and the two types of RAM, IWRAM (small but fast) and EWRAM (slow but bigger). It describes
where to put different sections of the compiled program in ROM and where the compiler should link
addresses. It also defines some important addresses needed for system startup (basically, the
boundaries of sections that need to be copied from ROM to RAM).

The link script includes one non-standard section, `.res`. This is a custom section created by the
resource packing tool `rmake`. It's just packed read-only data at a known address.

The startup assembly file `src/sys/gba_crt0.s` is stored at the beginning of ROM, and is the
entrypoint for the GBA. It includes space for the ROM header (which must be fixed up with the
`romfix` tool) and setup code for the GBA. At the end of setup, it calls `main()`. That file also
contains hooks for calling the GBA BIOS functions from C.

The bulk of the code is built in a typical manner with the `Makefile`. It builds each object file
and links them into an ELF. The ELF is then stripped to bare binary, and the `romfix` tool is run
to write a valid header and pad the ROM to a standard size.

## Resources

One exception to the build process is the resource packing system. The `rmake` tool takes a resource
manifest file (`res/Manifest`), processes each resource file, and packs them into an output file.
This output is converted to an object file with the `.res` section, to be loaded into the ROM. The
resource code `src/res.h` handles accessing the packed resource data.

Each file in the resource manifest can be transformed with a given subtool, or simply copied. The
format of the packed resource file is described in `inc/kit/res.h`.

## Stat DSL

The `res/Stats` resource is interesting. (Some would say "overengineered"; I would say
"interesting".) It allows defining stats by name and calculation with a simple stack-based
expression language. The `PackStats` resource tool parses the file, including parsing the
expressions into bytecode, and packs them into a specific resource format.

```
Level = 6
ModSTR = STR 2 / 5 -
SpellSaveDC = 8 ModINT + PB +
PB = Level 1 - 4 / 2 +
L1Slots = 2 Level 1 > + Level 2 > +
```

At runtime, each stat has a current value, and a calculation (which is the bytecode of the
definition in the resource file). Each stat also has a list of stats that depend on it. When a stat
is updated, all of its dependents are recalculated recursively.

The stat definition file contains a list of definitions, one per line, with this form:

```
StatName = <expression>
```

Stat names can only include alpha-numeric characters or underscores. Definitions may not contain circular references. Expressions can include these elements:

- A constant decimal number, which is pushed onto the stack
- A reference to another stat, whose value is pushed onto the stack
- A quoted string, whose hash value is pushed onto the stack
- An opcode:
  - `+`: Adds the top two values
  - `-`: Subtracts the top two values
  - `*`: Multiplies the top two values
  - `/`: Divides the top two values (integer division)
  - `%`: Remainder of dividing the top two values
  - `&`: Bitwise "and" the top two values
  - `|`: Bitwise "or" the top two values
  - `~`: Bitwise "not" the top value
  - `?`: Conditional: `<if-true> <if-false> <condition> ?`. Leaves `<if-false>` if `<condition>` is
         `0`, otherwise leaves `<if-true>`.
  - `=`: Pushes `1` if the top two values are equal, `0` otherwise
  - `<`: Pushes `1` if the second-top value is less than the top value, `0` otherwise
  - `>`: Pushes `1` if the second-top value is greater than the top value, `0` otherwise
  - `[`: Begins a quoted expression. The expression is pushed on the stack, and execution skips to
         after the matching `]`.
  - `]`: Ends a quoted expression. Returns to the calling code when encountered.
  - `.`: Executes a quoted expression on the stack. Control resumes when the expression completes.
  - `:`: Duplicates the top value on the stack
  - `_`: Drops the top value on the stack
  - `\`: Swaps the top two values on the stack
  - `;`: Rotates the top three values on the stack. The third element is brought to the top.

The expression interpreter also supports ops to halt, to fetch an arbitrary value, and to store an
arbitrary value, but these are not allowed in this stat definition file.
