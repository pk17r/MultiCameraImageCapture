#include "uvc_cam.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>

namespace uvc_camera {

class Camera {
  public:
    Camera();
    void onInit();
    //void sendInfo(sensor_msgs::ImagePtr &image, ros::Time time);
    void feedImages();
    ~Camera();

    //void timeCb(std_msgs::Time time);

  private:
    bool ok;
	//static const int width = 640, height = 480;
	int width, height;
	int fps, skip_frames, frames_to_skip;
    std::string device, frame;
    bool rotate;
    
    //Time last_time;
    boost::mutex time_mutex_;

    boost::thread image_thread;
    
    int counter;
    const std::string camImgPrefix1 = "cam1/";
	const std::string camImgSuffixTxt = ".txt";
	const std::string camImgSuffix = ".png";
    std::string camImgPath1;
    std::vector<int> compression_params;
    unsigned char image[480][640];
    cv::Mat image_mat_Bayer;
    cv::Mat image_mat_BGR;
    
    uvc_cam::Cam *cam;
};

};

