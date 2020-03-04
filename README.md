# NewsBlockchain

| 构建平台  | 结果                                                       |
| --------- | ------------------------------------------------------------ |
| Travis CI | [![Build Status](https://www.travis-ci.com/kaaass/NewsBlockchain.svg?branch=master)](https://www.travis-ci.com/kaaass/NewsBlockchain) |
| Appveyor  | [![Build status](https://ci.appveyor.com/api/projects/status/btr7ocm57pv4dr0l?svg=true)](https://ci.appveyor.com/project/kaaass/newsblockchain) |
| Codecov   | [![codecov](https://codecov.io/gh/kaaass/NewsBlockchain/branch/master/graph/badge.svg)](https://codecov.io/gh/kaaass/NewsBlockchain) |

一个简易的区块链模拟，用于存放、校验新闻。

本仓库为吉林大学软件学院数据结构荣誉课程(2018级)课程项目的代码仓库。

## 开发进程

|                      feature-blockchain                      |                         feature-hash                         |                       feature-huffman                        |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| [![Build Status](https://www.travis-ci.com/kaaass/NewsBlockchain.svg?branch=feature-blockchain)](https://www.travis-ci.com/kaaass/NewsBlockchain) | [![Build Status](https://www.travis-ci.com/kaaass/NewsBlockchain.svg?branch=feature-hash)](https://www.travis-ci.com/kaaass/NewsBlockchain) | [![Build Status](https://www.travis-ci.com/kaaass/NewsBlockchain.svg?branch=feature-huffman)](https://www.travis-ci.com/kaaass/NewsBlockchain) |
| [![Build status](https://ci.appveyor.com/api/projects/status/btr7ocm57pv4dr0l/branch/feature-blockchain?svg=true)](https://ci.appveyor.com/project/kaaass/newsblockchain/branch/feature-blockchain) | [![Build status](https://ci.appveyor.com/api/projects/status/btr7ocm57pv4dr0l/branch/feature-hash?svg=true)](https://ci.appveyor.com/project/kaaass/newsblockchain/branch/feature-hash) | [![Build status](https://ci.appveyor.com/api/projects/status/btr7ocm57pv4dr0l/branch/feature-huffman?svg=true)](https://ci.appveyor.com/project/kaaass/newsblockchain/branch/feature-huffman) |
| [![codecov](https://codecov.io/gh/kaaass/NewsBlockchain/branch/feature-blockchain/graph/badge.svg)](https://codecov.io/gh/kaaass/NewsBlockchain) | [![codecov](https://codecov.io/gh/kaaass/NewsBlockchain/branch/feature-hash/graph/badge.svg)](https://codecov.io/gh/kaaass/NewsBlockchain) | [![codecov](https://codecov.io/gh/kaaass/NewsBlockchain/branch/feature-huffman/graph/badge.svg)](https://codecov.io/gh/kaaass/NewsBlockchain) |
|             [@KAAAsS](https://github.com/kaaass)             |       [@Victoria1216](https://github.com/Victoria1216)       |             [@ZT](https://github.com/ztmail723)              |


## 使用

### 准备工作

1. 安装CMake
2. 克隆本仓库
3. `git submodule update --init --recursive`

### 编译

执行

```shell script
mkdir build
cd build
cmake .. -DENABLE_COVERAGE=On
cmake --build . --config Release
```

## 致谢

[CMake-codecov](https://github.com/RWTH-HPC/CMake-codecov)

[clang_travis_cmake_gtest_coveralls_example](https://github.com/david-grs/clang_travis_cmake_gtest_coveralls_example)