#ifndef DETECTOR_H
#define DETECTOR_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;
using namespace cv;
class Detector {
private:
	VideoCapture capture;
	CascadeClassifier face_cascade;
	Mat frame;

	void convertImage(Mat& result, int colorCode);

public:
	Detector();
	Detector(string cascade_name, int device_num, Mat image);
	Mat getFrame();
	vector<Rect> detectAndDisplay(Mat &result, int colorCode = CV_BGR2GRAY);
	void captureFrame();
};

#endif
