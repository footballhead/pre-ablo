import sys
from pathlib import Path
from typing import List

THIS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(THIS_DIR.parent))
from diabutil_python import decompose_with_groups, serialize_with_groups

SNAKEW_NUM_GROUPS: int = 8


def fix_snakew(in_file: Path, out_file: Path):
    cel = decompose_with_groups(in_file.read_bytes(), SNAKEW_NUM_GROUPS)

    # Turn an 11 frame animation into 12 by duplicating the last frame
    for group in cel:
        group += group[-1:]

    out_file.write_bytes(serialize_with_groups(cel))


def main(argv: List[str]) -> int:
    if len(argv) != 3:
        print(f'Usage: python {argv[0]} snakew.cel out.cel')
        return 1

    fix_snakew(Path(argv[1]), Path(argv[2]))

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
