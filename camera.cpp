#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "camera.h"
#include <iostream>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <string.h>

//gpio trigger header files
#include <termios.h>
#include "jetsonGPIO.h"

//mavlink header files
#include "mavlink_control.h"

using namespace std;
using namespace std::chrono;


//
// gpioExport
// Export the given gpio to userspace;
// Return: Success = 0 ; otherwise open file error
int gpioExport ( jetsonGPIO gpio )
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioExport unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    length = snprintf(commandBuffer, sizeof(commandBuffer), "%d", gpio);
    if (write(fileDescriptor, commandBuffer, length) != length) {
        perror("gpioExport");
        return fileDescriptor ;

    }
    close(fileDescriptor);

    return 0;
}

//
// gpioUnexport
// Unexport the given gpio from userspace
// Return: Success = 0 ; otherwise open file error
int gpioUnexport ( jetsonGPIO gpio )
{
    int fileDescriptor, length;
    char commandBuffer[MAX_BUF];

    fileDescriptor = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioUnexport unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    length = snprintf(commandBuffer, sizeof(commandBuffer), "%d", gpio);
    if (write(fileDescriptor, commandBuffer, length) != length) {
        perror("gpioUnexport") ;
        return fileDescriptor ;
    }
    close(fileDescriptor);
    return 0;
}

// gpioSetDirection
// Set the direction of the GPIO pin 
// Return: Success = 0 ; otherwise open file error
int gpioSetDirection ( jetsonGPIO gpio, unsigned int out_flag )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetDirection unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    if (out_flag) {
        if (write(fileDescriptor, "out", 4) != 4) {
            perror("gpioSetDirection") ;
            return fileDescriptor ;
        }
    }
    else {
        if (write(fileDescriptor, "in", 3) != 3) {
            perror("gpioSetDirection") ;
            return fileDescriptor ;
        }
    }
    close(fileDescriptor);
    return 0;
}

//
// gpioSetValue
// Set the value of the GPIO pin to 1 or 0
// Return: Success = 0 ; otherwise open file error
int gpioSetValue ( jetsonGPIO gpio, unsigned int value )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetValue unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    if (value) {
        if (write(fileDescriptor, "1", 2) != 2) {
            perror("gpioSetValue") ;
            return fileDescriptor ;
        }
    }
    else {
        if (write(fileDescriptor, "0", 2) != 2) {
            perror("gpioSetValue") ;
            return fileDescriptor ;
        }
    }
    close(fileDescriptor);
    return 0;
}

//
// gpioGetValue
// Get the value of the requested GPIO pin ; value return is 0 or 1
// Return: Success = 0 ; otherwise open file error
int gpioGetValue ( jetsonGPIO gpio, unsigned int *value)
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];
    char ch;

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fileDescriptor = open(commandBuffer, O_RDONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioGetValue unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    if (read(fileDescriptor, &ch, 1) != 1) {
        perror("gpioGetValue") ;
        return fileDescriptor ;
     }

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    close(fileDescriptor);
    return 0;
}


//
// gpioSetEdge
// Set the edge of the GPIO pin
// Valid edges: 'none' 'rising' 'falling' 'both'
// Return: Success = 0 ; otherwise open file error
int gpioSetEdge ( jetsonGPIO gpio, char *edge )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioSetEdge unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    if (write(fileDescriptor, edge, strlen(edge) + 1) != ((int)(strlen(edge) + 1))) {
        perror("gpioSetEdge") ;
        return fileDescriptor ;
    }
    close(fileDescriptor);
    return 0;
}

//
// gpioOpen
// Open the given pin for reading
// Returns the file descriptor of the named pin
int gpioOpen( jetsonGPIO gpio )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fileDescriptor = open(commandBuffer, O_RDONLY | O_NONBLOCK );
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioOpen unable to open gpio%d",gpio) ;
        perror(errorBuffer);
    }
    return fileDescriptor;
}

//
// gpioClose
// Close the given file descriptor 
int gpioClose ( int fileDescriptor )
{
    return close(fileDescriptor);
}

// gpioActiveLow
// Set the active_low attribute of the GPIO pin to 1 or 0
// Return: Success = 0 ; otherwise open file error
int gpioActiveLow ( jetsonGPIO gpio, unsigned int value )
{
    int fileDescriptor;
    char commandBuffer[MAX_BUF];

    snprintf(commandBuffer, sizeof(commandBuffer), SYSFS_GPIO_DIR "/gpio%d/active_low", gpio);

    fileDescriptor = open(commandBuffer, O_WRONLY);
    if (fileDescriptor < 0) {
        char errorBuffer[128] ;
        snprintf(errorBuffer,sizeof(errorBuffer), "gpioActiveLow unable to open gpio%d",gpio) ;
        perror(errorBuffer);
        return fileDescriptor;
    }

    if (value) {
        if (write(fileDescriptor, "1", 2) != 2) {
            perror("gpioActiveLow") ;
            return fileDescriptor ;
        }
    }
    else {
        if (write(fileDescriptor, "0", 2) != 2) {
            perror("gpioActiveLow") ;
            return fileDescriptor ;
        }
    }
    close(fileDescriptor);
    return 0;
}



namespace uvc_camera {
	
	jetsonTX1GPIONumber redLED = gpio398 ;     // Ouput  gpio398
	
	Camera::Camera(){
		/* default config values */
		counter = 0;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(0);

		/* initialize the cameras */
		cam1 = Camera::setCamera(cam1, 1);
		cam2 = Camera::setCamera(cam2, 2);
		cam3 = Camera::setCamera(cam3, 3);
		
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
        
    uvc_cam::Cam* Camera::setCamera(uvc_cam::Cam *cam, int deviceID){
		string deviceStr = "/dev/video" + to_string(deviceID);
		
		/*using namespace cv;
		//try to get the cameras in specific resolution
		VideoCapture cap;
		cap.open(deviceID);
		if(!cap.isOpened())
			cout << "***Cam " << deviceID <<  " resolution not set***\n";
		cap.set(CV_CAP_PROP_FRAME_WIDTH,width);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,height);
		//Mat CameraFrame;
		//cap >> CameraFrame;
		//if( CameraFrame.empty() )
		//    cout << "***Cam " << deviceID <<  " CameraFrame not captured***\n";
		//namedWindow(deviceStr, CV_WINDOW_AUTOSIZE);
		//imshow(deviceStr, CameraFrame);
		//cv::waitKey(1000);
		cap.release();
		*/
		
		/* initialize the cameras */
		int fps = 10;
		cam = new uvc_cam::Cam(deviceStr.c_str(), uvc_cam::Cam::MODE_BAYER, width, height, fps);
		//cam1->set_motion_thresholds(100, -1);
		cam->set_control(0x009a0901, 0); // exposure, auto (0 = auto, 1 = manual)
		cam->set_control(0x00980900, 9); // brightness
		//cam->set_control(0x9a0902, 78); // exposure time 15.6ms
		//usleep(500000);
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
	
	void triggerCameras() {
		// Flash the LED
		while(true){
			cout << "Setting the LED on" << endl;
			gpioSetValue(redLED, on);
			usleep(1);         // on for 1ms
			cout << "Setting the LED off" << endl;
			gpioSetValue(redLED, off);
			usleep(5);         // off for 5ms
		}
		
	}
	
	
    // ------------------------------------------------------------------------------
	//   Quit Signal Handler
	// ------------------------------------------------------------------------------
	// this function is called when you press Ctrl-C
	void quit_handler( int sig )
	{
		printf("\n");
		printf("TERMINATING AT USER REQUEST\n");
		printf("\n");

		// autopilot interface
		try {
			autopilot_interface_quit->handle_quit(sig);
		}
		catch (int error){}

		// serial port
		try {
			serial_port_quit->handle_quit(sig);
		}
		catch (int error){}

		// end program here
		exit(0);

	}
	
    void Camera::feedImages() {
		//set gpio trigger
		gpioUnexport(redLED);     // unexport the LED
		gpioExport(redLED);
		gpioSetDirection(redLED,outputPin) ;
		
		//mavlink start
		char *uart_name = (char*)"/dev/ttyUSB0";
		int baudrate = 115200;
		Serial_Port serial_port(uart_name, baudrate);
		Autopilot_Interface api(&serial_port);
		serial_port_quit         = &serial_port;
		autopilot_interface_quit = &api;
		signal(SIGINT,quit_handler);
		serial_port.start();
		api.start();
				
		mavlink_local_position_ned_t lpos;
		mavlink_global_position_int_t gpos;
		mavlink_attitude_t att;
		uint lastpostime = 0;
		
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
		
		bool showCaptures = false;	//to display the captured images during runtime
		
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
		//idx = cam1->grab(&img_frame, bytes_used); if (img_frame) cam1->release(idx);
		//idx = cam2->grab(&img_frame, bytes_used); if (img_frame) cam2->release(idx);
		//idx = cam3->grab(&img_frame, bytes_used); if (img_frame) cam3->release(idx);

		cout<< "Capturing start!" << endl;
		int time_diff;
		
		while (ok) {
			//Algo!!
			//if image in cam1 -> get image, release camera
			//wait for image in cam2 -> get image, release camera
			//wait for image in cam3 -> get image, release camera
			//note time
			//save images in multi-threading and update counter else clean all cameras
			
			gpos = api.current_messages.global_position_int;
			lpos = api.current_messages.local_position_ned;
			att = api.current_messages.attitude;
			if(gpos.time_boot_ms > lastpostime)
			{
				t1 = high_resolution_clock::now();
				//trigger cameras
				triggerCameras();
				lastpostime = gpos.time_boot_ms;
				
				img_frame = NULL;	//just a precaution so that old frame is not picked again
				//cam1
				idx = cam1->grab(&img_frame, bytes_used);
				//t1 = high_resolution_clock::now();
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
								cv::waitKey(0);
							}
							
							boost::thread thread_cam1(saveCapturedImage, camImgPrefix1, counter, time_from_base, img1, compression_params);
							boost::thread thread_cam2(saveCapturedImage, camImgPrefix2, counter, time_from_base, img2, compression_params);
							boost::thread thread_cam3(saveCapturedImage, camImgPrefix3, counter, time_from_base, img3, compression_params);
							t2 = high_resolution_clock::now();
							time_span = t2 - t1;
							cout << "\nSet " << counter<< " time_from_base " << time_from_base << " time " << ceil(time_span.count()) << "ms ";
							printf("GLOBAL POS = [ lat=%i , lon=%i , alt=%i , rel_alt=%i , vel=%i , %i , %i, hdg=%u ] \n", gpos.lat, gpos.lon, gpos.alt, gpos.relative_alt, gpos.vx, gpos.vy, gpos.vz, gpos.hdg);
							printf("LOCAL POS  = [ %f %f %f (m)\n", lpos.x, lpos.y, lpos.z );
							printf("ATTITUDE   = [ roll=%f , pitch=%f , yaw=%f , speeds=%f, %f, %f ] \n", att.roll, att.pitch, att.yaw, att.rollspeed, att.pitchspeed, att.yawspeed);
							counter++;
						}
						else { cout << "Cam3_not_grabbed"; }
					}
					else { cout << "Cam2_not_grabbed"; }
				}
				else { cout << "Cam1_not_grabbed"; }
			}
		}
		
		gpioUnexport(redLED);     // unexport the LED
		
		//mavlink stop
		api.stop();
		serial_port.stop();
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

