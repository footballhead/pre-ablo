'''
Integration test runner script, must build first!

unittest is used as the framework to describe and group tests
'''

# Please run yapf on me!

# TODO: Capture program output and only print on test failure?

from pathlib import Path

import subprocess
import unittest
import sys
import shutil
import filecmp

# TODO: Make these absolute so we can chdir
# TODO: make BUILD_DIR configurable or smarter
BUILD_DIR = Path('..') / 'build'
ASSETS_DIR = Path('assets')
GOLDEN_DIR = Path('golden')
OUTPUT_DIR = Path('output')
MISSING_GFX_DIR = Path('..') / '..' / 'missing_gfx'

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


class TestFixAmp(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([fixamp])
        self.assertEqual(process.returncode, 1)

    def test_valid_params_is_successful(self):
        # This mutates the file so copy input to output first
        template = ASSETS_DIR / 'levels' / 'l3data' / 'l3.amp'
        amp_file = OUTPUT_DIR / 'TestFixAmp_test_valid_params_is_successful.amp'
        shutil.copy2(template, amp_file)

        process = subprocess.run([fixamp, amp_file])
        self.assertEqual(process.returncode, 0)

        golden = MISSING_GFX_DIR / 'levels' / 'l3data' / 'l3.amp'
        self.assertTrue(filecmp.cmp(amp_file, golden))


class TestJoinCel(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([joincel])
        self.assertEqual(process.returncode, 1)


class TestJoinGroups(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([joingroups])
        self.assertEqual(process.returncode, 1)


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


class TestSplitCel(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([splitcel])
        self.assertEqual(process.returncode, 1)


class TestSplitGroups(unittest.TestCase):

    def test_no_args_nonzero_exit(self):
        process = subprocess.run([splitgroups])
        self.assertEqual(process.returncode, 1)


class TestMultiToolWorkflow(unittest.TestCase):

    def test_splitcel_cel2png(self):
        cel_file = ASSETS_DIR / 'data' / 'bigtgold.cel'
        outdir = OUTPUT_DIR / 'TestMultiToolWorkflow_test_splitcel_cel2png'

        process = subprocess.run([splitcel, cel_file, outdir])
        self.assertEqual(process.returncode, 0)

        frame_file = outdir / '0.celframe'
        pal_file = ASSETS_DIR / 'gendata' / 'mainmenu.pal'
        bigtgold_width = 46
        process = subprocess.run(
            [cel2png, frame_file, pal_file,
             str(bigtgold_width)])
        self.assertEqual(process.returncode, 0)

        # Frame 0 of bigtgold is the letter A
        png_file = outdir / '0.celframe.png'
        golden = GOLDEN_DIR / 'bigtgold_a.png'
        self.assertTrue(filecmp.cmp(png_file, golden))

    def test_splitcel_cel2png_header(self):
        cel_file = ASSETS_DIR / 'data' / 'inv' / 'objcurs.cel'
        outdir = OUTPUT_DIR / 'TestMultiToolWorkflow_test_splitcel_cel2png_header'

        process = subprocess.run([splitcel, cel_file, outdir])
        self.assertEqual(process.returncode, 0)

        frame_file = outdir / '0.celframe'
        pal_file = ASSETS_DIR / 'gendata' / 'mainmenu.pal'
        objcurs_hand_width = 33
        process = subprocess.run([
            cel2png, frame_file, pal_file,
            str(objcurs_hand_width), '--header'
        ])
        self.assertEqual(process.returncode, 0)

        png_file = outdir / '0.celframe.png'
        golden = GOLDEN_DIR / 'objcurs_hand.png'
        self.assertTrue(filecmp.cmp(png_file, golden))


if __name__ == '__main__':
    # TODO: Will these run using default unittests options?
    if not ASSETS_DIR.is_dir():
        print("Missing assets/ dir, run ./setup_assets.sh (see README)")
        exit(1)

    OUTPUT_DIR.mkdir(exist_ok=True)

    unittest.main()
