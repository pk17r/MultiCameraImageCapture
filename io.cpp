#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <sstream>

#include "camera.h"

using namespace cv;
using namespace std;

#define allCameras false
#define displayImages false


int main()
{
    cout<<"OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;
    
    cout<< "** Calling camera()" << endl;
    int a = 20;
    uvc_camera::Camera camera;
    cout << "a: " << a << endl;
	cout<< "** camera() Called." << endl;
	
	return 0;
	
}
