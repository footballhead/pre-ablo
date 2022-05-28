"""
Regenerates storm.def based on a dumpbin of storm.dll

If there are customizations to storm.def (e.g. to accomodate MSVC name
decoration for __stdcall) then it is unwise to run this again!

Assumptions:

* storm.dll in current dir
* dumpbin from MSVC in PATH
* can open and write to storm.def
"""

import re
import subprocess

from pathlib import Path

# Groups: ordinal (int), hint (hex), RVA (hex), name (str)
EXPORTS_TABLE_FORMAT_REGEX = \
    '(\d+)\s+([0-9A-F]+)\s+([0-9A-F]+)\s+([a-zA-Z0-F_]+)'

def main():
    exports = subprocess.run(
        ['dumpbin', '/exports', 'storm.dll'],
        capture_output=True, text=True, check=True)

    with Path('storm.def').open('w') as f:
        f.write('LIBRARY STORM\n')
        f.write('EXPORTS\n')

        # split into lines based on newline
        # cop off irrelevant output at the top (first 19 lines)
        for line in exports.stdout.split('\n')[19:]:
            m = re.search(EXPORTS_TABLE_FORMAT_REGEX, line)
            if m:
                f.write(f'    {m[4]} @{m[1]}\n')

    # TODO: Can we include the number of bytes pushed onto the stack postfix?

if __name__ == '__main__':
    main()