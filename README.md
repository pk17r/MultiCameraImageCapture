# MultiBaselineStereo
Multi Baseline Stereo Program with 3 cameras

Author: Prashant Kumar


Code to control and capture externally trigger images from E-Con systems cameras.

File descriptions:

io.cpp: Main file

camera.cpp: Main file to initialize and control cameras, grab and save images.

camera.h: Header file which defines the Camera class.

uvc_cam.cpp: Main file which controls and implements camera functionalities.

uvc_cam.h: Header file which defines the Cam class.

xunit folder files: Implement functions like finding cameras, camera initialization, setting trigger mode.

Self Notes:
-Check video devices attached to computer using 'ls -ltrh /dev/video*'. Use the video devices which actually are the e-con cameras in the camera.cpp file.
-Set image save locations in camera.h file. Make sure the folders exist.
-Use sudo ./cv_io on laptop while just ./cv_io on TX2 to run application.

