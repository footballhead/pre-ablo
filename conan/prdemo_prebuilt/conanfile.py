import os
import subprocess
from conans import tools, ConanFile
from pathlib import Path


class PreReleaseDemoConan(ConanFile):
    '''
    This recipe requires p7zip installed to extract a .7z file.
    '''

    name = 'prdemo'
    version = '0.1'
    description = 'Diablo Pre-Release Demo'

    # TODO: Conan option for local zip file (for testing)

    _download_url = 'https://diablo-evolution.net/files/prereleasedemo.7z'
    _download_filename = 'prereleasedemo.7z'
    _download_sha256 = 'fc81911c46d14cefeb36c10976677183ce5e4a740b0eae7300c995c7ce826e00'

    def source(self):
        # .7z not 100% supported by conan, so can't use tools.get()
        # (see https://github.com/conan-io/conan/issues/52)
        #
        # Also py7zr chokes on the file too (it doesn't like something about
        # DIABLO.EXE)
        #
        # So we have to rely on 7za program from p7zip

        tools.download(self._download_url,
                       self._download_filename,
                       sha256=self._download_sha256)
        download_path = Path(self.source_folder) / self._download_filename

        # TODO: Support both Windows and Linux
        subprocess.run(
            ['7za', 'e', f'-o{self.source_folder}', '--',
             str(download_path)])

        download_path.unlink()

    def package(self):
        self.copy("*", src=self.source_folder)
