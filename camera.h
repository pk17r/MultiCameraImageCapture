#include "uvc_cam.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>

namespace uvc_camera {

const int width = 1280;	//640, 1280
const int height = 960;	//480, 960
const std::string camImgPrefix1 = "cam1/";
const std::string camImgPrefix2 = "cam2/";
const std::string camImgPrefix3 = "cam3/";
const std::string camImgSuffix = ".png";
	
class Camera {
  public:
    Camera();
    void onInit();
    void feedImages();
    ~Camera();

  private:
    bool ok;
	int fps;
    std::string device, frame;
    
    boost::mutex time_mutex_;
    boost::thread image_thread;
    
    int counter;
    
    std::vector<int> compression_params;
    unsigned char image[height][width];
    cv::Mat image_mat_Bayer;
    cv::Mat image_mat_RGB;
    
    uvc_cam::Cam *cam;
};

};

