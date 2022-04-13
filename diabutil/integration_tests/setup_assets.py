'''
Gather assets for integration tests
'''

# Please run yapf on me!

import os
import sys
import subprocess
from pathlib import Path

# TODO: make BUILD_DIR configurable or smarter
BUILD_DIR = (Path('..') / 'build').resolve()
ASSETS_DIR = Path('assets').resolve()

# TODO: This is different on Windows vs Linux
mpqextract = BUILD_DIR / 'mpqextract' / 'Debug' / 'mpqextract'

# Fed as input to subprocess so must be byte-string
PRDEMO_LISTFILE = b'''data/bigtgold.cel
gendata/mainmenu.pal
'''

# Fed as input to subprocess so must be byte-string
RETAIL_LISTFILE = b'''ui_art/font42.bin
ui_art/font42g.pcx
'''


def main():
    if len(sys.argv) < 3:
        print(f'Usage: {sys.argv[0]} prdemo.mpq retail.mpq')
        exit(1)

    # We chdir so all paths must be absolute
    prdemo_mpq = Path(sys.argv[1]).resolve()
    retail_mpq = Path(sys.argv[2]).resolve()

    ASSETS_DIR.mkdir(parents=True, exist_ok=True)
    os.chdir(ASSETS_DIR)
    subprocess.run([mpqextract, prdemo_mpq], input=PRDEMO_LISTFILE, check=True)
    subprocess.run([mpqextract, retail_mpq], input=RETAIL_LISTFILE, check=True)


if __name__ == '__main__':
    main()
