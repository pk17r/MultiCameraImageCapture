#include "uvc_cam.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <chrono>

namespace uvc_camera {

const int width = 1280;	//640, 1280, 1280
const int height = 720;	//480,  720,  960
const std::string textFilePrefix = "log";
const std::string camImgSuffix = ".png";
	
class Camera {
  public:
    Camera(std::string, bool, bool, bool, int, int, int, int, int, bool, bool, bool, bool);
    uvc_cam::Cam* setCamera(uvc_cam::Cam*, int, int, int);
    void onInit();
    void fetchImagesFunction(bool, bool, bool, bool, bool, bool, bool);
    void feedImages(bool, bool, bool, bool, bool, bool, bool);
    void getImgMatFromCamera(unsigned char*, unsigned char (*)[height][width]);
    ~Camera();
    
  private:
    bool ok;
	
    boost::mutex time_mutex_;
    boost::thread image_thread;
    
    unsigned int counter;
    std::chrono::system_clock::time_point t_base;
    
    std::vector<int> compression_params;
    
    uvc_cam::Cam *cam1, *cam2, *cam3;
    std::string camImgPrefix1;
	std::string camImgPrefix2;
	std::string camImgPrefix3;
};

};
