#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "camera.h"
//#include <stdlib.h>     /* atoi */
#include <iostream>
#include <fstream>
#include <unistd.h>		//for usleep
#include <chrono>
#include <math.h>

using namespace std;
using namespace std::chrono;

namespace uvc_camera {
	
	Camera::Camera(){
		/* default config values */
		cout<<"here"<<endl;
		fps = 10;
		device1 = "/dev/video0";
		device2 = "/dev/video1";
		device3 = "/dev/video2";
		frame = "camera";
		//rotate = false;
		counter = 0;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(0);
		image1[height][width] = {};
		image2[height][width] = {};
		image3[height][width] = {};

		/* initialize the cameras */
		cam1 = Camera::setCamera(cam1, device1);
		cam2 = Camera::setCamera(cam2, device2);
		cam3 = Camera::setCamera(cam3, device3);

		/* and turn on the streamer */
		ok = true;
		//image_thread = boost::thread(boost::bind(&Camera::feedImages, this));
		cout << "Call feedImages()" << endl;
		Camera::feedImages();
    }
    
    uvc_cam::Cam* Camera::setCamera(uvc_cam::Cam *cam, string device){
		/* initialize the cameras */
		cam = new uvc_cam::Cam(device.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
		//cam1->set_motion_thresholds(100, -1);
		cam->set_control(0x009a0901, 1); // exposure, auto (0 = auto, 1 = manual)
		cam->set_control(0x00980900, 8); // brightness
		cam->set_control(0x9a0902, 78); // exposure time 15.6ms
		return cam;
	}
    
	void saveCapturedImage(string camImgPrefix, int counter_, unsigned char (*image_ptr)[height][width], std::vector<int> compression_params) {
		cv::Mat image_mat_Bayer(height,width,CV_8UC(1),*image_ptr);
		cv::Mat image_mat_RGB;
		cv::cvtColor(image_mat_Bayer, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB
		cv::imwrite(camImgPrefix + to_string(counter_) + camImgSuffix, image_mat_RGB, compression_params);
		cout << camImgPrefix << counter_ << " ";
	}
	
    void Camera::feedImages() {
		//INFO
		//&img_frame = unsigned char **frame		therefore *img_frame = unsigned char frame
		//bytes_used = uint32_t &bytes_used
		//*frame = (unsigned char *)buffer_mem_[buffer_.index];
			 
		cout << "In feedImages() "<<endl;
		unsigned char *img_frame1 = NULL;
		unsigned char *img_frame2 = NULL;
		unsigned char *img_frame3 = NULL;
		uint32_t bytes_used1, bytes_used2, bytes_used3;
		int idx1, idx2, idx3;
		unsigned char (*img1)[height][width] = &image1;
		unsigned char (*img2)[height][width] = &image2;
		unsigned char (*img3)[height][width] = &image3;
		high_resolution_clock::time_point t1, t2;
		duration<double, std::milli> time_span;
		t1 = high_resolution_clock::now();
		idx1 = cam1->grab(&img_frame1, bytes_used1);
		if (img_frame1) cam1->release(idx1);
		cout << "*** Cleaned Cam1 ****" << endl;
		idx2 = cam2->grab(&img_frame2, bytes_used2);
		if (img_frame2) cam2->release(idx2);
		cout << "*** Cleaned Cam2 ****" << endl;
		idx3 = cam3->grab(&img_frame3, bytes_used3);
		if (img_frame3) cam3->release(idx3);
		cout << "*** Cleaned Cam3 ****" << endl;
		t2 = high_resolution_clock::now();
		time_span = t2 - t1;
		cout<< "Time to call clean camera functions : " << time_span.count() << "ms" << endl;

		cout<< "Capturing start!" << endl;

		int time_diff;

		while (ok) {
		//Algo!!
		//if image in cam1 -> get image, release camera
		//wait for image in cam2 -> get image, release camera
		//wait for image in cam3 -> get image, release camera
		//note time
		//save images in multi-threading and update counter else clean all cameras

		//cam1
		idx1 = cam1->grab(&img_frame1, bytes_used1);
		if (img_frame1) {
			t1 = high_resolution_clock::now();
			memcpy( image1[0], img_frame1, height*width * sizeof(unsigned char));
			cam1->release(idx1);
			//cam2
			idx2 = cam2->grab(&img_frame2, bytes_used2);
			if (img_frame2) {
				memcpy( image2[0], img_frame2, height*width * sizeof(unsigned char));
				cam2->release(idx2);
				//cam3
				idx3 = cam3->grab(&img_frame3, bytes_used3);
				if (img_frame3) {
					memcpy( image3[0], img_frame3, height*width * sizeof(unsigned char));
					cam3->release(idx3);
					boost::thread thread_cam1(saveCapturedImage, camImgPrefix1, counter, img1, compression_params);
					boost::thread thread_cam2(saveCapturedImage, camImgPrefix2, counter, img2, compression_params);
					boost::thread thread_cam3(saveCapturedImage, camImgPrefix3, counter, img3, compression_params);
					t2 = high_resolution_clock::now();
					time_span = t2 - t1;
					cout << "\nSet " << counter<< " time " << ceil(time_span.count()) << "ms ";
					counter++;
				}
				else { cout << " CATCH_ME Cam3 not fetched!" << endl; }
			}
			else { cout << " CATCH_ME Cam2 not fetched!" << endl; }
		}
		else { cout << " CATCH_ME Cam1 not fetched!" << endl; }
		}
    }

    Camera::~Camera() {
		ok = false;
		image_thread.join();
		if (cam1) delete cam1;
		if (cam2) delete cam2;
		if (cam3) delete cam3;
    }


};

