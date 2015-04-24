#include "stdafx.h"
#include "Detector.h"

Detector::Detector() {
	string face_cascade_name = "D:\\My Document\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	capture.open(0);
	if (!capture.isOpened()){
		cerr << "Cannot load video capture device" << endl;
		throw std::exception("Error loading video capture device");
	}

	if (!face_cascade.load(face_cascade_name)) {
		cerr << "Error loading cascade xml. Please check path." << endl;
		throw std::exception("Error loading cascade file");
	}

	captureFrame();
}

Detector::Detector(string cascade_name, int device_num, Mat image){
	capture.open(device_num);
	if (!capture.isOpened()){
		cerr << "Cannot load video capture device" << endl;
		throw std::exception("Error loading video capture device");
	}

	if (!face_cascade.load(cascade_name)) {
		cerr << "Error loading cascade xml. Please check path." << endl;
		throw std::exception("Error loading cascade file");
	}

	frame = image;
}

Mat Detector::getFrame() {
	return frame;
}

void Detector::convertImage(Mat& result, int colorCode)
{
	cvtColor(frame, result, colorCode);
}

void Detector::captureFrame() {
	capture >> frame;
}

vector<Rect> Detector::detectAndDisplay(Mat& result, int colorCode){
	std::vector<Rect> faces;

	convertImage(result, colorCode);
	equalizeHist(result, result);

	// Detect faces
	const double SCALE = 1.1;
	const int MIN_NEIGHBORS = 2;
	const Size MIN_OBJECT_SIZE = Size(30, 30);
	face_cascade.detectMultiScale(result, faces, SCALE, MIN_NEIGHBORS, 0 | CASCADE_SCALE_IMAGE, MIN_OBJECT_SIZE);

	size_t ic = 0;

	for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

	{
		const int RECT_THICKNESS = 2;
		const int LINE_TYPE = 8;
		const int SHIFT_BITS = 0;
		rectangle(frame, faces[ic], Scalar(0, 255, 0), RECT_THICKNESS, LINE_TYPE, SHIFT_BITS);
	}

	return faces;
}
