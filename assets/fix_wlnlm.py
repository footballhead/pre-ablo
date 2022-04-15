import sys
from pathlib import Path
from typing import List


def fix_wlnlm(in_file: Path, out_file: Path):
    wlnlm_bytes = in_file.read_bytes()

    # There's no pattern to fix the header so just hardcode what we expect.
    new_header = b'\x20\x00\x00\x00' +\
                 b'\x50\x91\x00\x00' +\
                 b'\x8b\x1a\x01\x00' +\
                 b'\xfe\xaa\x01\x00' +\
                 b'\x15\x42\x02\x00' +\
                 b'\xf3\xd5\x02\x00' +\
                 b'\x36\x5d\x03\x00' +\
                 b'\xc1\xec\x03\x00'

    # Chop off old header, add our header
    out_file.write_bytes(new_header + wlnlm_bytes[len(new_header):])


def main(argv: List[str]) -> int:
    if len(argv) != 3:
        print(f'Usage: {argv[0]} wlnlm.cel output.cel')
        return 1

    fix_wlnlm(Path(argv[1]), Path(argv[2]))

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
