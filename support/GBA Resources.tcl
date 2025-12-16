# HexFiend template for a packed resources file

little_endian

set count [uint32 "Num Resources"]
uint32 "Total Size"

set offsets [list]

section "Map" {
    for {set i 0} {$i < $count} {incr i} {
        section $i {
            hex 4 "ID"
            lappend offsets [uint32 "Offset"]
        }
    }
}

proc header {} {
    set flags [uint8]
    move -1
    set compBits [expr $flags >> 4]
    if {$compBits == 0} {
        entry "Compression" "None" 1
    } elseif {$compBits == 1} {
        entry "Compression" "LZ77" 1
    } elseif {$compBits == 2} {
        entry "Compression" "Huffman" 1
        set huffBits [expr $flags & 0x0C]
        entry "Huffman Size" $huffBits 1
    } elseif {$compBits == 3} {
        entry "Compression" "Run-length" 1
    } elseif {$compBits == 8} {
        entry "Compression" "SubFilter" 1
        entry "Filter Size" [expr $flags & 0x3] 1
    } else {
        entry "Compression" $compBits 1
    }

    move 1

    uint24 "Uncompressed Length"
}

section "Resource Data" {
    set i 0
    set last [pos]
    foreach offset $offsets {
        if {$offset > $last} {
            set size [expr $offset - $last]
            section "Resource $i" {
                header
                bytes [expr $size - 4] "Data"
            }
            set i [incr $i]
        }
        set last $offset
    }

    bytes eof
    set size [expr [pos] - $last]
    move -$size

    section "Resource [incr $i]" {
        header
        bytes [expr $size - 4] "Data"
    }
}
