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
    void setCamera(uvc_cam::Cam*, std::string);
    void onInit();
    void feedImages();
    void cleanCameras(uvc_cam::Cam*, unsigned char*, uint32_t);
    void getImgMatFromCamera(unsigned char*, unsigned char (*)[height][width]);
    ~Camera();

  private:
    bool ok;
	int fps;
    std::string device1, device2, device3, frame;
    
    boost::mutex time_mutex_;
    boost::thread image_thread;
    
    unsigned int counter;
    
    std::vector<int> compression_params;
    unsigned char image1[height][width];
	unsigned char image2[height][width];
	unsigned char image3[height][width];
    cv::Mat image_mat_Bayer1;
    cv::Mat image_mat_Bayer2;
    cv::Mat image_mat_Bayer3;
    cv::Mat image_mat_RGB;
    
    uvc_cam::Cam *cam1, *cam2, *cam3;
};

};

