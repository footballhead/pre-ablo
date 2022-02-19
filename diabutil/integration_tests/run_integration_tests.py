'''
Integration test runner script, must build first!

unittest is used as the framework to describe and group tests.
'''

# Please run yapf on me!

from pathlib import Path
import subprocess
import unittest

BUILD_DIR = Path('..') / 'build'
cel2png = BUILD_DIR / 'cel2png' / 'cel2png'


class TestCel2Png(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([cel2png])
        self.assertEqual(process.returncode, 1)


if __name__ == '__main__':
    unittest.main()
