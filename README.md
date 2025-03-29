# Test

## Introduction ([中文](/README_zh.md))

## How to build?
Test is built using xmake, for a related xmake tutorial check out [xmake-io](https://xmake.io/).

### before build

Some of the dependencies of this project are system libraries (e.g. libx11-dev) or third-party libraries that you need to install by yourself (e.g. vulkansdk, pkg-config). You need to install them before configuring with xmake.

Of course, if some of the 3rd-party libraries that can be installed from xmake witch you also want to use the ones from system, you can also use the xmake configuration after installing them.

But the following must be installed by yourself, here is an example for ubuntu 24.04:
```
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.296-noble.list https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-noble.list
sudo apt update

sudo apt install libx11-dev libwayland-dev libxext-dev pkg-config vulkan-sdk
```
For Windows it is much easier, just go to the following website, download and install vulkansdk:
```
https://vulkan.lunarg.com/sdk/home
```

### configure

Before building, you need to configure the project.

#### For windows with msvc
```
xmake f -p windows -a x64 -m releasedbg -k shared --runtimes=MD --3rd_kind=shared -cv
```
#### For windows with clang-cl
```
xmake f -p windows -a x64 -m releasedbg -k shared --runtimes=MD --toolchain=clang-cl --3rd_kind=shared -cv
```
#### For windows with mingw
```
xmake f -p mingw -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --3rd_kind=shared --mingw=/PathToMingw -cv
```
#### For linux with gcc
```
xmake f -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --3rd_kind=shared -cv
```
#### For linux with gcc-13
```
xmake f -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --toolchain=gcc-13 --3rd_kind=shared -cv
```
#### For linux with llvm
```
xmake f -a x86_64 -m releasedbg -k shared --runtimes=stdc++_shared --toolchain=llvm --3rd_kind=shared -cv
```

### compile

After configure, it's time to compile this project with this command:
```
xmake -a
```
Or you can use this command to recompile it all:
```
xmake -ar
```
Or you can use this command to display compilation details:
```
xmake -av
```
And their combinations:
```
xmake -arv
```
Get help please use:
```
xmake --help
```