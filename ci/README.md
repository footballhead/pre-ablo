# conanfile.txt for CI

This conanfile downloads and installs prebuilts into the `./PRE-ABLO` folder.

## Testing Locally

```
mkdir temp
cd temp
conan install ../ci
```

Verify the contents of the `PRE-ABLO` folder

## Testing on GitLab CI

GitLab is currently configured to build branches, so create and push a branch
