#include "camera.h"
#include <iostream>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <string.h>
#include <fstream>

//gpio trigger header files
#include <termios.h>
#include "jetsonGPIO/jetsonGPIO.c"

//mavlink header files
#include "mavlink/mavlink_control.h"

using namespace std;
using namespace std::chrono;
using namespace cv;

namespace uvc_camera {
	
	jetsonTX1GPIONumber redLED = gpio398 ;     // Ouput  gpio398
	ofstream outfile;
	vector<int> compression_params;
	string windowNames[] = {"Cam1", "Cam2", "Cam3"};
	
	Camera::Camera(Settings settings){
		usleep(1000000);
		/* default config values */
		counter = 0;
		uvc_camera::compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		uvc_camera::compression_params.push_back(0);
		
		/* initialize the cameras */
		int cam_Ind = 0;
		if(settings.use_cam_x[cam_Ind]) setCamera(cam_Ind, settings); cam_Ind++;
		if(settings.use_cam_x[cam_Ind]) setCamera(cam_Ind, settings); cam_Ind++;
		if(settings.use_cam_x[cam_Ind]) setCamera(cam_Ind, settings); cam_Ind++;
		
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
		/* initialize the camera */
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
		usleep(500000);
	}
    
    void showWindows(Settings settings) {
		int cam_Ind = 0;
		if(settings.use_cam_x[cam_Ind]) namedWindow(windowNames[cam_Ind], CV_WINDOW_AUTOSIZE); cam_Ind++;
		if(settings.use_cam_x[cam_Ind]) namedWindow(windowNames[cam_Ind], CV_WINDOW_AUTOSIZE); cam_Ind++;
		if(settings.use_cam_x[cam_Ind]) namedWindow(windowNames[cam_Ind], CV_WINDOW_AUTOSIZE); cam_Ind++;
	}
    
    void saveFetchedImage(int cam_Ind, uvc_cam::Cam *cam, bool *cam_lock_ptr, int idx, Settings settings, unsigned char *img_frame, int counter_, uint64_t time_from_base) {
		unsigned char image[Resolution[settings.resolution][height]][Resolution[settings.resolution][width]];
		memcpy( image[0], img_frame, Resolution[settings.resolution][height] * Resolution[settings.resolution][width] * sizeof(unsigned char));
		cam->release(idx);
		*(cam_lock_ptr + cam_Ind) = false;	//removing camera lock after release
				
		cv::Mat image_mat_Bayer(Resolution[settings.resolution][height], Resolution[settings.resolution][width], CV_8UC(1), image);		//making an opencv Mat array
		cv::Mat image_mat_RGB;
		cv::cvtColor(image_mat_Bayer, image_mat_RGB, CV_BayerGR2RGB);	//CV_BayerRG2RGB -> Conversion
		if(settings.showCaptures) {
			imshow(windowNames[cam_Ind], image_mat_RGB);	//Display the grey scale converted frame
			waitKey(0.000001);
		}
		
		string saveName = settings.save_directory + "cam" + to_string(cam_Ind + 1) + "/" + to_string(settings.use_timestamp ? time_from_base : counter_) + ".png";
		if(counter_ > 0)	//leave the first image as it is generally bad!
			cv::imwrite(saveName, image_mat_RGB, compression_params);
		//cout << counter_ << "_" << saveName << endl;
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
		//set gpio trigger
		if(settings.useGPIOPinsAsTrigger)
		{
			cout << "Initialize GPIOs" << endl;
			gpioUnexport(redLED);     // unexport the LED
			gpioExport(redLED);
			gpioSetDirection(redLED,outputPin) ;
			cout << "GPIOs initialized" << endl;
		}
		
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
		
		//save settings file for record
		if(settings.use_timestamp) {
			string runSettingsFile = settings.save_directory + current_date + "_" + to_string(time_from_base) + "_settings.xml";
			FileStorage fs(runSettingsFile, FileStorage::WRITE);
			fs << "Settings" << settings;
			cout << "Saved settings file for record purpose: " << runSettingsFile << endl;
			fs.release();
		}
		
		//MAVLink gives GPS and IMU data. GPS messages used as trigger for cameras
		if(settings.useMAVLinkForTrigger)
		{
			string logFileName = settings.save_directory + current_date + "_" + to_string(time_from_base) + "_log.txt";
			cout << "Log file name: " << logFileName << endl;
			//open log file
			outfile.open(logFileName, ios_base::app);
			outfile << current_date << "," << current_time << "\n";
			outfile <<"counter,time_from_base,gps,time_to_fetch,gpos.lat,gpos.lon,gpos.alt,gpos.relative_alt,gpos.vx,gpos.vy,gpos.vz,gpos.hdg,lpos.x,lpos.y,lpos.z\n";
			outfile <<"counter,time_from_base,imu,att.roll,att.pitch,att.yaw,att.rollspeed,att.pitchspeed,att.yawspeed\n";
			
			//mavlink start
			cout << "Initialize MAVLINK" << endl;
			char *uart_name = (char *)settings.MAVLinkPort.c_str();
			int baudrate = settings.baudrate;
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
		high_resolution_clock::time_point t1, t2;
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
		
		if(settings.showCaptures) showWindows(settings);
		
		cout<< "Capturing start!" << endl;
		int time_diff;
		uint lastPosTime = 0, lastAttTime = 0;
		cout << "\nSet(#)  Timestamp \t\tImage_Fetch_time(ms)" << endl;
		bool cam_locks[3] = {false, false, false};
		bool *cam_lock_ptr = &cam_locks[0];
		
		bool gpsCycleBool = true;
		bool attCycleBool = false;
		while (ok) {
			
			if(settings.useMAVLinkForTrigger)
			{
				gpos = api->current_messages.global_position_int;
				lpos = api->current_messages.local_position_ned;
				att = api->current_messages.attitude;
				
				gpsCycleBool = gpos.time_boot_ms > lastPosTime;
				attCycleBool = att.time_boot_ms > lastAttTime;
			}
			else
			{
				//run cycle only when camera locks are off
				gpsCycleBool = !(cam_locks[0] || cam_locks[1] || cam_locks[2]);
			}
			
			if(gpsCycleBool && !cam_locks[0] && !cam_locks[1] && !cam_locks[2])		//run camera fetch only when all camera locks are false
			{
				//trigger cameras using GPIO pins
				if(settings.useGPIOPinsAsTrigger)
					triggerCameras();
				
				//update last GPS time from MAVLink
				if(settings.useMAVLinkForTrigger) 
					lastPosTime = gpos.time_boot_ms;
				
				//turn cycle flag off, with or without MAVLink usage
				gpsCycleBool = false;
				
				//cam1
				cam_Ind = 0;
				if(settings.use_cam_x[cam_Ind]) { idx = cam[cam_Ind]->grab(&img_frame, bytes_used); cam_locks[cam_Ind] = true; }
				
				t1 = high_resolution_clock::now();
				time_tag = system_clock::now() - t_base;
				n_time = duration_cast<millisecondTimeType> (time_tag);
				time_from_base = (uint64_t)n_time.count();
				
				if (img_frame || !settings.use_cam_x[cam_Ind]) {
					if(settings.use_cam_x[cam_Ind]) boost::thread thread_cam1(saveFetchedImage, cam_Ind, cam[cam_Ind], cam_lock_ptr, idx, settings, img_frame, counter, time_from_base);
						
					//cam2
					cam_Ind++;
					if(settings.use_cam_x[cam_Ind]) { idx = cam[cam_Ind]->grab(&img_frame, bytes_used); cam_locks[cam_Ind] = true; }
					if (img_frame || !settings.use_cam_x[cam_Ind]) {
						if(settings.use_cam_x[cam_Ind]) boost::thread thread_cam2(saveFetchedImage, cam_Ind, cam[cam_Ind], cam_lock_ptr, idx, settings, img_frame, counter, time_from_base);
							
						//cam3
						cam_Ind++;
						if(settings.use_cam_x[cam_Ind]) { idx = cam[cam_Ind]->grab(&img_frame, bytes_used); cam_locks[cam_Ind] = true; }
						if (img_frame || !settings.use_cam_x[cam_Ind]) {
							if(settings.use_cam_x[cam_Ind]) boost::thread thread_cam3(saveFetchedImage, cam_Ind, cam[cam_Ind], cam_lock_ptr, idx, settings, img_frame, counter, time_from_base);
							
							t2 = high_resolution_clock::now();
							time_span = t2 - t1;
							cout << counter<< "\t" << time_from_base << "\t" << ceil(time_span.count()) << endl;
							if(ceil(time_span.count()) > 3)
								cout << "Possible faulty start. Image fetch time > 3 ms!" << endl;
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
			
			//print if someday the camera locks actually come in use. Mostly will be of use at very high fps
			if(settings.useMAVLinkForTrigger && gpsCycleBool && (cam_locks[0] || cam_locks[1] || cam_locks[2]))
				cout << "Camera Locks Worked! Huraahhh!!! :D" << endl;
			
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

