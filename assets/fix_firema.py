import sys
from pathlib import Path
from typing import List, Literal

FIREMA_NUM_GROUPS = 8
UINT8_SIZE = 1
UINT16_SIZE = 2
UINT32_SIZE = 4


def read_uint32(data: bytes,
                i: int,
                byteorder: Literal['little', 'big'] = 'little') -> int:
    """Convert 4 bytes (data[i:i+4]) into an int"""
    return int.from_bytes(data[i:i + UINT32_SIZE], byteorder=byteorder)


def find_groups(data: bytes, num_groups: int) -> List[bytes]:
    """Use the group header to split the byte stream into discrete CELs"""
    assert (num_groups > 0)

    groups = []
    for i in range(num_groups):
        begin = read_uint32(data, i * 4)
        if i == num_groups - 1:
            end = len(data)
        else:
            end = read_uint32(data, (i + 1) * 4)

        groups.append(data[begin:end])

    return groups


def find_frames(data: bytes) -> List[bytes]:
    """Use the CEL header to split the byte stream into discrete frames"""
    frames = []

    num_frames = read_uint32(data, 0)
    for i in range(num_frames):
        # Start at UINT32_SIZE to skip num_frames uint32 (which we already read)
        begin = read_uint32(data, UINT32_SIZE + i * UINT32_SIZE)
        end = read_uint32(data, UINT32_SIZE + (i + 1) * UINT32_SIZE)

        frames.append(data[begin:end])

    return frames


def serialize(groups: List[List[bytes]]) -> bytes:
    """Turn a cel[group][frame] structure into a stream of .CEL bytes"""

    def join_frames(frames: List[bytes]) -> bytes:
        """Turn all frames into a serializable chunk with a header"""
        header = len(frames).to_bytes(UINT32_SIZE, 'little')

        # Header is 1 uint32 for num_frames then n+1 uint32 for frame start/end
        header_size = (len(frames) + 2) * UINT32_SIZE
        offset = header_size
        for frame in frames:
            header += offset.to_bytes(UINT32_SIZE, 'little')
            offset += len(frame)
        header += offset.to_bytes(UINT32_SIZE, 'little')

        return header + b''.join(frames)

    def join_groups(groups: List[bytes]) -> bytes:
        """Turn all groups into a serializable chunk with a header"""

        header = b''

        header_size = len(groups) * UINT32_SIZE
        offset = header_size
        for group in groups:
            header += offset.to_bytes(UINT32_SIZE, 'little')
            offset += len(group)

        return header + b''.join(groups)

    return join_groups([join_frames(frames) for frames in groups])


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

    # 2d array, indexed by group# then frame#
    fixed = []

    firema_bytes = Path(sys.argv[1]).read_bytes()
    groups = find_groups(firema_bytes, FIREMA_NUM_GROUPS)
    for group in groups:
        frames = find_frames(group)
        fixed.append([fix_frame(frame) for frame in frames])

    Path(sys.argv[2]).write_bytes(serialize(fixed))

    return 0


if __name__ == '__main__':
    sys.exit(main())
