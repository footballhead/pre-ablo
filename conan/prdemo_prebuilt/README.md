# Pre-Release Demo in Conan

This recipe packages the Pre-Release Demo contents as a Conan package.
With a Conan package, the Pre-Release Demo contents can be hosted on our GitLab and accessed more readily by the CI/CD pipeline.

## Host Setup

Install p7zip.

E.g. on Fedora: `sudo dnf install p7zip`

## Conan Setup

* Register the project remote (18183220 is the GitLab Project ID for diablo-prdemo-patches repo):

```
conan remote add diablo-prdemo-patches https://gitlab.com/api/v4/projects/18183220/packages/conan
```

* (Optional) For manually uploading, authenticate with GitLab's conan remote (see
[their docs](https://docs.gitlab.com/ee/user/packages/conan_repository/index.html#authenticate-to-the-package-registry))

```
conan user <gitlab_username or deploy_token_username> -r diablo-prdemo-patches -p <personal_access_token or deploy_token>
```

## Creating the package

* Create the package

```
conan create . gitlab/stable
```

* Upload the package

```
conan upload prdemo/0.1@gitlab/stable -r diablo-prdemo-patches --all
```

## Using the package

Here's a sample `conanfile.txt` that will get the package and copy all the non-
Conan contents to a new `DIABLO` folder:

```
[requires]
prdemo/0.1@gitlab/stable

[imports]
., *.DLL -> ./DIABLO
., *.EXE -> ./DIABLO
., *.RTF -> ./DIABLO
., *.MPQ -> ./DIABLO
., *.BMP -> ./DIABLO
```

Then, with conanfile.txt in the current directory:

```
conan install .
```

Your files will be in `./DIABLO`