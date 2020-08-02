# Build instructions
[Back to home](../)

**Building manually is only required if you require a configuration not covered by the**
**automated builds, or if you want to contribute to the development of Dnkvw.**

We recommend to use the provided binaries in the latest 
[GitHub Release](https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/releases). 

## CMake

CMake is required for building OpenCV and Dnkvw.
You must have at least CMake version 3.5 installed.

## OpenCV

Before you can build Dnkvw you first need to build and configure OpenCV 4.2.0. 
Newer versions might work, but were not tested.  

Building Dnkvw with OpenCV as a shared dependency is generally not recommended, 
since Dnkvw is easier to deploy with OpenCV included in the same .dll file. However, 
if you use Dnkvw in a context that brings its own version of OpenCV, the embedded version 
of OpenCV can cause conflicts. In this case, compiling with OpenCV as a shared dependency 
can be advantageous.  
Using a shared dependency can also make sense if you are absolutely certain that a compatible version OpenCV will be installed on the target machine and want to save storage space.

```console
dani@khan:~$ git clone https://github.com/opencv/opencv.git
dani@khan:~$ cd opencv
dani@khan:opencv$ mkdir build
dani@khan:opencv$ cd build
dani@khan:opencv/build$ mkdir install
dani@khan:opencv/build$ cmake -D BUILD_SHARED_LIBS=OFF -D CMAKE_INSTALL_PREFIX=./install ..
<CMAKE CONFIGURE OUTPUT HERE>
dani@khan::opencv/build$ make
<MAKE COMPILE OUTPUT HERE>
dani@khan::opencv/build$ make install
<MAKE INSTALL OUTPUT HERE>
```

## Dnkvw

If you haven't installed OpenCV globally, you need to set the 
OpenCV_DIR option to the installation path.

```console
dani@khan:~$ git clone https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window.git dnkvw
dani@khan:~$ cd dnkvw
dani@khan:dnkvw$ mkdir build
dani@khan:dnkvw/build$ cd build
dani@khan:dnkvw/build$ cmake -D OpenCV_DIR=<OPENCV DIRECTORY> ..
<CMAKE CONFIGURE OUTPUT HERE>
dani@khan:dnkvw/build$ make
<MAKE COMPILE OUTPUT HERE>
# If you want to deploy it, also use this
dani@khan:dnkvw/build$ make install
<MAKE INSTALL OUTPUT HERE>
```
