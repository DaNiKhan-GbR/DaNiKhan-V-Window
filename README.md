# DaNiKhan V-Window (Dnkvw)
DaNiKhan Virtual Window: A virtual window just using your webcam.

<p align="center">
    <img width=100% src="https://raw.githubusercontent.com/DaNiKhan-GbR/DaNiKhan-V-Window/master/docs/teaser.jpg">
</p>

<p align="center">
    <a href="https://danikhan-gbr.github.io/DaNiKhan-V-Window/">
        <img src="https://img.shields.io/badge/-Homepage-informational" alt="Homepage"/>
    </a>
    <a href="https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/actions">
        <img src="https://img.shields.io/github/workflow/status/DaNiKhan-GbR/DaNiKhan-V-Window/CD%20-%20Release" alt="Build"/>
    </a>
    <a href="https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/issues">
        <img src="https://img.shields.io/github/issues/DaNiKhan-GbR/DaNiKhan-V-Window" alt="Issues"/>
    </a>
    <a href="https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/pulls">
        <img src="https://img.shields.io/github/issues-pr/DaNiKhan-GbR/DaNiKhan-V-Window" alt="PRs"/>
    </a>
    <a href="https://github.com/DaNiKhan-GbR/DaNiKhan-V-Window/blob/master/LICENSE">
        <img src="https://img.shields.io/github/license/DaNiKhan-GbR/DaNiKhan-V-Window?color=blue" alt="License"/>
    </a>
</p>

<p align="center">
  <a href="#about">About</a> •
  <a href="#motiviation">Motiviation</a> •
  <a href="#usage">Usage</a> •
  <a href="#requirements">Requirements</a> •
  <a href="#links">Links</a> •
  <a href="#license">License</a>
</p>

## About

DaNiKhan Virtual Window (Dnkvw) is a library that allows the easy creation of virtual reality applications that don't require the user to own a headset or other specialized hardware. The resulting application takes the users head movement into account to create a depth effect. For this to work the user only needs a standard webcam.

This library exposes a [C](https://danikhan-gbr.github.io/DaNiKhan-V-Window/documentation/c-api.html) 
and a [C++](https://danikhan-gbr.github.io/DaNiKhan-V-Window/documentation/cpp-api.html) Interface.

## Motiviation

This project was initiated by the VR Project at the [FH Wedel](https://www.fh-wedel.de/).
In 2007 Johnny Lee [demonstrated](https://youtu.be/Jd3-eiid-Uw) a functional prototype for a virtual window using a head mounted Wii remote sensor bar to track the user's head position.

Since then there have been many attempts to recreate or improve upon his solution.

This project aims to create a library enable an easier way to implement a similar virtual window 
using a standard webcam.

## Usage

DaNiKhan V-Window offers a simple API to implement a virtual window using regular off the shelf consumer hardware.

Minimal C Usage:
```C
// Step 1: Include our library
#include <dnkvw/dnkvw.h>

// Step 2: create and configure a new dnkvw context
IDnkvwHandle dnkvw = dnkvw_createContext();
dnkvw_selectDnnTracker(dnkvw);
dnkvw_configureFrustum(dnkvw, windowWidth / (float)windowHeight, nearPlane);

// Step 3: start tracking
dnkvw_startTracking(dnkvw, cameraId);

// Step 4: read values
dnkvw_loadEyeOffset(dnkvw, &eyeOffset.x, &eyeOffset.y, &eyeOffset.z);
dnkvw_loadFrustum(dnkvw, &left, &right, &top, &bottom);

// Step 5: stop & cleanup
dnkvw_stopTracking(dnkvw);
dnkvw_freeContext(&dnkvw);
```

For more complete in-depth examples look at our [demos](https://danikhan-gbr.github.io/DaNiKhan-V-Window/demos/).

## Requirements

You need:
* OpenCV >= 4.2.0
* Cmake >= 3.5
* Webcam with at least 640x480px @ 30 fps

Tested on:
* Windows 10

## Links

* [Installation Instructions](https://danikhan-gbr.github.io/DaNiKhan-V-Window/installation/)
* [Documentation](https://danikhan-gbr.github.io/DaNiKhan-V-Window/documentation/)
* [Images & Demos](https://danikhan-gbr.github.io/DaNiKhan-V-Window/demos/)
* [Build Instructions](https://danikhan-gbr.github.io/DaNiKhan-V-Window/building/)

## License

This project is licensed under the BSD 3-Clause License.
See the [LICENSE](LICENSE) file for details.
