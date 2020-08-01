# Technical Documentation
[Back to Documentation](index.md)

## Introduction
The goal of this project is to create a virtual window. This means that the application will recreate the 
illusion that the user is looking through a window rather than at the screen of a monitor. This illusion 
is largely created through the use of parallax scrolling.
To achieve this, the position of the user’s head is taken into consideration when calculating the viewing frustum. 

Since there exist prior software projects that achieved a similar goal, the first step of the project 
was to analyze those. 

* [Johnny Lee](https://www.youtube.com/watch?v=Jd3-eiid-Uw) the original inspiration for most of these projects
* [Off-axis projection in Unity](https://medium.com/@michel.brisis/off-axis-projection-in-unity-1572d826541e) shows off a great use case for virtual windows
* [Fabian S](https://www.youtube.com/watch?v=h9kPI7_vhAU) an older project using Haar Cascades
* [get2him](https://www.youtube.com/watch?v=91tYEgpmN4M) another example of an existing application
* [Alexis Girault](https://www.youtube.com/watch?v=SWt_y5uIEAo) yet another example of an existing application
* [ViewTracker](https://store.steampowered.com/app/929270/ViewTracker/) a headtracking application on Steam
* [FaceAPI](https://sourceforge.net/p/facetracknoir/wiki/faceAPI/) an old and proprietary face tracking API
* [Andysaia](http://www.andysaia.com/radicalpropositions/unity-3d-faceapi-love/) a Unityplugin using FaceAPI

## User requirements
An important factor in this project was the use of consumer grade hardware. This means that to run the 
resulting software, the user should only need a typical recent laptop or desktop computer with a decent 
webcam. While Windows was the platform used during testing, the resulting software should work on 
Linux and macOS systems as well.

## Analysis of Libraries to Assist Implementation 
Before the implementation of the tracking algorithms could begin, a technical environment had to be 
established.  In order to implement the planned project, we need libraries covering video capturing, 
image processing and face tracking. For compatibility and optimal ease of implementation all of those 
should ideally be contained in a single library. To find the best solution for this project, several 
options were considered and analyzed.

### SimpleCV
An open source framework that aims to make the creation of powerful computer vision software easy. 
Unfortunately, it only supports Python, which makes it difficult to provide the simple, mostly 
language independent interface we wanted to implement with our library.

### VXL
VXL is a collection of C++ libraries for computer vision. Its goal is stated to be the creation of a 
light, fast and consistent system. However, the documentation was found to be lacking and the activity 
of the project’s development seemed to have come to a halt in recent years.

### VLFeat
Seems to be more optimized toward the research of implementation for new computer vision algorithms 
rather than the application of already established techniques.

### SOD
A very impressive collection of APIs for deep learning and advanced media analysis that seems to work on 
many devices and works nicely together with OpenCV. The reason we decided against using SOD for this 
project however, were the unclear licensing terms. Especially since we wanted to provide a library that 
is easy to use, we did not want to burden the users of our library with having to scan for potential 
licensing issues, should they choose to use our library for commercial software.

### OpenCV
The popular choice whenever computer vision is discussed. Offers a great documentation and a large pool of 
third party resources online, thanks to its popularity. While a large part of its community is focused on its 
use cases in Python, the library itself is written in C++ and therefore offers a great and well 
documented interface for C++ software. 

On the downside the sheer size of OpenCV would make it difficult to include all of it in our library, while 
still maintaining a manageable file size. The most optimal way to work around this would be to compile 
only the required files ourselves. We decided to instead statically link against OpenCV. This yields a much larger file size, but makes the build and the deployment process much easier for the library user.

Due to the versatility of the library and the availability of good documentation we ultimately concluded 
that taking this approach would be the best way going forward.

## Comparison of different face tracking algorithms
A core requirement for our project is accurate, stable and fast face tracking.
This is needed to calculate the viewing frustum and eye offset. There are many ways to achieve this, 
but this project focused on methods that work on simple consumer grade hardware. 

We decided to compare Haar Cascaces and a face tracking DNN because they are booth
embedded in OpenCV, are easy to use and we found good documentation on them.

### Haar Cascades
Haar Cascades were introduced by Paul Viola and Michael Jones in their paper "Rapid Object Detection 
using a Boosted Cascade of Simple Features" in 2001 and remained the state of the art in face detection 
for many years. OpenCV also includes many models based on this concept, which makes this approach 
easy to implement.

Haar Cascades were the first model implemented to create a rough prototype of this project. As expected, 
they were quite easy to get to work with OpenCV, and they worked well enough for a first prototype. 
They come, however, with a few drawbacks concerning the tracking quality and reliability.

In our early tests we found it to sometimes produce false positives, identifying hands or other objects 
as faces. Other times it did not recognize faces that had even just a small part occluded. Even when 
there was only one object in frame, there would be a lot of jitter.

| **Pros** 	                    | **Cons**  	                                    |
|---	                        |---	                                            |
| Easy to implement  	        | Lots of false positives   	                    |
| Works real-time  	            | Jittering, unstable tracking 	                    |
| Correctly detects most faces  | Bad with occlusion  	                            |
|                               | Bad when the face is not recorded from the front  |
<br><br>

Overall, our observations were consistent with the conclusions that other teams had already made. 
[An article](https://www.learnopencv.com/face-detection-opencv-dlib-and-deep-learning-c-python/) on 
learnopencv.com provides a nice overview of the pros and cons of Haar Cascades.

### DNN
In more recent years Deep Neural Networks have superseded Haar Cascades as the go to method of easily 
implemented face detection. They enable a faster and more stable face detection with greater accuracy than 
Haar Cascades. Since OpenCV also includes already pretrained models for DNNs, they were ultimately the 
superior choice for this project.

| **Pros**  	                            | **Cons**  	                        |
|---	                                    |---	                                |
| Most accurate in our tests                | Works really slow in debug builds     | 
| Runs at real-time on CPU                  | Slightly slower than Haar Cascades    |
| Works for different face orientations     |                                       |
| Works even under substantial occlusion    |                                       |
| Detects faces across various distances    |                                       |
<br><br>

The DNN based detector overcomes all the drawbacks of Haar cascade based detector, without compromising 
on any benefit provided by Haar. We could not see any major drawback for this method except that it is 
slower than the Dlib HoG based Face Detector discussed next.

### Conclusion
While we decided to keep both face trackers in the library, 
the DNN tracker is greatly superior and should be preferred.

## Converting to Frustum
In order to correctly infer the head position from the tracking data ... TODO

The calculations to convert the head position to the corrected viewing frustum are based 
on the paper [Generalized Perspective Projection](https://csc.lsu.edu/~kooima/pdfs/gen-perspective.pdf) 
by Robert Kooima.

## Multithreading
Because our library is intened for use in 3D graphics software, we wanted to reduce the CPU
load of the main rendering thread, regardless of which rendering framework is used. The best
solution we found was to implement mutlithreading in our library. When the tracker is started a
new thread is created which runs continuously in the background. In this thread the camera image is
fetched and the required calculations are done. The user of the library can poll the most recent results
of these calculations at any time.

## Summary
DaNiKhan Virtual Window enables developers of 3D rendering applications of many kinds to easily 
implement a virtual window using a simple webcam for headtracking.
The API is kept as simple as possible for the developer to use and the requirements for the end user 
are kept very low.

Tracking quality is good but shows a noticeable lag. One approach to improve on this could be GPU based 
tracking, but this was not explored as a part of this project so far.
