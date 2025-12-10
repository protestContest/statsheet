little_endian

set numStats [uint32 "Num Stats"]
for {set s 0} {$s < $numStats} {incr s} {
    section "Stat $s" {
        uint32 -hex "ID"
        set codeLen [uint32 "Code Length"]
        # bytes $codeLen
        section "Code" {
            for {set i 0} {$i < $codeLen} {incr i} {
                set offset [pos]
                set byte [uint8]
                if {$byte & 0x80} {
                    set byte [expr $byte & 0x7F]
                    entry "Short" $byte 1 $offset
                } elseif {$byte == 0x20} {
                    entry "Noop" "" 1 $offset
                } elseif {$byte == 0x3A} {
                    entry "Default" "" 1 $offset
                } elseif {$byte == 0x23} {
                    entry "Const" [uint32] 5 $offset
                    incr i 4
                } elseif {$byte == 0x24} {
                    entry "Var" [hex 4] 5 $offset
                    incr i 4
                } elseif {$byte == 0x2B} {
                    entry "Add" "" 1 $offset
                } elseif {$byte == 0x2D} {
                    entry "Sub" "" 1 $offset
                } elseif {$byte == 0x2A} {
                    entry "Mul" "" 1 $offset
                } elseif {$byte == 0x2F} {
                    entry "Div" "" 1 $offset
                } elseif {$byte == 0x25} {
                    entry "Rem" "" 1 $offset
                }
            }
        }

        set numDeps [uint32 "Num Deps"]
        for {set i 0} {$i < $numDeps} {incr i} {
            uint32 -hex "Dep ID"
        }
    }
}
