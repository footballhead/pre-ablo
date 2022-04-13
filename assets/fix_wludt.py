import sys
import itertools
from pathlib import Path

THIS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(THIS_DIR.parent))
from diabutil_python import decompose_cel, serialize_with_groups

WLUDT_NUM_GROUPS: int = 8


def fix_header(data: bytes) -> bytes:
    # There's no pattern to fix the header so just hardcode what we expect.
    # This was created via manual inspection in hexed.it
    new_header = b'\x20\x00\x00\x00' +\
                 b'\xd9\x72\x00\x00' +\
                 b'\x0f\xee\x00\x00' +\
                 b'\xb8\x5f\x01\x00' +\
                 b'\x31\xc8\x01\x00' +\
                 b'\x09\x2d\x02\x00' +\
                 b'\x5d\xa0\x02\x00' +\
                 b'\x98\x12\x03\x00'
    return new_header + data[len(new_header):]


def main() -> int:
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} wludt.cel output.cel')
        return 1

    # Header is broken. Fix it first so our algorithms can read it
    wludt_fixed_header = fix_header(Path(sys.argv[1]).read_bytes())

    # Correct the number of frames per group
    # (derived from manual inspection with TDG)
    fixed_cel = []
    flat_frames = list(
        itertools.chain.from_iterable(
            decompose_cel(wludt_fixed_header, WLUDT_NUM_GROUPS)))
    for num in [20, 20, 20, 20, 20, 16, 20, 16]:
        fixed_cel.append(flat_frames[:num])
        flat_frames = flat_frames[num:]

    # Duplicate frames to fix groups 6 and 8. We're lucky in that this is the
    # logical end of the animation, so all we miss is the blood pool growing.
    # If I didn't point it out to you, I doubt you'd notice :)
    fixed_cel[5] += fixed_cel[5][-1:] * 4
    fixed_cel[7] += fixed_cel[7][-1:] * 4

    # Write result
    Path(sys.argv[2]).write_bytes(serialize_with_groups(fixed_cel))

    return 0


if __name__ == '__main__':
    sys.exit(main())
