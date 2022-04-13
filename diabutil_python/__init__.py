from typing import List, Literal

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


def decompose_cel(data: bytes, num_groups: int) -> List[List[bytes]]:
    """Turn byte stream into cel[group][frame]. Works on both .CEL and .CL2"""
    assert (num_groups > 0)

    cel = []

    for group_i in range(num_groups):
        cel.append([])

        group_offset = read_uint32(data, group_i * UINT32_SIZE)
        num_frames = read_uint32(data, group_offset)
        for frame_i in range(num_frames):
            # + UINT32_SIZE to skip num_frames (already read!)
            # + group_offset to make indices relative to beginning of file
            begin = read_uint32(
                data, group_offset + UINT32_SIZE +
                frame_i * UINT32_SIZE) + group_offset
            end = read_uint32(
                data, group_offset + UINT32_SIZE +
                (frame_i + 1) * UINT32_SIZE) + group_offset
            cel[group_i].append(data[begin:end])

    return cel


def _join_frames(frames: List[bytes]) -> bytes:
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


def _join_groups(groups: List[bytes]) -> bytes:
    """Turn all groups into a serializable chunk with a header"""

    header = b''

    header_size = len(groups) * UINT32_SIZE
    offset = header_size
    for group in groups:
        header += offset.to_bytes(UINT32_SIZE, 'little')
        offset += len(group)

    return header + b''.join(groups)


def serialize_no_groups(cel: List[bytes]) -> bytes:
    """Turn a cel[frame] into a stream of .CEL bytes"""

    return _join_frames(cel)


def serialize_with_groups(cel: List[List[bytes]]) -> bytes:
    """Turn a cel[group][frame] structure into a stream of .CEL bytes"""

    return _join_groups([_join_frames(group) for group in cel])


class CelBuilder:

    def __init__(self, width: int):
        self._width = width
        self._x = 0
        self._y = 0
        self._header = [FRAME_HEADER_SIZE]
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
            # TODO: Don't force row size to be image width.

    def copy(self, data: bytes) -> None:
        assert (data)
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

    def transparent(self, n: int):
        assert (n > 0)
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
            self._header += [0
                             ] * (FRAME_HEADER_NUM_ENTRIES - len(self._header))
        # Slice off unwanted headers (e.g. graphics taller than 160 px)
        if len(self._header) > FRAME_HEADER_NUM_ENTRIES:
            self._header = self._header[:FRAME_HEADER_NUM_ENTRIES]

        header_bytes = b''.join(
            [x.to_bytes(UINT16_SIZE, 'little') for x in self._header])
        return header_bytes + self._data
