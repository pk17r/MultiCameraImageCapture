#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "camera.h"
#include <iostream>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <string.h>

using namespace std;
using namespace std::chrono;

namespace uvc_camera {
	
	Camera::Camera(){
		/* default config values */
		fps = 10;
		device1 = "/dev/video1";
		device2 = "/dev/video2";
		device3 = "/dev/video3";
		counter = 0;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(0);

		/* initialize the cameras */
		cam1 = Camera::setCamera(cam1, device1);
		cam2 = Camera::setCamera(cam2, device2);
		cam3 = Camera::setCamera(cam3, device3);
		
		cout << "\nAll Cameras Initialized!\n"<<endl;

		/* and turn on the streamer */
		ok = true;
		//image_thread = boost::thread(boost::bind(&Camera::feedImages, this));
		
		//base time = create tm with 4/1/2018:
		std::tm timeinfo = std::tm();
		timeinfo.tm_year = 2018 - 1900;   // year: 2000
		timeinfo.tm_mon = 3;      // month: apr
		timeinfo.tm_mday = 1;     // day: 1st
		std::time_t tt = std::mktime (&timeinfo);
		t_base = system_clock::from_time_t (tt);
		
		Camera::feedImages();
    }
        
    uvc_cam::Cam* Camera::setCamera(uvc_cam::Cam *cam, string device){
		/* initialize the cameras */
		cam = new uvc_cam::Cam(device.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
		//cam1->set_motion_thresholds(100, -1);
		cam->set_control(0x009a0901, 0); // exposure, auto (0 = auto, 1 = manual)
		cam->set_control(0x00980900, 9); // brightness
		//cam->set_control(0x9a0902, 78); // exposure time 15.6ms
		//usleep(300000);
		return cam;
	}
    
	void saveCapturedImage(string camImgPrefix, int counter_, uint64_t time_from_base, unsigned char (*image_ptr)[height][width], std::vector<int> compression_params) {
		std::chrono::high_resolution_clock::time_point t1, t2;
		t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time_span;
		cv::Mat image_mat_Bayer(height,width,CV_8UC(1),*image_ptr);		//making an opencv Mat array
		cv::Mat image_mat_RGB;
		cv::cvtColor(image_mat_Bayer, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
		//saving image to disk
		cv::imwrite(camImgPrefix + to_string(counter_) + camImgSuffix, image_mat_RGB, compression_params);
		t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		cout << counter_ << "_" << ceil(time_span.count()) << "ms " ;
	}
	
    void Camera::feedImages() {
		//INFO
		//&img_frame = unsigned char **frame		therefore *img_frame = unsigned char frame
		//bytes_used = uint32_t &bytes_used
		//*frame = (unsigned char *)buffer_mem_[buffer_.index];
			 
		unsigned char *img_frame = NULL;
		uint32_t bytes_used;
		int idx;
		
		string window1 = "Cam1";
		string window2 = "Cam2";
		string window3 = "Cam3";
		
		bool showCaptures = true;	//to display the captured images during runtime
		
		if(showCaptures) {
			cv::namedWindow(window1, CV_WINDOW_AUTOSIZE);
			cv::namedWindow(window2, CV_WINDOW_AUTOSIZE);
			cv::namedWindow(window3, CV_WINDOW_AUTOSIZE);
		}
		
		high_resolution_clock::time_point t1, t2, ta, tb;
		duration<double, std::milli> time_span;
		system_clock::duration time_tag;
		
		//milliseconds from base time
		uint64_t time_from_base;
		// convert to number of days:
		typedef duration<int,std::ratio<1,1000>> millisecondTimeType;
		std::time_t tt = system_clock::to_time_t(t_base);
		cout << "Base time: " << ctime(&tt) << endl;
		//cout << "Now time: " << ctime(system_clock::now()) << endl;
		
		//remove current image from cameras
		idx = cam1->grab(&img_frame, bytes_used); if (img_frame) cam1->release(idx);
		idx = cam2->grab(&img_frame, bytes_used); if (img_frame) cam2->release(idx);
		idx = cam3->grab(&img_frame, bytes_used); if (img_frame) cam3->release(idx);

		cout<< "Capturing start!" << endl;
		int time_diff;
		
		while (ok) {
			//Algo!!
			//if image in cam1 -> get image, release camera
			//wait for image in cam2 -> get image, release camera
			//wait for image in cam3 -> get image, release camera
			//note time
			//save images in multi-threading and update counter else clean all cameras
			
			img_frame = NULL;	//just a precaution so that old frame is not picked again
			//cam1
			idx = cam1->grab(&img_frame, bytes_used);
			t1 = high_resolution_clock::now();
			time_tag = system_clock::now() - t_base;
			if (img_frame) {
				ta = high_resolution_clock::now();
				unsigned char image1[height][width];
				memcpy( image1[0], img_frame, height*width * sizeof(unsigned char));
				cam1->release(idx);
				//cam2
				idx = cam2->grab(&img_frame, bytes_used);
				if (img_frame) {
					tb = high_resolution_clock::now();
					unsigned char image2[height][width];
					memcpy( image2[0], img_frame, height*width * sizeof(unsigned char));
					cam2->release(idx);
					//cam3
					idx = cam3->grab(&img_frame, bytes_used);
					if (img_frame) {
						//tc = high_resolution_clock::now();
						unsigned char image3[height][width];
						memcpy( image3[0], img_frame, height*width * sizeof(unsigned char));
						cam3->release(idx);
						unsigned char (*img1)[height][width] = &image1;
						unsigned char (*img2)[height][width] = &image2;
						unsigned char (*img3)[height][width] = &image3;
						millisecondTimeType n_time = duration_cast<millisecondTimeType> (time_tag);
						time_from_base = (uint64_t)n_time.count();
						
						if(showCaptures) {
							cv::Mat image_mat_Bayer1(height,width,CV_8UC(1),image1);		//making an opencv Mat array
							cv::Mat image_mat_RGB1;
							cv::cvtColor(image_mat_Bayer1, image_mat_RGB1, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
							cv::Mat image_mat_Bayer2(height,width,CV_8UC(1),image2);		//making an opencv Mat array
							cv::Mat image_mat_RGB2;
							cv::cvtColor(image_mat_Bayer2, image_mat_RGB2, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
							cv::Mat image_mat_Bayer3(height,width,CV_8UC(1),image3);		//making an opencv Mat array
							cv::Mat image_mat_RGB3;
							cv::cvtColor(image_mat_Bayer3, image_mat_RGB3, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
							//Display the grey scale converted frame
							cv::imshow(window1, image_mat_RGB1);
							cv::imshow(window2, image_mat_RGB2);
							cv::imshow(window3, image_mat_RGB3);
						}
						
						boost::thread thread_cam1(saveCapturedImage, camImgPrefix1, counter, time_from_base, img1, compression_params);
						boost::thread thread_cam2(saveCapturedImage, camImgPrefix2, counter, time_from_base, img2, compression_params);
						boost::thread thread_cam3(saveCapturedImage, camImgPrefix3, counter, time_from_base, img3, compression_params);
						t2 = high_resolution_clock::now();
						time_span = t2 - t1;
						cout << "\nSet " << counter<< " time_from_base " << time_from_base << " time " << ceil(time_span.count()) << "ms ";
						counter++;
						cv::waitKey(0);
					}
					else { cout << "Cam3_not_grabbed"; }
				}
				else { cout << "Cam2_not_grabbed"; }
			}
			else { cout << "Cam1_not_grabbed"; }
		}
    }

    Camera::~Camera() {
		cout << "Camera Object Destructor called. Cya!" << endl;
		ok = false;
		image_thread.join();
		if (cam1) delete cam1;
		if (cam2) delete cam2;
		if (cam3) delete cam3;
    }


};

