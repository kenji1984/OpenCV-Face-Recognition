// FaceDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
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


string face_cascade_name = "D:\\My Document\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Face Detection";

//initialize face recognizer model
Ptr<FaceRecognizer> model = createLBPHFaceRecognizer(2, 12, 8, 8, 100.0);

// Detect faces in a frame based on haarcascade xml file and saves all the faces to vector<Rect> faces
vector<Rect> detectAndDisplay(Mat frame, Mat original_gray);

/*
read a csv file name line by line. Each line should consist of image path follow by a separator(;) followed by a label
saves images to a vector<Mat> images, and the label to vector<int> labels.
the images will be trained and compared to the frame from webcam
the labels should states ID of the person in the image.
*/
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');

void predict(Mat frame, vector<Rect> faces, Mat frame_gray, Size size);


int main()
{
	/*grab the images from a file (or database) to be compared to and recognized from*/

	//initialize vector of images and labels to hold images and labels from csv file
	vector<Mat> images;
	vector<int> labels;
	string face_filename = "D:\\My Document\\Github\\OpenCV-Face-Recognition\\FaceDetection\\faces.csv";

	//read csv file
	try {
		read_csv(face_filename, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << face_filename << "\". Reason: " << e.msg << endl;
		exit(1);
	}

	for (Mat image : images){
		if (image.empty()) {
			cerr << "image empty" << endl;
			exit(1);
		}
	}
	//get the height and width of the images to be trained
	int height = images[0].rows;
	int width = images[0].cols;
	Size size = Size(width, height);

	//train the face (what does it even mean???)
	model->train(images, labels);


	/*captures a frame from webcam, and save it to frame variable, then frame is compared to the images from above*/

	//load fascade from xml file (this is to detect if there's a face in the frame)
	if (!face_cascade.load(face_cascade_name)) {
		cout << "Error loading xml file" << endl;
		return -1;
	}

	//initialize video capture and frame to hold each capture frame
	VideoCapture capture;
	Mat frame;

	capture.open(0);
	if (!capture.isOpened()){
		cout << "Error opening webcam" << endl;
		return -1;
	}

	bool exit = false;
	bool predictMode = false;

	while (!exit) {
		vector<Rect> faces;
		Mat original_gray;

		//capture frame and convert to gray image
		capture >> frame;
		cvtColor(frame, original_gray, COLOR_BGR2GRAY);

		if (!frame.empty()){
			//detect faces from gray image and save to vector of faces (vector<Rect> faces)
			faces = detectAndDisplay(frame, original_gray);
		}
		else {
			cout << "No frame captured" << endl;
			break;
		}

		predict(frame, faces, original_gray, size);

		int c = waitKey(10);
		switch (char(c))
		{

		case 'c':
		case 'C':
			exit = true;
			break;

		case 's':
		case 'S': {
			Mat img;
			Mat face;
			img = imread("C:\\Users\\Kenji\\Documents\\Visual Studio 2013\\Projects\\FaceDetection\\FaceDetection\\laughing.jpg", 1);
			int height = img.rows;
			int width = img.cols;
			cv::resize(original_gray(faces[0]), face, Size(width, height), 1.0, 1.0, INTER_CUBIC);
			imwrite("image.jpg", face);
			break;
		}
		default:
			break;
		}

		imshow("face recognizer", frame);

	}
	return 0;
}


vector<Rect> detectAndDisplay(Mat frame, Mat frame_gray)
{
	std::vector<Rect> faces;
	Mat crop;
	Mat res;
	Mat gray;
	equalizeHist(frame_gray, frame_gray);

	// Detect faces
	const double SCALE = 1.1;
	const int MIN_NEIGHBORS = 2;
	const Size MIN_OBJECT_SIZE = Size(30, 30);
	face_cascade.detectMultiScale(frame_gray, faces, SCALE, MIN_NEIGHBORS, 0 | CASCADE_SCALE_IMAGE, MIN_OBJECT_SIZE);

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

void predict(Mat frame, vector<Rect> faces, Mat original_gray, Size size)
{
	Mat face_im;

	/*training images and the face images from frame must be same size
	therefore we must resize it*/
	for (size_t i = 0; i < faces.size(); i++) {
		//get each face as a rectangle
		Rect face_i = faces[i];
		//draw a rectangle around each face
		rectangle(frame, face_i, CV_RGB(0, 0, 255), 1);
		//crop the rectangle and save it as an image
		face_im = original_gray(face_i);
		//resize the image to be the same as training images
		cv::resize(face_im, face_im, size, 1.0, 1.0, INTER_CUBIC);
		//predict which image it belongs to
		int prediction = model->predict(face_im);
		string text;
		if (prediction == 1) {
			text = "Hoan normal";
		}
		else if (prediction == 2) {
			text = "Hoan laughing";
		}
		else text = "Unknown Person";
		int pos_x = std::max(face_i.tl().x - 10, 0);
		int pos_y = std::max(face_i.tl().y - 10, 0);
		putText(frame, text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 3.0, CV_RGB(0, 255, 0), 2.0);
		cout << prediction;
	}
	//cv::resize(face_im, face_im, size, 1.0, 1.0, INTER_CUBIC);
	//int prediction = model->predict(face_im);
	//cout << prediction;
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator) {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}
