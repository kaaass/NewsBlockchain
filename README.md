# NewsBlockchain
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-4-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

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
## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/kaaass"><img src="https://avatars1.githubusercontent.com/u/13360135?v=4" width="100px;" alt=""/><br /><sub><b>KAAAsS</b></sub></a><br /><a href="https://github.com/kaaass/NewsBlockchain/commits?author=kaaass" title="Code">💻</a> <a href="#maintenance-kaaass" title="Maintenance">🚧</a></td>
    <td align="center"><a href="https://github.com/ztmail723"><img src="https://avatars3.githubusercontent.com/u/44718750?v=4" width="100px;" alt=""/><br /><sub><b>ZT</b></sub></a><br /><a href="https://github.com/kaaass/NewsBlockchain/commits?author=ztmail723" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/Victoria1216"><img src="https://avatars3.githubusercontent.com/u/56854008?v=4" width="100px;" alt=""/><br /><sub><b>Victoria1216</b></sub></a><br /><a href="https://github.com/kaaass/NewsBlockchain/commits?author=Victoria1216" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/xiaolllll"><img src="https://avatars3.githubusercontent.com/u/49188794?v=4" width="100px;" alt=""/><br /><sub><b>xiaolllll</b></sub></a><br /><a href="https://github.com/kaaass/NewsBlockchain/commits?author=xiaolllll" title="Code">💻</a></td>
  </tr>
</table>

<!-- markdownlint-enable -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!