import sys
from pathlib import Path

UINT16_SIZE: int = 2
MAPFLAG_TYPE_MASK: int = 0xF
MAPFLAG_DIRT: int = 0x40 << 8


def main() -> int:
    if len(sys.argv) != 3:
        print(f'Usage: python {sys.argv[0]} l3.amp out.amp')
        print('Turn unrecognized tiles into dirt for for better visibility')
        return 1

    fixed = b''

    amp = Path(sys.argv[1]).read_bytes()
    while amp:
        uint16 = int.from_bytes(amp[:UINT16_SIZE], 'little')

        type = uint16 & MAPFLAG_TYPE_MASK
        flags = uint16 ^ type

        # PR demo only understands types 1-6, but retail defines types 8-12
        # specifically for caves e.g. 8 is "SW corner" so devilution uses two
        # flags: do_vert = TRUE and do_cave_horz = TRUE. My guess is it has
        # something to do with caves having more TILs representing corners of
        # different orientations.
        if type in {8, 9, 10, 11, 12}:
            type = 0
            flags |= MAPFLAG_DIRT

        fixed += (type | flags).to_bytes(UINT16_SIZE, 'little')
        amp = amp[UINT16_SIZE:]

    Path(sys.argv[2]).write_bytes(fixed)

    return 0


if __name__ == '__main__':
    sys.exit(main())
