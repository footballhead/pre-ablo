import subprocess

from pathlib import Path

from gooey import Gooey, GooeyParser


def vcdiff_decode(dictionary, target, delta):
    args = ['vcdiff.exe', 'decode', '--dictionary', str(dictionary.resolve()),
                                    '--target', str(target.resolve()),
                                    '--delta', str(delta.resolve())]
    print(' '.join(args))
    return subprocess.run(args, capture_output=True)


@Gooey(program_name='open-vcdiff GUI')
def main():
    parser = GooeyParser(description="GUI for making patches using open-vcdiff") 
    parser.add_argument('Base', help="Unmodified file", type=Path, widget="FileChooser")
    parser.add_argument('Delta', help="VCDIFF encoded patch file", type=Path, widget="FileChooser")
    parser.add_argument('Output', help="Where to store newly patched binary", type=Path, widget="FileSaver")

    args = parser.parse_args()

    proc = vcdiff_decode(args.Base, args.Output, args.Delta)
    print(proc.stdout.decode('utf-8'))
    print(proc.stderr.decode('utf-8'))
    print('Success' if proc.returncode == 0 else 'Failure!')


if __name__ == '__main__':
    main()
