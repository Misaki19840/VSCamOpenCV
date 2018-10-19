#ifndef ARMARKER_DETECTION
#define ARMARKER_DETECTION

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <time.h>
#include <iostream>
#include "Parameters.h"

using namespace std;
using namespace cv;

void armarkerDetection_main();
void armarkerDetection_main_game01();
void armarkerCreation();

class PointerSet {
public:
	int point[255] = { 0 };
	Scalar color[255] = { Scalar(255, 0, 0)  };

	PointerSet(void) {
		color[0] = Scalar(255, 0, 0); // blue
		color[1] = Scalar(0, 255, 0); // green
		color[2] = Scalar(0, 0, 255); // red
		color[3] = Scalar(255, 255, 0);
		color[4] = Scalar(255, 0, 255);
		color[5] = Scalar(0, 255, 255);
	}

	void setPoint(int pointerId, int val) {
		point[pointerId] += val;
	}
};

#define NUMOBJ 4

class Targetset {
public:
	int imgwidth;
	int imgheight;
	vector<int> ids;
	vector<Point2d> posset;
	vector<clock_t> tmstamps;
	float createObjInterval;
	float tgtInterval_mean;
	float probNewObj;
	clock_t  prvtime;
	float objTimer;
	int PointId[NUMOBJ] = { 1, 1, -10, 3 };
	String Imgpath[NUMOBJ] = {
		"./image/fruit_ap_50.png",
		"./image/fruit_ap_50.png",
		"./image/fruit_apBlack_50.png",
		"./image/fruit_laf_50.png"
	};
	Mat imgs[NUMOBJ];

	int count_dgb;

	Targetset(int imgwidth_, int imgheight_) {
		imgwidth = imgwidth_;
		imgheight = imgheight_;
		createObjInterval = 1.5;
		tgtInterval_mean = 5;
		probNewObj = 0;
		prvtime = clock();
		objTimer = 3;

		for (int i = 0; i < NUMOBJ; i++) {
			imgs[i] = imread(Imgpath[i], IMREAD_UNCHANGED);
			//cout << "imgs[i].imgs[i].channels" << imgs[i].channels() << endl;
			//vector<cv::Mat> planes;
			//cv::split(imgs[i], planes);
			//cv::imwrite(Imgpath[i] + "ch00.png", planes[0]);
			//cv::imwrite(Imgpath[i] + "ch01.png", planes[1]);
			//cv::imwrite(Imgpath[i] + "ch02.png", planes[2]);
			//cv::imwrite(Imgpath[i] + "ch03.png", planes[3]);
			if (imgs[i].empty()) {
				cout << "cannot read target image. name = " << Imgpath[i];
				exit(0);
			}
		}

		count_dgb = 0;
	}

	void update(void) {
		clock_t currenttime = clock();
		float interval = (float)(currenttime - prvtime) / CLOCKS_PER_SEC;
		float probNewObj = (float)((1 / sqrt(2 * 3.14)) * exp(-(interval - tgtInterval_mean)* (interval - tgtInterval_mean) / 2));

		//cout << "currenttime, prvtime = " << currenttime << ", " << prvtime << endl;
		//cout << "interval, createObjInterval = " << interval << ", " << createObjInterval << endl;
		//cout << "object:" << endl;
		//for (int i = 0; i < ids.size(); i++) {
		//	cout << "[i] " << i << endl;
		//	cout << "[id] " << ids[i] << endl;
		//	cout << "[posset] " << posset << endl;
		//	cout << "[tmstamps] " << tmstamps[i] << endl;
		//}

		count_dgb++;
		//cout << "count_dgb: " << count_dgb << endl;

		// Create new object
		if (interval > createObjInterval) {
			//cout << "interval, createObjInterval = " << interval << ", " << createObjInterval << endl;
			int newid = rand() % NUMOBJ;
			int newposx = rand() % ( imgwidth - WINDOW_EDGE_X * 2 ) + WINDOW_EDGE_X;
			int newposy = rand() % ( imgheight - WINDOW_EDGE_Y * 2) + WINDOW_EDGE_Y;

			ids.push_back(newid);
			posset.push_back(Point2d(newposx, newposy));
			tmstamps.push_back(currenttime);

			prvtime = currenttime;
		}

		// update object
			vector<int> iderace;
			for (int i = 0; i < ids.size() ; i++) {
				float time = (float)(currenttime - tmstamps[i]) / CLOCKS_PER_SEC;
				//cout << "time, currenttime, tmstamps[i] = " << time << ", " << currenttime << ", " << tmstamps[i]  << endl;
				if (time > objTimer) {
					//cout << "time, objTimer = " << time << ", " << objTimer << endl;
					iderace.push_back(i);
				}
			}
			
			deleteTarget(iderace);
	}

	void deleteTarget(vector<int>& iderace) {
		//for (int i = 0; i < iderace.size(); i++) {
		//	ids.erase(ids.begin() + iderace[i]);
		//	posset.erase(posset.begin() + i);
		//	tmstamps.erase(tmstamps.begin() + i);
		//}
		for (int i = 0; i < iderace.size(); i++) {
			if (iderace[i] > 0) {
				ids.erase(ids.begin() + i);
				posset.erase(posset.begin() + i);
				tmstamps.erase(tmstamps.begin() + i);
			}
		}
	}
};

#endif