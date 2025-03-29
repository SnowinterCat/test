# Test

## 简介 ([English](/README.md))

## 如何构建?
Test 使用 xmake 构建，相关 xmake 教程请查看 [xmake-io](https://xmake.io/).

### before build

本项目的某些依赖库是系统库（如 libx11-dev）或需要自行安装的第三方库（如 vulkansdk、pkg-config）。在使用 xmake 配置之前，您需要先安装它们。

当然，如果某些可以通过 xmake 安装的第三方库你也想使用系统中的库，也可以在安装后使用 xmake 配置。

但以下库必须自行安装，这里以 ubuntu 24.04 为例：
```
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.296-noble.list https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-noble.list
sudo apt update

sudo apt install libx11-dev libwayland-dev libxext-dev pkg-config vulkan-sdk
```
对于Windows系统操作会简单许多，仅仅只需要到以下网站安装vulkansdk即可：
```
https://vulkan.lunarg.com/sdk/home
```

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