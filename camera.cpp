#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "camera.h"
#include <stdlib.h>     /* atoi */
#include <iostream>
#include <fstream>

using namespace std;

namespace uvc_camera {

Camera::Camera(){
	//cam(0);
	//a = 99;
	//cout << "a: " << a << endl;
      /* default config values */
      cout<<"here"<<endl;
      height = 480;
      width = 640;
      fps = 10;
      skip_frames = 0;
      frames_to_skip = 0;
      device = "/dev/video0";
      frame = "camera";
      rotate = false;
      counter = 0;
      compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
      compression_params.push_back(0);
      image[height][width] = {};
      cv::Mat image_mat_Bayer = cv::Mat(height,width,CV_8U);
      cv::Mat image_mat_BGR = cv::Mat(height,width,CV_8UC(3));

      /* initialize the cameras */
      cam = new uvc_cam::Cam(device.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
      //cam->set_motion_thresholds(100, -1);
      cam->set_control(0x009a0901, 1); // exposure, auto (0 = auto, 1 = manual)
      cam->set_control(0x00980900, 8); // brightness
      cam->set_control(0x9a0902, 78); // exposure time 15.6ms

      /* and turn on the streamer */
      ok = true;
      image_thread = boost::thread(boost::bind(&Camera::feedImages, this));
      
      Camera::feedImages();
    }

    void Camera::feedImages() {
      unsigned int pair_id = 0;
      char keypressed;
      while (ok) {
        unsigned char *img_frame = NULL;
        uint32_t bytes_used;

        int idx = cam->grab(&img_frame, bytes_used);
		//cv::namedWindow("Cam1", CV_WINDOW_AUTOSIZE);

          if (img_frame) {
             //INFO
             //&img_frame = unsigned char **frame		therefore *img_frame = unsigned char frame
             //bytes_used = uint32_t &bytes_used
             //*frame = (unsigned char *)buffer_mem_[buffer_.index];
             
             //*image_linear = (uint8_t)atoi((const char*)img_frame);
             cout << "**** bytes_used: " << bytes_used << endl;
             cout << "**** First 10 pixel readings: ";
             for (int i = 0; i < 10; i++)
			 {
				 printf("%u ", img_frame[i]);
			 }
			 printf("\n");
             *image[0]=*img_frame;
             //cv::Mat aa = cv::Mat(image);
             //cv::imshow("Cam1", aa);
             cout << "% Counter: " << counter << endl;
             //ofstream myfile;
			 //camImgPath1 = camImgPrefix1 + to_string(counter) + camImgSuffixTxt;
             //myfile.open (camImgPath1);
             for (int i = 0; i < height; i++)
			 {
				 for (int j = 0; j < width; j++)
				 {
					 image[i][j]=img_frame[i*width + j];
					 //image_mat_Bayer.data[i*width + j]=img_frame[i*width + j];
					 //myfile<< (int)img_frame[i*width + j] << " ";
				 }
				 //myfile<<"\n";
			 }
			 //myfile.close();
			 //cout << "*** A ****" << endl;
			 cv::Mat image_mat_Bayer(height,width,CV_8UC(1),image);
			 //cout << "*** B ****" << endl;
             cv::cvtColor(image_mat_Bayer, image_mat_BGR, CV_BayerRG2RGB);	//CV_BayerRG2RGB
             //cout << "*** C ****" << endl;
             camImgPath1 = camImgPrefix1 + to_string(counter) + camImgSuffix;
             cv::imwrite(camImgPath1, image_mat_BGR, compression_params);
             cout<< "Saved to " << camImgPath1 << endl;
             counter++;
             
             ++pair_id;
             
             cam->release(idx);
          }


        //if (img_frame) cam->release(idx);
        
        //Wait for Escape keyevent to exit from loop
		keypressed = (char)cv::waitKey(10);
		if( keypressed == 27 )
			ok = false;
      }
    }

    Camera::~Camera() {
      ok = false;
      image_thread.join();
      if (cam) delete cam;
    }


};

