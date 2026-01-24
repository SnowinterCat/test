# Test

## Introduction ([中文](/README_zh.md))

## How to build?
Test is built using xmake, for a related xmake tutorial check out [xmake-io](https://xmake.io/).

### Before build

Some of the dependencies of this project are system libraries (e.g. libx11-dev) or third-party libraries that you need to install by yourself (e.g. vulkansdk, pkg-config). You need to install them before configuring with xmake.

Of course, if some of the 3rd-party libraries that can be installed from xmake witch you also want to use the ones from system, you can also use the xmake configuration after installing them.

1. Install vulkansdk
    ```
    https://vulkan.lunarg.com/sdk/home
    ```
   - For Windows, You need to download and install vulkansdk on this webside.
   - For Linux, You need to download the vulkansdk compressed package on this webside.
     - After downloading, extract the files to any directory and locate the setup-env.sh file in the vulkansdk directory.
     - Before the next step, run the following command to import the environment variables for vulkansdk. (you need to replace the path and the vulkansdk version number)
        ```
        . /pathto/vulkansdk/1.x.xxx.x/setup-env.sh
        ```
     - Run the following command to verify that the vulkansdk environment variable has been imported correctly.
        ```
        echo $VULKAN_SDK
        ```
2. Install pkg-config (Linux Only)
   - For apt, dnf and yum package management systems
        ```
        sudo apt install pkg-config
        sudo dnf install pkg-config
        sudo yum install pkg-config
        ```
   - For pacman package management systems
        ```
        sudo pacman -S pkgconf
        ```
3. Install header files (Linux distributions with header files and runtime libraries separated Only)

    All header files for libraries related to Wayland and X11 must be installed.

    Here is an example for ubuntu 24.04:
    ```
    sudo apt install libx11-dev libwayland-dev libxext-dev
    ```

### Configure

1. Windows
   - Use msvc
        ```
        xmake f -p windows -a x64 -m releasedbg -k shared --3rd_kind=shared --runtimes=MD -cv
        ```
   - Use clang-cl
        ```
        xmake f -p windows -a x64 -m releasedbg -k shared --3rd_kind=shared --runtimes=MD --toolchain=clang-cl -cv
        ```
   - Use mingw with libstdc++
        ```
        xmake f -p mingw -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared --mingw=/path_of_mingw -cv
        ```
   - Use mingw with stdc++
        ```
        xmake f -p mingw -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=c++_shared --mingw=/path_of_mingw -cv
        ```
2. Linux
   - Use gcc with libstdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared -cv
        ```
   - Use gcc-13 with libstdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared --toolchain=gcc-13 -cv
        ```
   - Use clang with libstdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=stdc++_shared --toolchain=clang -cv
        ```
   - Use clang with stdc++
        ```
        xmake f -p linux -a x86_64 -m releasedbg -k shared --3rd_kind=shared --runtimes=c++_shared --toolchain=clang -cv
        ```

### Compile

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