#include <iostream>
#include <ctype.h>
#include <stdio.h>

#include "camera.h"

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
	
	cout << "\nUsage: sudo ./cv_io [Notes_for_log(Dont_use_commas_or_\n)]" << endl;
	
	uvc_camera::Settings settings;
	const string inputSettingsFile = "../settings.xml";
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
    
    settings.notes_for_log = argc > 1 ? argv[1] : ""; //record notes for this run to save to log
    
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
	cout <<"baudrate: " << settings.baudrate << endl;
	cout <<"counter: " << settings.counter << endl;
	cout <<"notes_for_log: " << settings.notes_for_log << endl;
	
    cout<<"\nOpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;
    
	uvc_camera::Camera camera(settings);
	
	return 0;
	
}
