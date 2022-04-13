import sys
from collections import namedtuple
from pathlib import Path
from typing import List

THIS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(THIS_DIR.parent))
from diabutil_python import decompose_no_groups, UINT16_SIZE, serialize_no_groups

MiniTile = namedtuple('MiniTile', ['number', 'type'])


def parse_mini_tile(data: int) -> MiniTile:
    return MiniTile(number=(data & 0x0FFF), type=((data & 0x7000) >> 12))


def parse_min(data: bytes) -> List[MiniTile]:
    return [
        parse_mini_tile(int.from_bytes(data[i:i + UINT16_SIZE], 'little'))
        for i in range(0, len(data), UINT16_SIZE)
    ]


def main() -> int:
    if len(sys.argv) != 4:
        print(f'Usage: python {sys.argv[0]} l2.cel l2.min out.cel')
        return 1

    # TODO: These functions are a straight port of the C++ code and could be written to be more pythonic

    def remove_2_bytes(data: bytes, i: int) -> bytes:
        return data[:i] + data[i + 2:]

    def fix_left_triangle_to_wall(data: bytes) -> bytes:
        data = remove_2_bytes(data, 0x000)
        data = remove_2_bytes(data, 0x006)
        data = remove_2_bytes(data, 0x014)
        data = remove_2_bytes(data, 0x02A)
        data = remove_2_bytes(data, 0x048)
        data = remove_2_bytes(data, 0x06E)
        data = remove_2_bytes(data, 0x09C)
        data = remove_2_bytes(data, 0x0D2)
        return data

    def fix_right_triangle_to_wall(data: bytes) -> bytes:
        data = remove_2_bytes(data, 0x002)
        data = remove_2_bytes(data, 0x00C)
        data = remove_2_bytes(data, 0x01E)
        data = remove_2_bytes(data, 0x038)
        data = remove_2_bytes(data, 0x05A)
        data = remove_2_bytes(data, 0x084)
        data = remove_2_bytes(data, 0x0B6)
        data = remove_2_bytes(data, 0x0F0)
        return data

    def fix_left_triangle(data: bytes) -> bytes:
        data = fix_left_triangle_to_wall(data)
        data = remove_2_bytes(data, 0x110)
        data = remove_2_bytes(data, 0x14A)
        data = remove_2_bytes(data, 0x17C)
        data = remove_2_bytes(data, 0x1A6)
        data = remove_2_bytes(data, 0x1C8)
        data = remove_2_bytes(data, 0x1E2)
        data = remove_2_bytes(data, 0x1F4)
        data = remove_2_bytes(data, 0x1FE)
        return data

    def fix_right_triangle(data: bytes) -> bytes:
        data = fix_right_triangle_to_wall(data)
        data = remove_2_bytes(data, 0x12E)
        data = remove_2_bytes(data, 0x164)
        data = remove_2_bytes(data, 0x192)
        data = remove_2_bytes(data, 0x1B8)
        data = remove_2_bytes(data, 0x1D6)
        data = remove_2_bytes(data, 0x1EC)
        data = remove_2_bytes(data, 0x1FA)
        data = remove_2_bytes(data, 0x200)
        return data

    switch = [
        None,  # Padding, do nothing
        None,  # Padding, do nothing
        fix_left_triangle,
        fix_right_triangle,
        fix_left_triangle_to_wall,
        fix_right_triangle_to_wall,
        # Do nothing for other cases
    ]

    cel = decompose_no_groups(Path(sys.argv[1]).read_bytes())
    fixed = set()
    min = parse_min(Path(sys.argv[2]).read_bytes())

    # Remove 2 bytes of black pixels from each row of affected graphics
    for tile in min:
        if switch[tile.type]:
            # Diablo loves 1-based arrays. We use 0-based arrays for CEL frame-indexing
            tile_number = tile.number - 1
            # Don't fix the same tile twice!
            if not tile_number in fixed:
                cel[tile_number] = switch[tile.type](cel[tile_number])
                fixed = fixed | {tile_number}

    print(f'Fixed {len(fixed)} tiles')

    Path(sys.argv[3]).write_bytes(serialize_no_groups(cel))

    return 0


if __name__ == '__main__':
    sys.exit(main())
