#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <sstream>

#include "camera.h"
#include <popt.h>

using namespace cv;
using namespace std;

#define allCameras false
#define displayImages false

int main(int argc, char *argv[])
{
	cout << "\n";
	cout << "*********************************************************" << endl;
	cout << "*                                                       *" << endl;
	cout << "*                    Welcome to                         *" << endl;
	cout << "*         Multibaseline Stereo Vision Project           *" << endl;
	cout << "*                by Prashant Kumar                      *" << endl;
	cout << "*                                                       *" << endl;
	cout << "*********************************************************" << endl;
	
	/*if(argc <= 10) {
		cerr << "\n\n**Correct Usage**\n\nsudo ./cv_io save_dir showCaptures useMAVLinkForTrigger useGPIOPinsAsTrigger cam_ID_lcr cameras_to_use resolution brightness exposure use_timestamp MAVLinkPort\n" << endl;
		cerr << "e.g.: sudo ./cv_io /mnt/ssd/ 0 0 0 123 lcr 2 9 100 0 /dev/ttyUSB0" << endl;
		cerr << "\nsave_dir = '' if cam* folders are in build directory or e.g. /mnt/ssd/ if somewhere else." << endl;
		cerr << "showCaptures useMAVLinkForTrigger useGPIOPinsAsTrigger are booleans 0 or 1." << endl;
		cerr << "cam_ID_lcr are 012 or 123, etc for left, center, right camera. use ls /dev/video* to check cam ids." << endl;
		cerr << "cameras_to_use is lcr for all cameras, lc for left and center, r for only right, etc." << endl;
		cerr << "resolution is 0-2 for 640x480/1280x720/1280x960." << endl;
		cerr << "brightness is 0-25." << endl;
		cerr << "exposure 0 = auto, 1-20000 is manual, e.g. 78." << endl;
		cerr << "use_timestamp is to save images with timestamp name or counter name." << endl;
		cerr << "MAVLinkPort is port name of MAVLink e.g. /dev/ttyUSB0 or /dev/ttyTHS02" << endl;
		cerr << "\n" << endl;
		return -1;
	}
	int i = 0;
	auto settings = uvc_camera::Settings();
	
	settings.save_directory = argv[++i];
	settings.showCaptures = (bool)atoi(argv[++i]);	//to display the captured images during runtime
	settings.useMAVLinkForTrigger = (bool)atoi(argv[++i]);	//use MAVLink GPS messages as trigger
	settings.useGPIOPinsAsTrigger = (bool)atoi(argv[++i]);	//to use TX2 GPIO pins to trigger cameras

	string cameras = argv[++i];	int j = 0;
	settings.cam_x_ID[j] = cameras[j] - '0'; j++;
	settings.cam_x_ID[j] = cameras[j] - '0'; j++;
	settings.cam_x_ID[j] = cameras[j] - '0'; j++;
	
	cameras = argv[++i]; j = 0;
	settings.use_cam_x[j] = cameras.find("l") != -1; j++;
	settings.use_cam_x[j] = cameras.find("c") != -1; j++;
	settings.use_cam_x[j] = cameras.find("r") != -1; j++;
	
	settings.resolution = atoi(argv[++i]);
	settings.brightness = atoi(argv[++i]);
	settings.exposure = atoi(argv[++i]);
	settings.use_timestamp = (bool)atoi(argv[++i]);
	settings.MAVLinkPort = argv[++i];
	*/
	uvc_camera::Settings settings;
	
	const string inputSettingsFile = argc > 1 ? argv[1] : "../settings.xml";
    FileStorage fs(inputSettingsFile, FileStorage::READ); // Read the settings
    if (!fs.isOpened())
    {
        cout << "Could not open the configuration file: \"" << inputSettingsFile << "\"" << endl;
        FileStorage fs(inputSettingsFile, FileStorage::WRITE);
		fs << "Settings" << settings;
		cout << "Created a configuration file with default values!" << endl;
		fs.release();	// close Settings file
        return -1;
    }
    fs["Settings"] >> settings;
    fs.release();	// close Settings file
    cout << "\nLoaded settings from configuration file: \"" << inputSettingsFile << "\"" << endl;
    
	cout <<"\n*Inputs*" << endl;
	cout <<"save_directory: \"" << settings.save_directory << "\"" << endl;
	cout <<"showCaptures: " << settings.showCaptures << endl;
	cout <<"useMAVLinkForTrigger: " << settings.useMAVLinkForTrigger << endl;
	cout <<"useGPIOPinsAsTrigger: " << settings.useGPIOPinsAsTrigger << endl;
	int j = 0;
	cout <<"cam_l_ID: " << settings.cam_x_ID[j++] << endl;
	cout <<"cam_c_ID: " << settings.cam_x_ID[j++] << endl;
	cout <<"cam_r_ID: " << settings.cam_x_ID[j++] << endl;
	j = 0;
	cout <<"use_cam_l: " << settings.use_cam_x[j++] << endl;
	cout <<"use_cam_c: " << settings.use_cam_x[j++] << endl;
	cout <<"use_cam_r: " << settings.use_cam_x[j++] << endl;
	
	cout <<"resolution: " << uvc_camera::Resolution[settings.resolution][0] << "x" << uvc_camera::Resolution[settings.resolution][1] << endl;
	cout <<"brightness: " << settings.brightness << endl;
	cout <<"exposure: " << settings.exposure << endl;
	cout <<"use_timestamp: " << settings.use_timestamp << endl;
	cout <<"MAVLinkPort: " << settings.MAVLinkPort << endl;
	
    cout<<"\nOpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;
    
	uvc_camera::Camera camera(settings);
	
	return 0;
	
}
