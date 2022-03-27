# Pre-Release Demo in Conan

This recipe packages our ddrawwrapper DLL for easier inclusion in our CI configuration.

This recipe can only be built on Windows and we only need the DLL to shove into the final distributable .ZIP

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

## Using

* Create the package

```
conan create . gitlab/stable
```

* Upload the package

```
conan upload ddrawwrapper/0.1@gitlab/stable -r diablo-prdemo-patches --all
```