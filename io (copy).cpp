#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctype.h>
#include <stdio.h>

using namespace cv;
using namespace std;

string type2str(int type)
{
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

int main()
{
    Mat CameraFrame;
    Mat Grey;
    Mat Color;
 
    VideoCapture cap;
    char keypressed;
 
    //Opens the first imaging device.
    cap.open(0);
 
    //Check whether user selected camera is opened successfully.
    if( !cap.isOpened() )
    {
	cout << "***Could not initialize capturing...***\n";
	return -1;
    }

    //get the property of video capture
    int fps = cap.get(CV_CAP_PROP_FPS);
    int format = cap.get(CV_CAP_PROP_FORMAT);
    //int fps = cap.get(CV_CAP_PROP_FPS);
    printf("FPS: %d, Format:  %d \n", fps, format);
    
    
    
    //test code to know details of cameraFrame
    cap >> CameraFrame;
    string ty =  type2str( CameraFrame.type() );
    printf("Matrix: %s %dx%d \n", ty.c_str(), CameraFrame.cols, CameraFrame.rows);
 
    //Create a windows to display camera preview.
    namedWindow("RawBayer", CV_WINDOW_AUTOSIZE);
    namedWindow("Grayscale", CV_WINDOW_AUTOSIZE);
    namedWindow("Color", CV_WINDOW_AUTOSIZE);
 
    //Loop infinitely to fetch frame from camera and display it.
    for(;;)
    {
	int fps = cap.get(CV_CAP_PROP_FPS);
        int format = cap.get(CV_CAP_PROP_FORMAT);
        //int fps = cap.get(CV_CAP_PROP_FPS);
        printf("FPS: %d, Format:  %d \n", fps, format);
        
	//Fetch frame from camera. So simple, isn't;-)
	//cap >> CameraFrame;
	cap.read(CameraFrame);

	//Check whether received frame has valid pointer.
	if( CameraFrame.empty() )
	    break;
 
	cvtColor(CameraFrame, Grey, CV_RGB2GRAY); 
	cvtColor(CameraFrame, Color, CV_RGB2BGR);
 
	//Display the received frame
	imshow("RawBayer", CameraFrame);
	//Display the grey scale converted frame
	imshow("Grayscale", Grey);
	//Display the color scale converted frame
	imshow("Color", Color);
 
	//Wait for Escape keyevent to exit from loop
	keypressed = (char)waitKey(10);
	if( keypressed == 27 )
	    break;
    }
 
    //Release the camera interface.
    cap.release();
 
    return 0;
}