from conans import tools, ConanFile


class DirectDrawWrapperConan(ConanFile):
    name = 'ddrawwrapper'
    version = '0.1'
    description = 'DirectDraw Wrapper'

    _download_url = 'https://github.com/footballhead/diablo-ddrawwrapper/releases/download/v0.1.0/ddrawwrapper-prde-v0.1.0.zip'
    _download_sha256 = '51df71040456851cdf9ebc34bb8ca5fbbcbf013d30061d1c6dc8cd2c381b63c7'

    def source(self):
        tools.get(self._download_url, sha256=self._download_sha256)

    def package(self):
        self.copy("*", src=self.source_folder)
