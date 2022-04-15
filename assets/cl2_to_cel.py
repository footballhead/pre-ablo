import sys
import argparse
from pathlib import Path
from typing import List

THIS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(THIS_DIR.parent))
from diabutil_python import CelBuilder, decompose_with_groups, FRAME_HEADER_SIZE, serialize_with_groups

UINT8_SIZE: int = 1


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
            # A fill is a copy of the same pixel over and over
            builder.copy(pixel * size)
        else:
            # copy
            size = (-instruction)
            builder.copy(data[i:i + size])
            i += size

    return builder.build()


def cl2_to_cel(input: Path, width: int, groups: int, output: Path) -> None:
    # TODO conversion without --groups specified
    cl2 = decompose_with_groups(input.read_bytes(), groups)
    cel = []

    for group in cl2:
        cel.append([convert_frame(frame, width) for frame in group])

    # TODO conversion without --groups specified
    output.write_bytes(serialize_with_groups(cel))


def main(argv: List[str]) -> int:
    parser = argparse.ArgumentParser(description='Naively convert CL2 to CEL')
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--width', type=int, required=True)
    parser.add_argument('--groups', type=int, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args(argv[1:])

    cl2_to_cel(args.input, args.width, args.groups, args.output)

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
