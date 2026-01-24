# Test

## 简介 ([English](/README.md))

## 如何构建?
Test 使用 xmake 构建，相关 xmake 教程请查看 [xmake-io](https://xmake.io/).

### Before build

本项目的某些依赖库是系统库（如 libx11-dev）或需要自行安装的第三方库（如 vulkansdk、pkg-config）。在使用 xmake 配置之前，您需要先安装它们。

当然，如果某些可以通过 xmake 安装的第三方库你也想使用系统中的库，也可以在安装后使用 xmake 配置。

1. 安装 vulkansdk 开发包
    ```
    https://vulkan.lunarg.com/sdk/home
    ```
   - 对于 Windows 你需要到以上网站下载并安装 vulkansdk
   - 对于 Linux 你需要到以上网站下载 vulkansdk 的压缩包
     - 下载完成后解压到任意路径，并找到 vulkansdk 内部的 setup-env.sh 文件
     - 在进行下一步之前运行以下命令导入 vulkansdk 的环境变量(你需要替换路径，以及 vulkansdk 的版本号)
        ```
        . /pathto/vulkansdk/1.x.xxx.x/setup-env.sh
        ```
     - 运行以下命令检查 vulkansdk 的环境变量是否被正确导入
        ```
        echo $VULKAN_SDK
        ```
2. 安装 pkg-config (Linux Only)
   - 对于 apt, dnf, yum 系列包管理系统
        ```
        sudo apt install pkg-config
        sudo dnf install pkg-config
        sudo yum install pkg-config
        ```
   - 对于 pacman 系列包管理系统
        ```
        sudo pacman -S pkgconf
        ```
3. 安装头文件 (头文件与运行库分离的 Linux 发行版 Only)

    需要安装 wayland 与 X11 相关的所有库的头文件

    这里以 ubuntu 24.04 为例：
    ```
    sudo apt install libx11-dev libwayland-dev libxext-dev
    ```

### 配置

在构建之前，您需要对项目进行配置。

1. Windows
   - 使用 msvc
        ```
        xmake f -p windows -a x64 -m releasedbg -k shared --3rd_kind=shared --runtimes=MD -cv
        ```
   - 使用 clang-cl
        ```
        xmake f -p windows -a x64 -m releasedbg -k shared --3rd_kind=shared --runtimes=MD --toolchain=clang-cl -cv
        ```
   - 使用 mingw 与 libstdc++
        ```
        xmake f -p mingw -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared --mingw=/path_of_mingw -cv
        ```
   - 使用 mingw 与 stdc++
        ```
        xmake f -p mingw -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=c++_shared --mingw=/path_of_mingw -cv
        ```
2. Linux
   - 使用 gcc 与 libstdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared -cv
        ```
   - 使用 gcc-13 与 libstdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared --toolchain=gcc-13 -cv
        ```
   - 使用 clang 与 libstdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared --toolchain=clang -cv
        ```
   - 使用 clang 与 stdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=c++_shared --toolchain=clang -cv
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