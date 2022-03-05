import sys
from pathlib import Path


def main() -> int:
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} wludt.cel output.cel')
        return 1

    wludt_bytes = Path(sys.argv[1]).read_bytes()

    # There's no pattern to fix the header so just hardcode what we expect.
    new_header = b' \x00\x00\x00\xd9r\x00\x00\x0f\xee\x00\x00\xb8_\x01\x001\xc8\x01\x00\t-\x02\x00]\xa0\x02\x00\x98\x12\x03\x00'

    # Chop off old header, add new header
    Path(sys.argv[2]).write_bytes(new_header + wludt_bytes[len(new_header):])

    return 0


if __name__ == '__main__':
    sys.exit(main())
