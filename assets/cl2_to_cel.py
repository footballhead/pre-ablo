from email import header
from lib2to3.pytree import convert
import sys
import argparse
from pathlib import Path
from typing import Literal, List

UINT8_SIZE: int = 1
UINT16_SIZE: int = 2
UINT32_SIZE: int = 4
FRAME_HEADER_NUM_ENTRIES: int = 5
FRAME_HEADER_SIZE: int = FRAME_HEADER_NUM_ENTRIES * UINT16_SIZE
INT8_MAX: int = 127
SKIP_HEADER_INTERVAL: int = 32

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


def decompose_cl2(data: bytes, num_groups: int) -> List[List[bytes]]:
    """Turn byte stream into cl2[group][frame]"""
    assert(num_groups > 0)

    cl2 = []

    for group_i in range(num_groups):
        cl2.append([])

        group_offset = read_uint32(data, group_i * UINT32_SIZE)
        num_frames = read_uint32(data, group_offset)
        for frame_i in range(num_frames):
            # + UINT32_SIZE to skip num_frames (already read!)
            # + group_offset to make indices relative to beginning of file
            begin = read_uint32(data, group_offset + UINT32_SIZE + frame_i * UINT32_SIZE) + group_offset
            end = read_uint32(data, group_offset + UINT32_SIZE + (frame_i + 1) * UINT32_SIZE) + group_offset
            cl2[group_i].append(data[begin:end])

    return cl2


class CelBuilder:

    def __init__(self, width: int):
        self._width = width
        self._x = 0
        self._y = 0
        self._header = [FRAME_HEADER_SIZE]
        # self._until_next_header = self._width * SKIP_HEADER_INTERVAL
        self._data = b''

    def _advance_column(self, n: int):
        self._x += n
        if self._x >= self._width:
            self._next_row()

    def _next_row(self):
        self._x -= self._width
        self._y += 1

        if self._y % SKIP_HEADER_INTERVAL == 0:
            self._header.append(len(self._data) + FRAME_HEADER_SIZE)
        pass

    def copy(self, data: bytes) -> None:
        assert(data)
        to_copy = data[:]
        while to_copy:
            remaining = self._width - self._x
            size = min(len(to_copy), remaining, INT8_MAX)
        
            # Insert CEL `copy` instruction
            self._data += size.to_bytes(1, 'little')
            # Append `copy` operands
            self._data += to_copy[:size]

            self._advance_column(size)
            to_copy = to_copy[size:]

    def fill(self, val: bytes, n: int) -> None:
        assert(len(val) == UINT8_SIZE)
        assert(n > 0)
        while n > 0:
            remaining = self._width - self._x
            size = min(n, remaining, INT8_MAX)

            # Insert CEL `copy` instruction
            self._data += size.to_bytes(1, 'little')
            # Turn fill into copy
            self._data += val * size

            self._advance_column(size)
            n -= size

    def transparent(self, n: int):
        assert(n > 0)
        while n > 0:
            remaining = self._width - self._x
            size = min(n, remaining, INT8_MAX)

            # Insert CEL `transparent` instruction
            self._data += (-size).to_bytes(1, 'little', signed=True)

            self._advance_column(size)
            n -= size

    def build(self) -> bytes:
        # Pad self._header with 0's if need be
        if len(self._header) < FRAME_HEADER_NUM_ENTRIES:
            self._header += [0] * (FRAME_HEADER_NUM_ENTRIES - len(self._header))
        # Slice off unwanted headers (e.g. graphics taller than 160 px)
        if len(self._header) > FRAME_HEADER_NUM_ENTRIES:
            self._header = self._header[:FRAME_HEADER_NUM_ENTRIES]

        header_bytes = b''.join([x.to_bytes(UINT16_SIZE, 'little') for x in self._header])
        return header_bytes + self._data


def convert_frame(data: bytes, width: int) -> bytes:
    """Convert a stream of CL2 bytes into a stream of CEL bytes"""

    builder = CelBuilder(width)

    # Don't use the existing header (we're reading the whole CL2, not skipping parts)
    i = FRAME_HEADER_SIZE
    while i < len(data):
        instruction = int.from_bytes(data[i:i + UINT8_SIZE],
                                     'little',
                                     signed=True)
        i += UINT8_SIZE

        assert (instruction != 0)

        if instruction > 0:
            # transparency
            builder.transparent(instruction)
        elif (-instruction) > 65:
            # fill
            size = (-instruction) - 65
            pixel = data[i:i + UINT8_SIZE]
            i += UINT8_SIZE
            builder.fill(pixel, size)
        else:
            # copy
            size = (-instruction)
            builder.copy(data[i:i + size])
            i += size

    return builder.build()


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
    parser = argparse.ArgumentParser(description='Naively convert CL2 to CEL')
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--width', type=int, required=True)
    parser.add_argument('--groups', type=int, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()

    cl2 = decompose_cl2(args.input.read_bytes(), args.groups)
    cel = []

    for group in cl2:
        cel.append([convert_frame(frame, args.width) for frame in group])

    args.output.write_bytes(serialize(cel))

    return 0


if __name__ == '__main__':
    sys.exit(main())
    