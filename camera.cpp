#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "camera.h"
#include <iostream>
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
		high_resolution_clock::time_point t1, t2;
		t1 = high_resolution_clock::now();
		duration<double, std::milli> time_span;
		cv::Mat image_mat_Bayer(height,width,CV_8UC(1),*image_ptr);		//making an opencv Mat array
		cv::Mat image_mat_RGB;
		cv::cvtColor(image_mat_Bayer, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
		//saving image to disk
		cv::imwrite(camImgPrefix + to_string(counter_) + camImgSuffix, image_mat_RGB, compression_params);
		t2 = high_resolution_clock::now();
		time_span = t2 - t1;
		cout << camImgPrefix << counter_ << "_" << ceil(time_span.count()) << "ms " ;
	}
	
    void Camera::feedImages() {
		//INFO
		//&img_frame = unsigned char **frame		therefore *img_frame = unsigned char frame
		//bytes_used = uint32_t &bytes_used
		//*frame = (unsigned char *)buffer_mem_[buffer_.index];
			 
		cout << "In feedImages() "<<endl;
		unsigned char *img_frame = NULL;
		uint32_t bytes_used;
		int idx;
		
		high_resolution_clock::time_point t1, t2;
		duration<double, std::milli> time_span;
		
		//remove current image from cameras
		idx = cam1->grab(&img_frame, bytes_used);
		if (img_frame) cam1->release(idx);
		idx = cam2->grab(&img_frame, bytes_used);
		if (img_frame) cam2->release(idx);
		idx = cam3->grab(&img_frame, bytes_used);
		if (img_frame) cam3->release(idx);

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
		idx = cam1->grab(&img_frame, bytes_used);
		t1 = high_resolution_clock::now();
		if (img_frame) {
			unsigned char image1[height][width];
			memcpy( image1[0], img_frame, height*width * sizeof(unsigned char));
			cam1->release(idx);
			//cam2
			idx = cam2->grab(&img_frame, bytes_used);
			if (img_frame) {
				unsigned char image2[height][width];
				memcpy( image2[0], img_frame, height*width * sizeof(unsigned char));
				cam2->release(idx);
				//cam3
				idx = cam3->grab(&img_frame, bytes_used);
				if (img_frame) {
					unsigned char image3[height][width];
					memcpy( image3[0], img_frame, height*width * sizeof(unsigned char));
					cam3->release(idx);
					unsigned char (*img1)[height][width] = &image1;
					unsigned char (*img2)[height][width] = &image2;
					unsigned char (*img3)[height][width] = &image3;
					boost::thread thread_cam1(saveCapturedImage, camImgPrefix1, counter, img1, compression_params);
					boost::thread thread_cam2(saveCapturedImage, camImgPrefix2, counter, img2, compression_params);
					boost::thread thread_cam3(saveCapturedImage, camImgPrefix3, counter, img3, compression_params);
					t2 = high_resolution_clock::now();
					time_span = t2 - t1;
					cout << "\nSet " << counter<< " time " << ceil(time_span.count()) << "ms ";
					counter++;
				}
				else { cout << "Cam3 not_grabbed,_nothing_saved_and_counter_not_updated!"; }
			}
			else { cout << "Cam2 not_grabbed,_nothing_saved_and_counter_not_updated!"; }
		}
		else { cout << "Cam1 not_grabbed,_nothing_saved_and_counter_not_updated!"; }
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

