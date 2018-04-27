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
    
    uvc_camera::Camera camera;
	
	return 0;
	
}
