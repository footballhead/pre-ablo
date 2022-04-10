import itertools
import sys
from pathlib import Path
from typing import List, Literal

UINT32_SIZE: int = 4


def fix_header(data: bytes) -> bytes:
    # There's no pattern to fix the header so just hardcode what we expect.
    # This was created via manual inspection in hexed.it
    new_header = b'\x20\x00\x00\x00' +\
                 b'\xd9\x72\x00\x00' +\
                 b'\x0f\xee\x00\x00' +\
                 b'\xb8\x5f\x01\x00' +\
                 b'\x31\xc8\x01\x00' +\
                 b'\x09\x2d\x02\x00' +\
                 b'\x5d\xa0\x02\x00' +\
                 b'\x98\x12\x03\x00'
    return new_header + data[len(new_header):]


def read_uint32(data: bytes,
                i: int,
                byteorder: Literal['little', 'big'] = 'little') -> int:
    """Convert 4 bytes (data[i:i+4]) into an int
    
    Args:
        data (bytes)
        i (int): Index into data to the beginning of the int
        byteorder ('little' or 'big'): Byt endianness (default is 'little')

    Returns:
        int: data[i:i + 4] interpretted with byteorder
    """
    return int.from_bytes(data[i:i + UINT32_SIZE], byteorder=byteorder)


def decompose_cel(data: bytes, num_groups: int) -> List[List[bytes]]:
    """Turn byte stream into cl2[group][frame]"""
    assert(num_groups > 0)

    cel = []

    for group_i in range(num_groups):
        cel.append([])

        group_offset = read_uint32(data, group_i * UINT32_SIZE)
        num_frames = read_uint32(data, group_offset)
        for frame_i in range(num_frames):
            # + UINT32_SIZE to skip num_frames (already read!)
            # + group_offset to make indices relative to beginning of file
            begin = read_uint32(data, group_offset + UINT32_SIZE + frame_i * UINT32_SIZE) + group_offset
            end = read_uint32(data, group_offset + UINT32_SIZE + (frame_i + 1) * UINT32_SIZE) + group_offset
            cel[group_i].append(data[begin:end])

    return cel


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


def main() -> int:
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} wludt.cel output.cel')
        return 1

    # Header is broken. Fix it first so our algorithms can read it
    wludt_fixed_header = fix_header(Path(sys.argv[1]).read_bytes())

    # Correct the number of frames per group
    # (derived from manual inspection with TDG)
    fixed_cel = []
    flat_frames = list(itertools.chain.from_iterable(decompose_cel(wludt_fixed_header, 8)))
    for num in [20, 20, 20, 20, 20, 16, 20, 16]:
        fixed_cel.append(flat_frames[:num])
        flat_frames = flat_frames[num:]

    # Duplicate frames to fix groups 6 and 8. We're lucky in that this is the
    # logical end of the animation, so all we miss is the blood pool growing.
    # If I didn't point it out to you, I doubt you'd notice :)
    fixed_cel[5] += fixed_cel[5][-1:] * 4
    fixed_cel[7] += fixed_cel[7][-1:] * 4

    # Write result
    Path(sys.argv[2]).write_bytes(serialize(fixed_cel))

    return 0


if __name__ == '__main__':
    sys.exit(main())
