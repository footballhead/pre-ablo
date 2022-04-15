import sys
from pathlib import Path
from typing import List

THIS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(THIS_DIR.parent))
from diabutil_python import decompose_with_groups, serialize_with_groups

SNAKEA_NUM_GROUPS: int = 8


def fix_snakea(in_file: Path, out_file: Path):
    cel = decompose_with_groups(in_file.read_bytes(), SNAKEA_NUM_GROUPS)

    # Turn a 13 frame animation into 14 by duplicating the last frame
    for group in cel:
        group += group[-1:]

    out_file.write_bytes(serialize_with_groups(cel))


def main(argv: List[str]) -> int:
    if len(argv) != 3:
        print(f'Usage: python {argv[0]} snakew.cel out.cel')
        return 1

    fix_snakea(Path(argv[1]), Path(argv[2]))

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
