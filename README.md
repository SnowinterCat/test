# Test

## Introduction ([中文](/README_zh.md))

## How to build?
Test is built using xmake, for a related xmake tutorial check out [xmake-io](https://xmake.io/).

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