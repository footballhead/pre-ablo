'''
Integration test runner script, must build first!

unittest is used as the framework to describe and group tests
'''

# Please run yapf on me!

from pathlib import Path
import subprocess
import unittest
import sys

# TODO: make BUILD_DIR configurable or smarter
BUILD_DIR = Path('..') / 'build'
# TODO: script to setup ASSETS_DIR
ASSETS_DIR = Path('assets')
OUTPUT_DIR = Path('output')
cel2png = BUILD_DIR / 'cel2png' / 'cel2png'
drawtext = BUILD_DIR / 'drawtext' / 'drawtext'
drawtextpcx = BUILD_DIR / 'drawtextpcx' / 'drawtextpcx'
fixamp = BUILD_DIR / 'fixamp' / 'fixamp'
joincel = BUILD_DIR / 'joincel' / 'joincel'
joingroups = BUILD_DIR / 'joingroups' / 'joingroups'
mpqadd = BUILD_DIR / 'mpqadd' / 'mpqadd'
mpqextract = BUILD_DIR / 'mpqextract' / 'mpqextract'
retail2prdemo = BUILD_DIR / 'retail2prdemo' / 'retail2prdemo'
splitcel = BUILD_DIR / 'splitcel' / 'splitcel'
splitgroups = BUILD_DIR / 'splitgroups' / 'splitgroups'


class TestCel2Png(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([cel2png])
        self.assertEqual(process.returncode, 1)


class TestDrawText(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([drawtext])
        self.assertEqual(process.returncode, 1)

    def test_bigtgold_is_successful(self):
        bigtgold_cel = ASSETS_DIR / 'bigtgold.cel'
        mainmenu_pal = ASSETS_DIR / 'mainmenu.pal'
        output_png = OUTPUT_DIR / 'TestDrawText_test_bigtgold_is_successful.png'
        message = "Don't have a cow, man"
        process = subprocess.run(
            [drawtext, bigtgold_cel, mainmenu_pal, output_png, message])
        self.assertEqual(process.returncode, 0)
        # TODO: Compare output file


class TestDrawTextPcx(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([drawtextpcx])
        self.assertEqual(process.returncode, 1)

    def test_valid_params_is_successful(self):
        pcx_file = ASSETS_DIR / 'font42y.pcx'
        bin_file = ASSETS_DIR / 'font42.bin'
        out_file = OUTPUT_DIR / 'TestDrawTextPcx_test_valid_params_is_successful.png'
        message = "Steamed hams"
        process = subprocess.run(
            [drawtextpcx, pcx_file, bin_file, out_file, message])
        self.assertEqual(process.returncode, 0)
        # TODO compare to a golden file
        # TODO add output param


class TestFixAmp(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([fixamp])
        self.assertEqual(process.returncode, 1)


class TestJoinCel(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([joincel])
        self.assertEqual(process.returncode, 1)


class TestJoinGroups(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([joingroups])
        self.assertEqual(process.returncode, 1)


class TestMpqAdd(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([mpqadd])
        self.assertEqual(process.returncode, 1)


class TestMpqExtract(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([mpqextract])
        self.assertEqual(process.returncode, 1)


class TestSplitCel(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([splitcel])
        self.assertEqual(process.returncode, 1)


class TestSplitGroups(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([splitgroups])
        self.assertEqual(process.returncode, 1)


if __name__ == '__main__':
    OUTPUT_DIR.mkdir(exist_ok=True)
    unittest.main()
