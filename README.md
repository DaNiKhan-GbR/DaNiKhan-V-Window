# DaNiKhan V-Window
DaNiKhan Virtual Window: A virtual window just using your webcam.

TODO Screenshot

<p align="center">
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
  <a href="sScreenshots-demos">Screenshots & Demos</a> •
  <a href="#requirements">Requirements</a> •
  <a href="#installation">Installation</a> •
  <a href="#usage">Usage</a> •
  <a href="#building">Building</a> •
  <a href="#license">License</a>
</p>

## About

DaNiKhan Virtual Window is a library that allows the easy creation of virtual reality applications that don't require the user to own a headset or other specialized hardware. The resulting application takes the users head movement into account to create a depth effect. For this to work the user only needs a standard webcam.

## Motiviation

This project was initiated by the VR Project at the [FH Wedel](https://www.fh-wedel.de/).
In 2007 Johnny Lee [demonstrated](https://youtu.be/Jd3-eiid-Uw) a functional prototype for a virtual window using a head mounted Wii remote sensor bar to track the user's head position.

Since then there have been many attempts to recreate or improve upon his solution.

This project aims to create a library enable an easier way to implement a similar virtual window 
using a standard webcam.

## Screenshots & Demos

You can find some usable demos and screenshots [here](https://danikhan-gbr.github.io/DaNiKhan-V-Window/demos/).

## Requirements

You need:
* OpenCV >= 4.2.0
* Cmake >= 3.5
* Webcam with at least 640x480px @ 30 fps

Tested on:
* Windows 10

For instructions how to build OpenCV look here: TODO

## Installation

Precompiled binaries are available.
You can find our installation instructions [here](https://danikhan-gbr.github.io/DaNiKhan-V-Window/installation/).

## Usage

DaNiKhan V-Window offers a simple API to implement a virtual window using regular off the shelf consumer hardware.

TODO Codebeispiel

Logging defaults:
* Debug: Dnkvw = Verbose, Internal = Silent
* Release: Dnkvw = Warning, internal = Silent

## Building

You can find our build instructions [here](https://danikhan-gbr.github.io/DaNiKhan-V-Window/building/).

## License

This project is licensed under the BSD 3-Clause License.
See the [LICENSE](LICENSE) file for details.
