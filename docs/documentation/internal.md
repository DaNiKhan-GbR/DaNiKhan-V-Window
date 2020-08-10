# Internal Documentation
[Back to Documentation](index.md)

This document describes the overall structure of the library from a contributors perspective.

## interface/dnkvw.h
Defines the Dnkvw C Interface. More info can be found [here](c-api.md).

## interface/dnkvwInterface.h
Defines the Dnkvw C++ Interface. More info can be found [here](cpp-api.md).

## dnkvw.cpp
Implements the public C Interface unsing the dnkvwContext. Contains mostly wrappers.
More info can be found [here](c-api.md).

## constants.hpp
Contains definitions of constants used throughout the library.

## dnkvwContext.h/.cpp
Implements the Dnkvw Interface. More info can be found [here](cpp-api.md).

## tracker.hpp
Defines the interface for various facial tracking implemenetations. 

## haarTracker.hpp/.cpp
Implements the tracker using [Haar Cascades](https://docs.opencv.org/trunk/db/d28/tutorial_cascade_classifier.html).

## dnnTracker.hpp/.cpp
Implements the tracker using [DNN](https://en.wikipedia.org/wiki/Deep_learning#Deep_neural_networks) tracking.

## thrdWinCalc.hpp/.cpp
These files contain the multithreaded virtual window calculations including running the tracking and the actual eye offset and frustum calculations.

## logger.hpp
Provides simple logging utilities.

## fpsTimer.hpp
Provides a simple frames per second timer.

## vec3.hpp
Provides an easy way to represent vectors and several functions to aid with vector calculations.
