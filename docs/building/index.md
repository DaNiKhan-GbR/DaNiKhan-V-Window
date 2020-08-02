# Build instructions
[Back to home](../)

**Building is only required if you require a configuration not covered by the**
**automated builds, or if you want to contribute to the development of Dnkvw.**

## CMake

For building you must have at least CMake version 3.5 installed.

## OpenCV

The first step is to build and configure OpenCV 4.2.0. Newer versions might work, but were not tested.
It is generally not recommended to build Dnkvw with OpenCV as a shared dependency, since Dnkvw is easier to deploy with OpenCV included in the same .dll file. However, if you use Dnkvw in a context that
TODO OepnCV Not shared recommended -> shared possible unterschiede/vor nahctieel erklären

```console
dani@khan:~ git clone https://github.com/opencv/opencv.git
dani@khan:~ cd opencv
dani@khan:opencv$ opencv$ mkdir build
dani@khan:opencv/build$ cd build
dani@khan:opencv/build$ cmake .. -D BUILD_SHARED_LIBS=OFF -D CMAKE_INSTALL_PREFIX=./install TODO CONTRIB?
<CMAKE CONFIGURE OUTPUT HERE>
dani@khan:build$ make
<MAKE CONFIGURE OUTPUT HERE>

```


## Dnkvw

TODO OpenCV Install path Windows: `<INSTALL_DIR>/x64/vc16/lib`

Dnkvw can

```console
dani@khan:~$ git clone https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window.git dnkvw
dani@khan:~$ cd dnkvw
dani@khan:dnkvw$ mkdir build
dani@khan:build$ cd build
dani@khan:build$ cmake .. -D OpenCV_DIR=<OPENCV DIRECTORY>
<CMAKE CONFIGURE OUTPUT HERE>
dani@khan:build$ make
<MAKE CONFIGURE OUTPUT HERE>
```
