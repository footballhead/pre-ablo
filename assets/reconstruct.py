from pathlib import Path
import os
import subprocess
import sys
import shutil

# Scripts local to current directory (assets/)
from cl2_to_cel import cl2_to_cel
from fix_dungeon_cels import fix_dungeon_cels
from fix_firema import fix_firema
from fix_l3_amp import fix_l3_amp
from fix_wlnlm import fix_wlnlm
from fix_wludt import fix_wludt

THIS_DIR = Path(__file__).resolve().parent


def mpq_extract(mpq: Path, listfile: Path) -> None:
    """Run mpqextract using a listfile on disk. Stores extracted files into the
    current working directory. mpqextract must be in PATH!
    """
    subprocess.run(['mpqextract', str(mpq)],
                   input=listfile.read_bytes(),
                   check=True)


def cl2_convert(file: Path, width: int, groups: int):
    """Convert a .CL2 and place the resulting .CEL in the same directory. Cuts
    down on typing and line length.
    """
    cl2_to_cel(file, width, groups, file.with_suffix('.cel'))


def main() -> int:
    if len(sys.argv) != 5:
        print(
            f'Usage: {sys.argv[0]} prdemo.mpq beta.mpq retail.mpq hellfire.mpq'
        )
        return 1

    if THIS_DIR.name != 'assets':
        print('Run me from the assets/ directory!')
        return 2

    mpq_extract(Path(sys.argv[1]), THIS_DIR / 'listfile_prdemo.txt')
    mpq_extract(Path(sys.argv[2]), THIS_DIR / 'listfile_beta.txt')
    mpq_extract(Path(sys.argv[3]), THIS_DIR / 'listfile_retail.txt')
    mpq_extract(Path(sys.argv[4]), THIS_DIR / 'listfile_hellfire.txt')

    # l2.cel
    fix_dungeon_cels(THIS_DIR / 'levels' / 'l2data' / 'l2.cel',
                     THIS_DIR / 'levels' / 'l2data' / 'l2.min',
                     THIS_DIR / 'levels' / 'l2data' / 'l2.cel')

    # l3.cel
    fix_dungeon_cels(THIS_DIR / 'levels' / 'l3data' / 'l3.cel',
                     THIS_DIR / 'levels' / 'l3data' / 'l3.min',
                     THIS_DIR / 'levels' / 'l3data' / 'l3.cel')

    # l3.amp
    fix_l3_amp(THIS_DIR / 'levels' / 'l3data' / 'l3.amp',
               THIS_DIR / 'levels' / 'l3data' / 'l3.amp')

    # Copy neutral death animations to cover for missing files.
    # We use the plrgfx_frame_fix patch to ensure frames in the code binary match the assets (to avoid crashing)
    for i in {'a', 'b', 'd', 'h', 'm', 's', 't', 'u'}:
        shutil.copy2(
            THIS_DIR / 'plrgfx' / 'warrior' / 'wmn' / 'wmndt.cel',
            THIS_DIR / 'plrgfx' / 'warrior' / f'wm{i}' / f'wm{i}dt.cel')
        shutil.copy2(
            THIS_DIR / 'plrgfx' / 'warrior' / 'whn' / 'whndt.cel',
            THIS_DIR / 'plrgfx' / 'warrior' / f'wh{i}' / f'wh{i}dt.cel')
        shutil.copy2(THIS_DIR / 'plrgfx' / 'rogue' / 'rln' / 'rlndt.cel',
                     THIS_DIR / 'plrgfx' / 'rogue' / f'rl{i}' / f'rl{i}dt.cel')

    # firema.cel
    fix_firema(THIS_DIR / 'monsters' / 'fireman' / 'firema.cel',
               THIS_DIR / 'monsters' / 'fireman' / 'firema.cel')

    # wlnlm.cel
    fix_wlnlm(THIS_DIR / 'plrgfx' / 'warrior' / 'wln' / 'wlnlm.cel',
              THIS_DIR / 'plrgfx' / 'warrior' / 'wln' / 'wlnlm.cel')

    # wludt.cel
    fix_wludt(THIS_DIR / 'plrgfx' / 'warrior' / 'wlu' / 'wludt.cel',
              THIS_DIR / 'plrgfx' / 'warrior' / 'wlu' / 'wludt.cel')

    # Use mages instead of magew because we need a 20 frame animation
    shutil.copy2(THIS_DIR / 'monsters' / 'mage' / 'mages.cl2',
                 THIS_DIR / 'monsters' / 'mage' / 'magew.cl2')

    # Convert monster GFX
    for i in {'a', 'd', 'h', 'n', 'w'}:
        cl2_convert(THIS_DIR / 'monsters' / 'black' / f'black{i}.cl2',
                    width=160,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'diablo' / f'diablo{i}.cl2',
                    width=160,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'goatlord' / f'goatl{i}.cl2',
                    width=160,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'mage' / f'mage{i}.cl2',
                    width=128,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'snake' / f'snake{i}.cl2',
                    width=160,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'succ' / f'scbs{i}.cl2',
                    width=128,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'thin' / f'thin{i}.cl2',
                    width=160,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'tsneak' / f'tsneak{i}.cl2',
                    width=128,
                    groups=8)
        cl2_convert(THIS_DIR / 'monsters' / 'unrav' / f'unrav{i}.cl2',
                    width=96,
                    groups=8)
        # TODO antworm?
    cl2_convert(THIS_DIR / 'monsters' / 'snake' / f'snakes.cl2',
                width=160,
                groups=8)
    cl2_convert(THIS_DIR / 'monsters' / 'thin' / f'thins.cl2',
                width=160,
                groups=8)

    # Cleanup (remove all .CL2 files)
    for root, _, files in os.walk(THIS_DIR / 'monsters'):
        for file in files:
            if file.endswith('.cl2'):
                (Path(root) / file).unlink()

    return 0


if __name__ == '__main__':
    sys.exit(main())
