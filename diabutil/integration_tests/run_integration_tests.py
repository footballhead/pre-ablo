'''
Integration test runner script, must build first!

unittest is used as the framework to describe and group tests
'''

# Please run yapf on me!

# TODO: Capture program output and only print on test failure?

from pathlib import Path

import os
import subprocess
import unittest
import sys
import shutil
import filecmp

# TODO: make BUILD_DIR configurable or smarter
BUILD_DIR = (Path('..') / 'build').resolve()
ASSETS_DIR = Path('assets').resolve()
GOLDEN_DIR = Path('golden').resolve()
OUTPUT_DIR = Path('output').resolve()
MISSING_GFX_DIR = (Path('..') / '..' / 'assets').resolve()

cel2png = BUILD_DIR / 'cel2png' / 'cel2png'
drawtext = BUILD_DIR / 'drawtext' / 'drawtext'
drawtextpcx = BUILD_DIR / 'drawtextpcx' / 'drawtextpcx'
mpqadd = BUILD_DIR / 'mpqadd' / 'mpqadd'
mpqextract = BUILD_DIR / 'mpqextract' / 'mpqextract'


class TestCel2Png(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([cel2png])
        self.assertEqual(process.returncode, 1)


class TestDrawText(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([drawtext])
        self.assertEqual(process.returncode, 1)

    def test_bigtgold_is_successful(self):
        bigtgold_cel = ASSETS_DIR / 'data' / 'bigtgold.cel'
        mainmenu_pal = ASSETS_DIR / 'gendata' / 'mainmenu.pal'
        output_png = OUTPUT_DIR / 'TestDrawText_test_bigtgold_is_successful.png'
        message = "DON'T HAVE A COW, MAN"

        process = subprocess.run(
            [drawtext, bigtgold_cel, mainmenu_pal, output_png, message])
        self.assertEqual(process.returncode, 0)

        golden = GOLDEN_DIR / 'drawtext.png'
        self.assertTrue(filecmp.cmp(output_png, golden))


class TestDrawTextPcx(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([drawtextpcx])
        self.assertEqual(process.returncode, 1)

    def test_valid_params_is_successful(self):
        pcx_file = ASSETS_DIR / 'ui_art' / 'font42g.pcx'
        bin_file = ASSETS_DIR / 'ui_art' / 'font42.bin'
        out_file = OUTPUT_DIR / 'TestDrawTextPcx_test_valid_params_is_successful.png'
        # Note: this font has different upper and lower case letters!
        message = "STEAMED HAMS"

        process = subprocess.run(
            [drawtextpcx, pcx_file, bin_file, out_file, message])
        self.assertEqual(process.returncode, 0)

        golden = GOLDEN_DIR / 'drawtextpcx.png'
        self.assertTrue(filecmp.cmp(out_file, golden))


class TestMpqAdd(unittest.TestCase):
    # TODO: Don't run if WITH_STORMLIB is OFF

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([mpqadd])
        self.assertEqual(process.returncode, 1)


class TestMpqExtract(unittest.TestCase):
    # TODO: Don't run if WITH_STORMLIB is OFF

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([mpqextract])
        self.assertEqual(process.returncode, 1)


if __name__ == '__main__':
    # TODO: Will these run using default unittests options?
    if not ASSETS_DIR.is_dir():
        print("Missing assets/ dir, run ./setup_assets.sh (see README)")
        exit(1)

    OUTPUT_DIR.mkdir(exist_ok=True)

    unittest.main()
