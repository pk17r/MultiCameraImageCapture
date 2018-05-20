#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "uvc_cam.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <chrono>

using namespace std;
using namespace cv;

namespace uvc_camera {

class Settings {
  public:
	void write(FileStorage& fs) const                        //Write serialization for this class
    {
		fs << "{"
			<< "save_directory"  << save_directory
			<< "showCaptures" << showCaptures
			<< "useMAVLinkForTrigger" << useMAVLinkForTrigger
			<< "useGPIOPinsAsTrigger" << useGPIOPinsAsTrigger
			<< "cam_l_ID" << cam_x_ID[0]
			<< "cam_c_ID" << cam_x_ID[1]
			<< "cam_r_ID" << cam_x_ID[2]
			<< "use_cam_l" << use_cam_x[0]
			<< "use_cam_c" << use_cam_x[1]
			<< "use_cam_r" << use_cam_x[2]
			<< "brightness" << brightness
			<< "exposure" << exposure
			<< "use_timestamp" << use_timestamp
			<< "resolution"   << resolution
			<< "MAVLinkPort"  << MAVLinkPort
			<< "baudrate"  << baudrate
			<< "counter"  << counter

		<< "}";
    }
	void read(const FileNode& node)                          //Read serialization for this class
	{
		node["save_directory" ] >> save_directory;
		node["showCaptures"] >> showCaptures;
		node["useMAVLinkForTrigger"] >> useMAVLinkForTrigger;
		node["useGPIOPinsAsTrigger"]  >> useGPIOPinsAsTrigger;
		node["cam_l_ID"] >> cam_x_ID[0];
		node["cam_c_ID"] >> cam_x_ID[1];
		node["cam_r_ID"] >> cam_x_ID[2];
		node["use_cam_l"] >> use_cam_x[0];
		node["use_cam_c"] >> use_cam_x[1];
		node["use_cam_r"] >> use_cam_x[2];
		node["brightness"] >> brightness;
		node["exposure"] >> exposure;
		node["use_timestamp"] >> use_timestamp;
		node["resolution"] >> resolution;
		node["MAVLinkPort"] >> MAVLinkPort;
		node["baudrate"] >> baudrate;
		node["counter"] >> counter;
		
	}
  
	string save_directory = "";
	bool showCaptures = false;	//to display the captured images during runtime
	bool useMAVLinkForTrigger = false;	//use MAVLink GPS messages as trigger
	bool useGPIOPinsAsTrigger = false;	//to use TX2 GPIO pins to trigger cameras
	int cam_x_ID[3] = { 0, 1, 2 };
	bool use_cam_x[3] = { true, true, true };
	int brightness = 9;
	int exposure = 78;
	bool use_timestamp = true;
	int resolution = 2;
	string MAVLinkPort = "/dev/ttyUSB0";
	int baudrate = 115200;
	int counter = 0;
	
};
static inline void read(const FileNode& node, Settings& x, const Settings& default_value = Settings()) {
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}
static inline void write(FileStorage& fs, const String&, const Settings& s ) {
    s.write(fs);
}
const int Resolution[3][2] = { {640,480}, {1280,720}, {1280,960} };
const int width = 0;
const int height = 1;

class Camera {
  public:
    Camera(Settings);
    void setCamera(int, Settings);
    void onInit();
    void fetchImagesFunction(Settings);
    void feedImages(Settings);
    void getImgMatFromCamera(unsigned char*, unsigned char (*)[height][width]);
    ~Camera();
    
  private:
    bool ok;
	
    boost::mutex time_mutex_;
    boost::thread image_thread;
    
    unsigned int counter;
    chrono::system_clock::time_point t_base;
    
    uvc_cam::Cam *cam[3];
};

};
