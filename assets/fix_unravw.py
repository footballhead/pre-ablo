import sys
from pathlib import Path

# This script is not used because, even after fixing the header, unravw suffers
# the same problems as unrava... The height is wrong, there are the wrong
# number of frames, and the graphics are randomly translated and cropped.


def main() -> int:
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} unravw.cel output.cel')
        return 1

    unravw_bytes = Path(sys.argv[1]).read_bytes()

    # There's no pattern to fix the header so just hardcode what we expect.
    # This came from manual inspection of retail unravw.cel in hexed.it
    new_header = b'\x20\x00\x00\x00' +\
                 b'\x07\x27\x00\x00' +\
                 b'\xc5\x49\x00\x00' +\
                 b'\xa3\x6b\x00\x00' +\
                 b'\xc3\x86\x00\x00' +\
                 b'\x26\xa9\x00\x00' +\
                 b'\x4c\xd0\x00\x00' +\
                 b'\x93\xed\x00\x00'

    # Chop off old header, add our header
    Path(sys.argv[2]).write_bytes(new_header + unravw_bytes[len(new_header):])

    return 0


if __name__ == '__main__':
    sys.exit(main())
