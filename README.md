# Distributed key-value storage

[![CI Status](https://github.com/egorpristanskii/distributed_storage/actions/workflows/build.yaml/badge.svg)](https://github.com/egorpristanskii/distributed_storage/actions/workflows/build.yaml) [![Coverage Status](https://coveralls.io/repos/github/egorpristanskii/distributed_storage/badge.svg?branch=main)](https://coveralls.io/github/egorpristanskii/distributed_storage?branch=main)

## Local build

Pull and run docker image

``` bash
docker pull egorpristanskiy/ci_base
docker run --rm -it -v "<path_to_cloned_repo>:/build" --entrypoint /bin/bash egorpristanskiy/ci_base:latest
```

### Release build

Install dependencies

``` bash
conan install . --output-folder=build --build=missing -s build_type=Release
```

Run build

``` bash
cmake --preset=conan-release
cmake --build --preset=conan-release -j$(nproc)
```

Build deb package

``` bash
cmake --build --preset=conan-release --target package
```

#### Build tests and run

Build with tests

``` bash
# Build with tests
cmake --preset=conan-release -DENABLE_TESTS=ON
cmake --build --preset=conan-release -j$(nproc)
```

Run tests

``` bash
ctest --preset=conan-release --output-on-failure
```
