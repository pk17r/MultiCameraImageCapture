#include "uvc_cam.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <chrono>

namespace uvc_camera {

const int width = 1280;	//640, 1280, 1280
const int height = 960;	//480,  760,  960
const std::string camImgPrefix1 = "cam1/";
const std::string camImgPrefix2 = "cam2/";
const std::string camImgPrefix3 = "cam3/";
const std::string camImgSuffix = ".png";
	
class Camera {
  public:
    Camera();
    uvc_cam::Cam* setCamera(uvc_cam::Cam*, std::string);
    void onInit();
    void feedImages();
    void getImgMatFromCamera(unsigned char*, unsigned char (*)[height][width]);
    ~Camera();

  private:
    bool ok;
	int fps;
    std::string device1, device2, device3, frame;
    
    boost::mutex time_mutex_;
    boost::thread image_thread;
    
    unsigned int counter;
    std::chrono::system_clock::time_point t_base;
    
    std::vector<int> compression_params;
    
    uvc_cam::Cam *cam1, *cam2, *cam3;
};

};

