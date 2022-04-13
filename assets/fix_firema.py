import sys
from pathlib import Path
from typing import List, Literal

THIS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(THIS_DIR.parent))
from diabutil_python import decompose_cel, serialize_with_groups, UINT16_SIZE, UINT8_SIZE

FIREMA_NUM_GROUPS: int = 8


def fix_frame(data: bytes) -> bytes:
    """Pad the right side of each row to fit desired width"""

    OLD_WIDTH = 96
    DESIRED_WIDTH = 128

    header_num_elements = 5
    header_size = header_num_elements * UINT16_SIZE

    # column index
    x = 0
    # row index
    y = 0
    # new skip header
    header_offsets = [header_size]
    # data with padding added
    newdata = b''

    # We make our own header so skip the existing header
    i = 5 * UINT16_SIZE
    while i < len(data):
        # Read+copy instruction
        newdata += data[i:i + UINT8_SIZE]
        instruction = int.from_bytes(data[i:i + UINT8_SIZE],
                                     'little',
                                     signed=True)
        i += UINT8_SIZE

        assert (instruction != 0)

        if instruction < 0:
            # transparent
            width = -instruction
            assert (width > 0)
        else:
            # copy data based on copy instruction
            width = instruction
            newdata += data[i:i + width]
            i += width

        x += width
        assert (x <= OLD_WIDTH)
        if x == OLD_WIDTH:
            # Pad with transparency (this is the entire purpose of this script)
            pad = (-(DESIRED_WIDTH - OLD_WIDTH)).to_bytes(UINT8_SIZE,
                                                          'little',
                                                          signed=True)
            newdata += pad

            # retrace to next row
            x = 0
            y += 1

            if y % 32 == 0:
                header_offsets.append(len(newdata) + header_size)
                pass

    assert (len(header_offsets) == header_num_elements)

    return b''.join(
        [offset.to_bytes(UINT16_SIZE, 'little')
         for offset in header_offsets]) + newdata


def main() -> int:
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} firema.cel output.cel')
        return 1

    original_cel = decompose_cel(
        Path(sys.argv[1]).read_bytes(), FIREMA_NUM_GROUPS)

    fixed_cel = [[fix_frame(frame) for frame in group]
                 for group in original_cel]

    Path(sys.argv[2]).write_bytes(serialize_with_groups(fixed_cel))

    return 0


if __name__ == '__main__':
    sys.exit(main())
