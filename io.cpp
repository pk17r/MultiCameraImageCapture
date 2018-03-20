#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <sstream>

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

string IntToString(int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}


int main()
{
    Mat CameraFrame1, CameraFrame2, CameraFrame3;
    Mat Grey1, Grey2, Grey3;
    Mat* GreyPtr1 = &Grey1;
    Mat* GreyPtr2 = &Grey2;
    Mat* GreyPtr3 = &Grey3;
 
    VideoCapture cap1;
    VideoCapture cap2;
    VideoCapture cap3;
    char keypressed;

    cout<<"OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;
 
    //Opens the first imaging device.
    cap1.open(1);
    cap2.open(2);
    cap3.open(3);
 
    //Check whether user selected camera is opened successfully.
    /*if(!cap1.isOpened() || !cap2.isOpened() || !cap3.isOpened())
    {
	cout << "***Could not initialize capturing...***\n";
	if(!cap1.isOpened())
		cout << "***Cam1 not initialized...***\n";
	if(!cap2.isOpened())
		cout << "***Cam2 not initialized...***\n";
	if(!cap3.isOpened())
		cout << "***Cam3 not initialized...***\n";
	return -1;
    }
    else
	cout << "All cameras initialized.\n";*/

    //get the property of video capture
    int fps = cap1.get(CV_CAP_PROP_FPS);
    int format = cap1.get(CV_CAP_PROP_FORMAT);
    printf("Cam1: FPS: %d, Format:  %d \n", fps, format);
    
    cap1.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    cap1.set(CV_CAP_PROP_FRAME_HEIGHT,960);
    cap2.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    cap2.set(CV_CAP_PROP_FRAME_HEIGHT,960);
    cap3.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    cap3.set(CV_CAP_PROP_FRAME_HEIGHT,960);

    //test code to know details of cameraFrame
    cap1 >> CameraFrame1;
    cap2 >> CameraFrame2;
    cap3 >> CameraFrame3;
    string ty1 =  type2str( CameraFrame1.type() );
    string ty2 =  type2str( CameraFrame2.type() );
    string ty3 =  type2str( CameraFrame3.type() );
    printf("Matrix1: %s %dx%d \n", ty1.c_str(), CameraFrame1.cols, CameraFrame1.rows);
    printf("Matrix2: %s %dx%d \n", ty2.c_str(), CameraFrame2.cols, CameraFrame2.rows);
    printf("Matrix3: %s %dx%d \n", ty3.c_str(), CameraFrame3.cols, CameraFrame3.rows);
 
    //Create a windows to display camera preview.
    //namedWindow("RawBayer", CV_WINDOW_AUTOSIZE);
    namedWindow("Cam1", CV_WINDOW_AUTOSIZE);
    namedWindow("Cam2", CV_WINDOW_AUTOSIZE);
    namedWindow("Cam3", CV_WINDOW_AUTOSIZE);

    time_t nowTime, lastTime;
    double timeGap;
    time(&lastTime);
    double reqdTimeGap = 0; //seconds
    int counter = 1;
    string camImgPrefix1 = "cam1/";
    string camImgPrefix2 = "cam2/";
    string camImgPrefix3 = "cam3/";
    string camImgSuffix = ".png";
    string camImgPath1, camImgPath2, camImgPath3;

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);

    //Loop infinitely to fetch frame from camera and display it.
    for(;;)
    {
	time(&nowTime);
	timeGap = difftime(nowTime,lastTime);
	if(timeGap >= reqdTimeGap)
	{
		lastTime = nowTime;
		//int fps = cap.get(CV_CAP_PROP_FPS);
		//int format = cap.get(CV_CAP_PROP_FORMAT);
		//printf("FPS: %d, Format:  %d \n", fps, format);
		
		//Fetch frame from camera. So simple, isn't;-)
		cap1 >> CameraFrame1;
		cap2 >> CameraFrame2;
		cap3 >> CameraFrame3;
		//cap1.read(CameraFrame1);
		//cap2.read(CameraFrame2);

		//Check whether received frame has valid pointer.
		if( CameraFrame1.empty() || CameraFrame2.empty() || CameraFrame3.empty() )
		    break;
	 
		cvtColor(CameraFrame1, Grey1, CV_RGB2GRAY); 
		cvtColor(CameraFrame2, Grey2, CV_RGB2GRAY);
		cvtColor(CameraFrame3, Grey3, CV_RGB2GRAY);
		//imdecode(CameraFrame1, CV_LOAD_IMAGE_COLOR, GreyPtr1);
		//imdecode(CameraFrame2, CV_LOAD_IMAGE_COLOR, GreyPtr2);
	 
		//Display the received frame
		//imshow("RawBayer", CameraFrame);
		//Display the grey scale converted frame
		imshow("Cam1", Grey1);
		imshow("Cam2", Grey2);
		imshow("Cam3", Grey3);

		if(false)	//to save images
		{
			camImgPath1 = camImgPrefix1 + IntToString(counter) + camImgSuffix;
			camImgPath2 = camImgPrefix2 + IntToString(counter) + camImgSuffix;
			camImgPath3 = camImgPrefix3 + IntToString(counter) + camImgSuffix;

			imwrite(camImgPath1, Grey1, compression_params);
			imwrite(camImgPath2, Grey2, compression_params);
			imwrite(camImgPath3, Grey3, compression_params);
			cout<< "Saved to " << camImgPath1 << ", " << camImgPath2 << ", " << camImgPath3 << endl;

			counter++;
		}
		
	}

	//Wait for Escape keyevent to exit from loop
	keypressed = (char)waitKey(10);
	if( keypressed == 27 )
	    break;
    }
 
    //Release the camera interface.
    cap1.release();
    cap2.release();
    cap3.release();
 
    return 0;
}
