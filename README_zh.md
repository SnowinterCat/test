# Test

## 简介 ([English](/README.md))

## 如何构建?
Test 使用 xmake 构建，相关 xmake 教程请查看 [xmake-io](https://xmake.io/).

### 配置

在构建之前，您需要对项目进行配置。

#### Windows 上使用 msvc
```
xmake f -p windows -a x64 -m releasedbg -k shared --runtimes=MD --3rd_kind=shared -cv
```
#### Windows 上使用 clang-cl
```
xmake f -p windows -a x64 -m releasedbg -k shared --runtimes=MD --toolchain=clang-cl --3rd_kind=shared -cv
```
#### Windows 上使用 mingw
```
xmake f -p mingw -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --3rd_kind=shared -cv
```
#### Linux 上使用 gcc
```
xmake f -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --3rd_kind=shared -cv
```
#### Linux 上使用 gcc-13
```
xmake f -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --toolchain=gcc-13 --3rd_kind=shared -cv
```
#### Linux 上使用 llvm
```
xmake f -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --toolchain=llvm --3rd_kind=shared -cv
```

### 编译

配置完成后，就可以使用此命令编译此项目了：
```
xmake -a
```
或者使用此命令全部重新编译：
```
xmake -ar
```
或者使用此命令显示编译详情：
```
xmake -av
```
以及它们的组合：
```
xmake -arv
```
获取帮助请使用：
```
xmake --help
```