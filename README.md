# NewsBlockchain

| 构建平台  | 结果                                                       |
| --------- | ------------------------------------------------------------ |
| Travis CI | [![Build Status](https://www.travis-ci.com/kaaass/news_blockchain.svg?branch=master)](https://www.travis-ci.com/kaaass/news_blockchain) |
| Appveyor  | [![Build status](https://ci.appveyor.com/api/projects/status/btr7ocm57pv4dr0l?svg=true)](https://ci.appveyor.com/project/kaaass/newsblockchain) |
| Codecov   | [![codecov](https://codecov.io/gh/kaaass/NewsBlockchain/branch/master/graph/badge.svg)](https://codecov.io/gh/kaaass/NewsBlockchain) |

一个简易的区块链模拟，用于存放、校验新闻。

本仓库为吉林大学软件学院数据结构荣誉课程(2018级)课程项目的代码仓库。

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