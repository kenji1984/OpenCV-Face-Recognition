#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Person.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;
using namespace cv;

class Recognizer {
private:
	Ptr<FaceRecognizer> model;
	void displayText(Person person, Point point, Mat frame);

public:
	Recognizer();
	Recognizer(Ptr<FaceRecognizer> model);
	Ptr<FaceRecognizer> getModel();

	/*
	input: vector of images and labels
	output: add images to model to be used for recognition
	*/
	void trainImages(vector<Mat> images, vector<int> labels);

	/*
	input: vector of faces. Note: frame, gray, and size are just for displaying square around the face
	output: return the label of the training image that fit the image from faces
	*/
	void prediction(Mat frame, vector<Rect> faces, Mat gray, Size size, vector<Person> persons);

	/*
	read a csv file name line by line. Each line should consist of image path follow by a separator(;) followed by a label
	saves images to a vector<Mat> images, and the label to vector<int> labels.
	the images will be trained and compared to the frame from webcam
	the labels should states ID of the person in the image.
	*/
	static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');
};

#endif