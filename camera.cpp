#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "camera.h"
//#include <stdlib.h>     /* atoi */
#include <iostream>
#include <fstream>
#include <unistd.h>		//for usleep
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace uvc_camera {
	
	Camera::Camera(){
	//cam(0);
	//a = 99;
	//cout << "a: " << a << endl;
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
      cv::Mat image_mat_Bayer1 = cv::Mat(height,width,CV_8U);
      cv::Mat image_mat_Bayer2 = cv::Mat(height,width,CV_8U);
      cv::Mat image_mat_Bayer3 = cv::Mat(height,width,CV_8U);
      cv::Mat image_mat_RGB = cv::Mat(height,width,CV_8UC(3));
      
      /*Camera::setCamera(cam1, device1);
      Camera::setCamera(cam2, device2);
      Camera::setCamera(cam3, device3);*/
      /* initialize the cameras */
      cam1 = new uvc_cam::Cam(device1.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
      cam2 = new uvc_cam::Cam(device2.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
      cam3 = new uvc_cam::Cam(device3.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
      //cam1->set_motion_thresholds(100, -1);
      cam1->set_control(0x009a0901, 1); // exposure, auto (0 = auto, 1 = manual)
      cam1->set_control(0x00980900, 8); // brightness
      cam1->set_control(0x9a0902, 78); // exposure time 15.6ms*/
      //cam1->set_motion_thresholds(100, -1);
      cam2->set_control(0x009a0901, 1); // exposure, auto (0 = auto, 1 = manual)
      cam2->set_control(0x00980900, 8); // brightness
      cam2->set_control(0x9a0902, 78); // exposure time 15.6ms*/
      //cam1->set_motion_thresholds(100, -1);
      cam3->set_control(0x009a0901, 1); // exposure, auto (0 = auto, 1 = manual)
      cam3->set_control(0x00980900, 8); // brightness
      cam3->set_control(0x9a0902, 78); // exposure time 15.6ms*/
      
      /* and turn on the streamer */
      ok = true;
      //image_thread = boost::thread(boost::bind(&Camera::feedImages, this));
      cout << "Call feedImages()" << endl;
      Camera::feedImages();
    }
    
    void Camera::setCamera(uvc_cam::Cam *cam, string device){
		/* initialize the cameras */
		cam = new uvc_cam::Cam(device.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
		//cam1->set_motion_thresholds(100, -1);
		cam->set_control(0x009a0901, 1); // exposure, auto (0 = auto, 1 = manual)
		cam->set_control(0x00980900, 8); // brightness
		cam->set_control(0x9a0902, 78); // exposure time 15.6ms
	}
    
    void Camera::cleanCameras(uvc_cam::Cam *cam, unsigned char *img_frame, uint32_t bytes_used){
		cout << "*** In CleanCameras ****" << endl;
		int idx = cam->grab(&img_frame, bytes_used);
		while(img_frame)
		{
			cout << "*** runCleanCamera ****" << endl;
			cam->release(idx);
			idx = cam->grab(&img_frame, bytes_used);
		}
	}
	
	void Camera::getImgMatFromCamera(unsigned char *img_frame, unsigned char (*image_ptr)[height][width]) {
		/*cout << "**** First 10 pixel readings: ";
		for (int i = 0; i < 10; i++)
		{
			printf("%u ", (unsigned int)img_frame[i]);
		}
		printf("\n");*/
		//*image[0]=*img_frame;
		//cv::Mat aa = cv::Mat(image);
		//cv::imshow("Cam1", aa);
		//ofstream myfile;
		//camImgPath1 = camImgPrefix1 + to_string(counter) + camImgSuffixTxt;
		//myfile.open (camImgPath1);
		for (int i = 0; i < height; i++)
		{
		 for (int j = 0; j < width; j++)
		 {
			 (*image_ptr)[i][j]=img_frame[i*width + j];
			 //image_mat_Bayer.data[i*width + j]=img_frame[i*width + j];
			 //myfile<< (int)img_frame[i*width + j] << " ";
		 }
		 //myfile<<"\n";
		}
	}

    void Camera::feedImages() {
		cout << "In feedImages() "<<endl;
		unsigned char *img_frame = NULL;
		uint32_t bytes_used;
		int idx;
		unsigned char (*img1)[height][width] = &image1;
		unsigned char (*img2)[height][width] = &image2;
		unsigned char (*img3)[height][width] = &image3;
		cout << "*** A ****" << endl;
		high_resolution_clock::time_point t1, t2, t3, t4;
		duration<double, std::milli> time_span;
		t1 = high_resolution_clock::now();
		cout << "*** B ****" << endl;
		//Camera::cleanCameras(cam1, img_frame, bytes_used);
		idx = cam1->grab(&img_frame, bytes_used);
		cout << "*** C ****" << endl;
		if (img_frame) cam1->release(idx);
		cout << "*** D ****" << endl;
		t2 = high_resolution_clock::now();
		cout << "*** E ****" << endl;
		//Camera::cleanCameras(cam2, img_frame, bytes_used);
		idx = cam2->grab(&img_frame, bytes_used);
		if (img_frame) cam2->release(idx);
		t3 = high_resolution_clock::now();
		//Camera::cleanCameras(cam3, img_frame, bytes_used);
		idx = cam3->grab(&img_frame, bytes_used);
		if (img_frame) cam3->release(idx);
		t4 = high_resolution_clock::now();
		time_span = t2 - t1;
		cout << "*** F ****" << endl;
		cout<< "Time to clean cam1: " << time_span.count() << "ms" << endl;
		time_span = t3 - t2;
		cout<< "Time to clean cam2: " << time_span.count() << "ms" << endl;
		time_span = t4 - t3;
		cout<< "Time to clean cam3: " << time_span.count() << "ms" << endl;
		t1 = high_resolution_clock::now();
		time_span = t1 - t4;
		cout<< "Time to print this: " << time_span.count() << "ms" << endl;

		usleep(10000);	//sleep for ## microseconds before start capturing images
		
		cout<< "Capturing start!" << endl;
		
		int time_diff;
		
      while (ok) {
        //time start
        //if image in cam1 -> get image, release camera
        //wait for image in cam2 -> get image, release camera
        //wait for image in cam3 -> get image, release camera
        //note time
        //if time is less than 300 ms, save images and update counter else clean all cameras

		//cam1
        idx = cam1->grab(&img_frame, bytes_used);
        if (img_frame) {
			t1 = high_resolution_clock::now();
			Camera::getImgMatFromCamera(img_frame, img1);
			cam1->release(idx);
			//cam2
			idx = cam2->grab(&img_frame, bytes_used);
			while(!img_frame) { cout << "2"; idx = cam2->grab(&img_frame, bytes_used); }
			if (img_frame) {
				Camera::getImgMatFromCamera(img_frame, img2);
				cam2->release(idx);
				//cam3
				idx = cam3->grab(&img_frame, bytes_used);
				while(!img_frame) { cout << "3"; idx = cam3->grab(&img_frame, bytes_used); }
				if (img_frame) {
					Camera::getImgMatFromCamera(img_frame, img3);
					cam3->release(idx);
					t2 = high_resolution_clock::now();
					time_span = t2 - t1;
					cout << "Time to fetch images: " << time_span.count() << "ms" << endl;
					if(time_diff < 300) {
						//img1
						cv::Mat image_mat_Bayer1(height,width,CV_8UC(1),image1);
						cv::cvtColor(image_mat_Bayer1, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB
						cv::imwrite(camImgPrefix1 + to_string(counter) + camImgSuffix, image_mat_RGB, compression_params);
						//img2
						cv::Mat image_mat_Bayer2(height,width,CV_8UC(1),image2);
						cv::cvtColor(image_mat_Bayer2, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB
						cv::imwrite(camImgPrefix2 + to_string(counter) + camImgSuffix, image_mat_RGB, compression_params);
						//img3
						cv::Mat image_mat_Bayer3(height,width,CV_8UC(1),image3);
						cv::cvtColor(image_mat_Bayer3, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB
						cv::imwrite(camImgPrefix3 + to_string(counter) + camImgSuffix, image_mat_RGB, compression_params);
						t3 = high_resolution_clock::now();
						time_span = t3 - t2;
						cout << "Time to save images: " << time_span.count() << "ms" << endl;
						time_span = t3 - t1;
						cout << "Total Cycle Time is: " << time_span.count() << "ms" << endl;
						cout << "% Counter: " << counter << endl;
						counter++;
					}
				}
			}
		}
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

