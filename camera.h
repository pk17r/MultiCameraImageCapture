#include "uvc_cam.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <chrono>

using namespace std;

namespace uvc_camera {

class Settings {
	public:
	string save_directory;
	bool showCaptures;	//to display the captured images during runtime
	bool useMAVLinkForTrigger;	//use MAVLink GPS messages as trigger
	bool useGPIOPinsAsTrigger;	//to use TX2 GPIO pins to trigger cameras
	int cam_x_ID[3] = { 0, 1, 2 };
	bool use_cam_x[3] = { true, true, true };
	int brightness;
	int exposure;
	bool use_timestamp;
	int resolution;
};

const int Resolution[3][2] = { {640,480}, {1280,720}, {1280,960} };
const int width = 0;
const int height = 1;
static int width_sz = 640;
static int height_sz = 480;

//const int width = 1280;	//640, 1280, 1280
//const int height = 960;	//480,  720,  960
const string textFilePrefix = "log";
const string camImgSuffix = ".png";
	
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
    
    vector<int> compression_params;
    
    uvc_cam::Cam *cam[3];
    
    string camImgPrefix1;
	string camImgPrefix2;
	string camImgPrefix3;
};

};
