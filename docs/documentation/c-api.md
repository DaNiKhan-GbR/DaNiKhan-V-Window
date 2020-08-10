# C API Documentation
[Back to Documentation](index.md)

This is the API Documentation for the C Interface of dnkvw.

<hr>

```C
IDnkvwHandle dnkvw_createContext();
```
This method creates a new dnkvw context. 
It is needed for all operations in this library. 
Do not forget to free it after you are done with it.

**Returns:**
* the newly created context

<hr>

```C
void dnkvw_freeContext(IDnkvwHandle* context);
```
This method deinitializes and frees a dnkw context.
If you use C NEVER use 'free()' to free an existing context.
Only use this method to free a context.

**Parameters:**
* `context` a pointer to an initialized context

<hr>

```C
void dnkvw_setLogLevel(IDnkvwHandle context, int logLevel);
```
Set the log level.

**Parameters:**
* `context` an initialized dnkvw context
* `logLevel` the log level

**Logging defaults:**
* Debug: Dnkvw = Verbose
* Release: Dnkvw = Warning

<hr>

```C
void dnkvw_setInternalLogLevel(IDnkvwHandle context, int internalLogLevel);
```
Set the internal log level.
Not intended for production use.
 
**Parameters:**
* `context` an initialized dnkvw context
* `internalLogLevel` the log level

**Logging defaults:**
* Debug: Internal = Silent
* Release: Internal = Silent

<hr>

```C
int dnkvw_selectHaarTracker(IDnkvwHandle context);
```
Use Haar tracking. 
Overwrites previously selected tracker.

**Parameters:**
* `context` an initialized dnkvw context

**Returns:**
* `1` if the selection was successful, `0` if not

<hr>

```C
int dnkvw_selectDnnTracker(IDnkvwHandle context);
```
Use DNN tracking. 
Overwrites previously selected tracker.

**Parameters:**
* `context` an initialized dnkvw context

**Returns:**
* `1` if the selection was successful, `0` if not

<hr>

```C
int dnkvw_startTracking(IDnkvwHandle context, int cameraId);
```
Starts the tracking.

**Parameters:**
* `context` an initialized dnkvw context
* `cameraId` the Id of the camera to be uses for tracking

**Returns:**
* `1` if the tracking could be started successfully. 
  A `0` value may indicate a problem with the camera.

<hr>

```C
void dnkvw_stopTracking(IDnkvwHandle context);
```
Stops the tracking.
This function may block.

**Parameters:**
* `context` an initialized dnkvw context

<hr>

```C
void dnkvw_stopTrackingAsync(IDnkvwHandle context);
```
Stops the tracking as soon as possible without blocking.

**Parameters:**
* `context` an initialized dnkvw context

<hr>

```C
void dnkvw_configureFrustum(IDnkvwHandle context, float aspectRatio, float nearPlane);
```
Configures the viewing frustum according to the parameters.

**Parameters:**
* `context` an initialized dnkvw context
* `aspectRatio` the aspect ratio
* `nearPlane` the near plane

<hr>

```C
void dnkvw_calibrate(IDnkvwHandle context);
```
Runs a short calibration routine.
The user should sit or stand as centered as possible before this
function is called. The position of the user will be used as the new
center until a new calibration is started.

This method should be called shortly after the start of the tracking 
to correct perspective problems caused by a wrong starting center.

**Parameters:**
* `context` an initialized dnkvw context

<hr>

```C
void dnkvw_loadFrustum(IDnkvwHandle context, float* left, float* right, float* top, float* bottom);
```
Get the last frustum.
The viewing frustum is the volume that may appear on the screen.
It is described by a near plane, a far plane and the distances of the
left, right, top and bottom plane based on the near plane.
This function returns only the left, right, top and bottom values.
The near and far plane can be set independently.

These values can be used to calculate the projection matrix. This is normally
done by your 3D graphics framework. Look at our demos for some case specific
examples.

For more information see 
[https://en.wikipedia.org/wiki/Viewing_frustum](https://en.wikipedia.org/wiki/Viewing_frustum)

**Parameters:**
* `context` an initialized dnkvw context
* `left` a pointer to a float where the left result can be saved
* `right` a pointer to a float where the right result can be saved
* `top` a pointer to a float where the top result can be saved
* `bottom` a pointer to a float where the bottom result can be saved

<hr>

```C
void dnkvw_loadEyeOffset(IDnkvwHandle context, float* x, float* y, float* z);
```
Get the last calculated eye offset.

**Parameters:**
* `context` an initialized dnkvw context
* `x` a pointer to a float where the X result can be saved
* `y` a pointer to a float where the Y result can be saved
* `z` a pointer to a float where the Z result can be saved

<hr>

```C
void dnkvw_loadFps(IDnkvwHandle context, float* fps);
```
Get the last FPS value.

**Parameters:**
* `context` an initialized dnkvw context
* `fps` a pointer to a float where the result can be saved

<hr>

```C
void dnkvw_debugCameraInput(IDnkvwHandle context, int cameraId);
```
Debugging function to test the camera input. 
Should not be called while the tracking thread is running.
This function may block.
Don't use this in production code.

**Parameters:**
* `context` an initialized dnkvw context
* `cameraId` Id of the camera to use for tracking

<hr>

```C
void dnkvw_debugCameraFace(IDnkvwHandle context, int cameraId);
```
Debugging function to test the face detection. 
Should not be called while the tracking thread is running.
This function may block.  
Don't use this in production code.

**Parameters:**
* `context` an initialized dnkvw context
* `cameraId` Id of the camera to use for tracking
