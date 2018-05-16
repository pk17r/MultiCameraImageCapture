//#include "opencv2/video/tracking.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"

#include "camera.h"
#include <iostream>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <string.h>
#include <fstream>

//gpio trigger header files
#include <termios.h>
#include "jetsonGPIO.h"

//mavlink header files
#include "mavlink_control.h"

using namespace std;
using namespace std::chrono;
using namespace cv;


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
	ofstream outfile;
	
	Camera::Camera(Settings settings){
		camImgPrefix1 = settings.save_directory + "cam1/";
		camImgPrefix2 = settings.save_directory + "cam2/";
		camImgPrefix3 = settings.save_directory + "cam3/";
		
		/* default config values */
		counter = 0;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(0);
		
		height_sz = Resolution[settings.resolution][height];
		width_sz = Resolution[settings.resolution][width];
		
		cout << "\nIf output is 'Unable to find parent usb device.' or if you want to use TX2 GPIO trigger, run executable using administrative rights." << endl;
		usleep(1000000);

		/* initialize the cameras */
		int cam_Ind = 0;
		if(settings.use_cam_x[cam_Ind]) setCamera(cam_Ind++, settings);
		if(settings.use_cam_x[cam_Ind]) setCamera(cam_Ind++, settings);
		if(settings.use_cam_x[cam_Ind]) setCamera(cam_Ind++, settings);
		
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
		
		Camera::feedImages(settings);
    }
        
    void Camera::setCamera(int cam_Ind, Settings settings){
		string deviceStr = "/dev/video" + to_string(settings.cam_x_ID[cam_Ind]);
		/* initialize the cameras */
		cam[cam_Ind] = new uvc_cam::Cam(deviceStr.c_str(), uvc_cam::Cam::MODE_BAYER, 
				Resolution[settings.resolution][width], Resolution[settings.resolution][height], 10 /*fps*/);
		//cam1->set_motion_thresholds(100, -1);
		if(settings.exposure == 0) {
			cam[cam_Ind]->set_control(0x009a0901, 0); // exposure, auto (0 = auto, 1 = manual)
		} else {
			cam[cam_Ind]->set_control(0x009a0901, 1); // exposure, auto (0 = auto, 1 = manual)
			cam[cam_Ind]->set_control(0x009a0902, settings.exposure); // exposure value
		}
		cam[cam_Ind]->set_control(0x00980900, settings.brightness); // brightness
		//cam->set_control(0x9a0902, 78); // exposure time 15.6ms
		//usleep(500000);
		//return cam;
	}
    
	void saveCapturedImage(string camImgPrefix, int counter_, uint64_t time_from_base, Settings settings, unsigned char *image_ptr, std::vector<int> compression_params) {
		unsigned char image[Resolution[settings.resolution][height]][Resolution[settings.resolution][width]];
		//std::chrono::high_resolution_clock::time_point t1, t2;
		//t1 = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double, std::milli> time_span;
		memcpy( &image[0][0], image_ptr, Resolution[settings.resolution][height] * Resolution[settings.resolution][width] * sizeof(unsigned char));
		cv::Mat image_mat_Bayer(Resolution[settings.resolution][height], Resolution[settings.resolution][width], CV_8UC(1), image);		//making an opencv Mat array
		cv::Mat image_mat_RGB;
		cv::cvtColor(image_mat_Bayer, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
		//saving image to disk
		cv::imwrite(camImgPrefix + to_string(settings.use_timestamp ? time_from_base : counter_) + camImgSuffix, image_mat_RGB, compression_params);
		//t2 = std::chrono::high_resolution_clock::now();
		//time_span = t2 - t1;
		//cout << counter_ << "_" << ceil(time_span.count()) << "ms " ;
	}
	
	void triggerCameras() {
		// Flash the LED
		//cout << "Setting the LED on" << endl;
		gpioSetValue(redLED, on);
		usleep(1);         // on for 1ms
		//cout << "Setting the LED off" << endl;
		gpioSetValue(redLED, off);
		usleep(2);         // off for 2ms
	}
	
	
    // ------------------------------------------------------------------------------
	//   Quit Signal Handler
	// ------------------------------------------------------------------------------
	// this function is called when you press Ctrl-C
	void quit_handler( int sig ) {
		cout<< "\n\nClose text log file" << endl;
		outfile.close();
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
	
	mavlink_local_position_ned_t lpos;
	mavlink_global_position_int_t gpos;
	mavlink_attitude_t att;
	Autopilot_Interface *api;
	
	void Camera::feedImages(Settings settings) {
		//bool showCaptures = false;	//to display the captured images during runtime
		//bool useMAVLinkForTrigger = false;	//use MAVLink GPS messages as trigger
		//bool useGPIOPinsAsTrigger = false;	//to use TX2 GPIO pins to trigger cameras
		
		//set gpio trigger
		if(settings.useGPIOPinsAsTrigger)
		{
			cout << "Initialize GPIOs" << endl;
			gpioUnexport(redLED);     // unexport the LED
			gpioExport(redLED);
			gpioSetDirection(redLED,outputPin) ;
			cout << "GPIOs initialized" << endl;
		}
		
		//MAVLink gives GPS and IMU data. GPS messages used as trigger for cameras
		if(settings.useMAVLinkForTrigger)
		{
			//current date
			time_t now = time(NULL);
			struct tm tstruct;
			char buf[40];
			tstruct = *localtime(&now);
			//format: day DD-MM-YYYY
			strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
			string current_date = string(buf);
			
			//current time
			tstruct = *localtime(&now);
			//format: HH:mm:ss
			strftime(buf, sizeof(buf), "%X", &tstruct);
			string current_time = string(buf);
			
			//milliseconds from base time
			system_clock::duration time_tag = system_clock::now() - t_base;
			// convert to milliseconds:
			typedef duration<int,std::ratio<1,1000>> millisecondTimeType;
			millisecondTimeType n_time = duration_cast<millisecondTimeType> (time_tag);
			uint64_t time_from_base = (uint64_t)n_time.count();
			
			string textFileName = textFilePrefix + "-" + current_date + "-" + to_string(time_from_base) + ".txt";
			cout << "Text file name: " << textFileName << endl;
			
			//open text file
			//ofstream outfile;
			outfile.open(textFileName, ios_base::app);
			outfile << current_date << "," << current_time << "\n";
			outfile <<"counter,time_from_base,gps,time_to_fetch,gpos.lat,gpos.lon,gpos.alt,gpos.relative_alt,gpos.vx,gpos.vy,gpos.vz,gpos.hdg,lpos.x,lpos.y,lpos.z\n";
			outfile <<"counter,time_from_base,imu,att.roll,att.pitch,att.yaw,att.rollspeed,att.pitchspeed,att.yawspeed\n";
		
			//mavlink start
			cout << "Initialize MAVLINK" << endl;
			char *uart_name = (char *)settings.MAVLinkPort.c_str();
			int baudrate = 115200;
			Serial_Port serial_port(uart_name, baudrate);
			Autopilot_Interface autopilot_interface(&serial_port);
			serial_port_quit         = &serial_port;
			autopilot_interface_quit = &autopilot_interface;
			signal(SIGINT,quit_handler);
			cout << "Start serial port" << endl;
			serial_port.start();
			cout << "Start api" << endl;
			autopilot_interface.start();
			api = &autopilot_interface;
			
			cout << "MAVLINK initialized" << endl;
			
			fetchImagesFunction(settings);
			
			//mavlink stop
			autopilot_interface.stop();
			serial_port.stop();
		}
		else
		{
			fetchImagesFunction(settings);
		}
		
		if(settings.useGPIOPinsAsTrigger)
		{
			gpioUnexport(redLED);     // unexport the LED
		}
    }
    
    void Camera::fetchImagesFunction(Settings settings) {
		high_resolution_clock::time_point t1, t2, ta, tb;
		duration<double, std::milli> time_span;
		system_clock::duration time_tag;
		
		//milliseconds from base time
		uint64_t time_from_base;
		// convert to milliseconds:
		typedef duration<int,std::ratio<1,1000>> millisecondTimeType;
		std::time_t tt = system_clock::to_time_t(t_base);
		cout << "Base time: " << ctime(&tt) << endl;
		
		time_tag = system_clock::now() - t_base;
		millisecondTimeType n_time = duration_cast<millisecondTimeType> (time_tag);
		time_from_base = (uint64_t)n_time.count();
		
		unsigned char *img_frame = NULL;
		uint32_t bytes_used;
		int idx;

		int cam_Ind = 0;
		string windowNames[] = {"Cam1", "Cam2", "Cam3"};
		if(settings.showCaptures) {
			if(settings.use_cam_x[cam_Ind]) namedWindow(windowNames[cam_Ind++], CV_WINDOW_AUTOSIZE);
			if(settings.use_cam_x[cam_Ind]) namedWindow(windowNames[cam_Ind++], CV_WINDOW_AUTOSIZE);
			if(settings.use_cam_x[cam_Ind]) namedWindow(windowNames[cam_Ind++], CV_WINDOW_AUTOSIZE);
		}
		
		cout<< "Capturing start!" << endl;
		int time_diff;
		uint lastPosTime = 0, lastAttTime = 0;
		cout << "\nSet(#)  Timestamp \t\tImage_Fetch_time(ms)" << endl;
		
		while (ok) {
			//Algo!!
			//if image in cam1 -> get image, release camera
			//wait for image in cam2 -> get image, release camera
			//wait for image in cam3 -> get image, release camera
			//note time
			//save images in multi-threading and update counter else clean all cameras
			
			bool imgCycleBool = true;
			bool attCycleBool = false;
			if(settings.useMAVLinkForTrigger)
			{
				gpos = api->current_messages.global_position_int;
				lpos = api->current_messages.local_position_ned;
				att = api->current_messages.attitude;
				
				imgCycleBool = gpos.time_boot_ms > lastPosTime;
				attCycleBool = att.time_boot_ms > lastAttTime;
			}
			
			if(imgCycleBool)
			{
				// = high_resolution_clock::now();
				
				//trigger cameras using GPIO pins
				if(settings.useGPIOPinsAsTrigger)
					triggerCameras();
				
				//update last GPS time from MAVLink
				if(settings.useMAVLinkForTrigger)
					lastPosTime = gpos.time_boot_ms;
				
				//cam1
				img_frame = NULL;	//just a precaution so that old frame is not picked again
				cam_Ind = 0;
				if(settings.use_cam_x[cam_Ind])
					idx = cam[cam_Ind]->grab(&img_frame, bytes_used);
				t1 = high_resolution_clock::now();
				time_tag = system_clock::now() - t_base;
				if (img_frame || !settings.use_cam_x[cam_Ind]) {
					ta = high_resolution_clock::now();
					unsigned char image1[Resolution[settings.resolution][height]][Resolution[settings.resolution][width]];
					if(settings.use_cam_x[cam_Ind]) {
						memcpy( image1[0], img_frame, Resolution[settings.resolution][height] * Resolution[settings.resolution][width] * sizeof(unsigned char));
						cam[cam_Ind]->release(idx);
					}
					//cam2
					cam_Ind++;
					if(settings.use_cam_x[cam_Ind])
						idx = cam[cam_Ind]->grab(&img_frame, bytes_used);
					if (img_frame || !settings.use_cam_x[cam_Ind]) {
						tb = high_resolution_clock::now();
						unsigned char image2[Resolution[settings.resolution][height]][Resolution[settings.resolution][width]];
						if(settings.use_cam_x[cam_Ind]) {
							memcpy( image2[0], img_frame, Resolution[settings.resolution][height] * Resolution[settings.resolution][width] * sizeof(unsigned char));
							cam[cam_Ind]->release(idx);
						}
						//cam3
						cam_Ind++;
						if(settings.use_cam_x[cam_Ind])
							idx = cam[cam_Ind]->grab(&img_frame, bytes_used);
						if (img_frame || !settings.use_cam_x[cam_Ind]) {
							//tc = high_resolution_clock::now();
							unsigned char image3[Resolution[settings.resolution][height]][Resolution[settings.resolution][width]];
							if(settings.use_cam_x[cam_Ind]) {
								memcpy( image3[0], img_frame, Resolution[settings.resolution][height] * Resolution[settings.resolution][width] * sizeof(unsigned char));
								cam[cam_Ind]->release(idx);
							}
							
							n_time = duration_cast<millisecondTimeType> (time_tag);
							time_from_base = (uint64_t)n_time.count();
							
							if(settings.showCaptures) {
								cam_Ind = 0;
								if(settings.use_cam_x[cam_Ind]) {
									Mat image_mat_Bayer1(Resolution[settings.resolution][height], Resolution[settings.resolution][width], CV_8UC(1), image1);		//making an opencv Mat array
									Mat image_mat_RGB1;
									cvtColor(image_mat_Bayer1, image_mat_RGB1, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
									imshow(windowNames[cam_Ind], image_mat_RGB1);							//Display the grey scale converted frame
								}
								cam_Ind++;
								if(settings.use_cam_x[cam_Ind]) {
									Mat image_mat_Bayer2(Resolution[settings.resolution][height], Resolution[settings.resolution][width],CV_8UC(1),image2);		//making an opencv Mat array
									Mat image_mat_RGB2;
									cvtColor(image_mat_Bayer2, image_mat_RGB2, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
									imshow(windowNames[cam_Ind], image_mat_RGB2);							//Display the grey scale converted frame
								}
								cam_Ind++;
								if(settings.use_cam_x[cam_Ind]) {
									Mat image_mat_Bayer3(Resolution[settings.resolution][height], Resolution[settings.resolution][width],CV_8UC(1),image3);		//making an opencv Mat array
									Mat image_mat_RGB3;
									cvtColor(image_mat_Bayer3, image_mat_RGB3, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
									imshow(windowNames[cam_Ind], image_mat_RGB3);							//Display the grey scale converted frame
								}
								cv::waitKey(0);
							}
							cam_Ind = 0;
							if(settings.use_cam_x[cam_Ind]) {
								boost::thread thread_cam1(saveCapturedImage, camImgPrefix1, counter, time_from_base, settings, &image1[0][0], compression_params);
							}
							cam_Ind++;
							if(settings.use_cam_x[cam_Ind]) {
								boost::thread thread_cam2(saveCapturedImage, camImgPrefix2, counter, time_from_base, settings, &image2[0][0], compression_params);
							}
							cam_Ind++;
							if(settings.use_cam_x[cam_Ind]) {
								boost::thread thread_cam3(saveCapturedImage, camImgPrefix3, counter, time_from_base, settings, &image3[0][0], compression_params);
							}
							t2 = high_resolution_clock::now();
							time_span = t2 - t1;
							cout << counter<< "\t" << time_from_base << "\t" << ceil(time_span.count()) << endl;
							if(ceil(time_span.count()) > 10)
								cout << "Possible faulty start. Image fetch time > 10 ms!" << endl;
							//if(useMAVLinkForTrigger) {
							//	printf("GLOBAL POS = [ lat=%i , lon=%i , alt=%i , rel_alt=%i , vel=%i , %i , %i, hdg=%u ] \n", gpos.lat, gpos.lon, gpos.alt, gpos.relative_alt, gpos.vx, gpos.vy, gpos.vz, gpos.hdg);
							//	printf("LOCAL POS  = [ %f %f %f (m)\n", lpos.x, lpos.y, lpos.z );
							//	printf("ATTITUDE   = [ roll=%f , pitch=%f , yaw=%f , speeds=%f, %f, %f ] \n", att.roll, att.pitch, att.yaw, att.rollspeed, att.pitchspeed, att.yawspeed);
							//}
							
							if(settings.useMAVLinkForTrigger)
								outfile <<counter<<","<<time_from_base<<",g,"<<ceil(time_span.count())<<","<<gpos.lat<<","<<gpos.lon<<","<<gpos.alt<<","<<gpos.relative_alt<<","<<gpos.vx<<","<<gpos.vy<<","<<gpos.vz<<","<<gpos.hdg<<","<<lpos.x<<","<<lpos.y<<","<<lpos.z<<","<<"\n";
							
							counter++;
						}
						else { cout << "Cam3_not_grabbed"; }
					}
					else { cout << "Cam2_not_grabbed"; }
				}
				else { cout << "Cam1_not_grabbed"; }
			}
			
			//imu runs faster than gps so recording it separately
			if(attCycleBool && settings.useMAVLinkForTrigger) 
			{
				//update last IMU time from MAVLink
				if(settings.useMAVLinkForTrigger)
					lastAttTime = att.time_boot_ms;
				
				time_tag = system_clock::now() - t_base;
				n_time = duration_cast<millisecondTimeType> (time_tag);
				time_from_base = (uint64_t)n_time.count();
				outfile <<counter<<","<<time_from_base<<",i,"<<att.roll<<","<<att.pitch<<","<<att.yaw<<","<<att.rollspeed<<","<<att.pitchspeed<<","<<att.yawspeed<<"\n";
			}
		}
	}
	
    Camera::~Camera() {
		cout << "Camera Object Destructor called. Cya!" << endl;
		ok = false;
		image_thread.join();
		int cam_Ind = 0;
		if (cam[cam_Ind]) delete cam[cam_Ind]; cam_Ind++;
		if (cam[cam_Ind]) delete cam[cam_Ind]; cam_Ind++;
		if (cam[cam_Ind]) delete cam[cam_Ind]; cam_Ind++;
    }


};

