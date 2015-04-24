#include "stdafx.h"
#include "Recognizer.h"

Recognizer::Recognizer(){
	model = createLBPHFaceRecognizer(2, 14, 8, 8, 115.0);
}

Recognizer::Recognizer(Ptr<FaceRecognizer> model){
	this->model = model;
}

Ptr<FaceRecognizer> Recognizer::getModel(){
	return model;
}

void Recognizer::displayText(Person person, Point point, Mat frame) {
	putText(frame, person.getName(), Point(point.x, point.y - 20), FONT_HERSHEY_PLAIN, 1.5, CV_RGB(150, 0, 255), 2.0);
	putText(frame, person.getDescription(), point, FONT_HERSHEY_PLAIN, 1.5, CV_RGB(150, 0, 255), 2.0);
}

void Recognizer::trainImages(vector<Mat> images, vector<int> labels) {
	model->train(images, labels);
}

void Recognizer::prediction(Mat frame, vector<Rect> faces, Mat gray, Size size, vector<Person> persons) {

	Mat face_im;

	/*training images and the face images from frame must be same size
	therefore we must resize it*/
	for (size_t i = 0; i < faces.size(); i++) {
		//get each face as a rectangle
		Rect face_i = faces[i];

		//crop the rectangle and save it as an image face_im
		face_im = gray(face_i);
		//resize the image to be the same as training images
		cv::resize(face_im, face_im, size, 1.0, 1.0, INTER_CUBIC);

		//predict which image it belongs to
		int predict = model->predict(face_im);
		string text;
		//text = format("predicted class = %d", predict);

		//set the position of the predicted text on top of the square
		int pos_x = std::max(face_i.tl().x - 10, 0);
		int pos_y = std::max(face_i.tl().y - 10, 0);
		//putText(frame, text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0);
		for (Person person : persons) {
			if (person.getPID() == predict) {
				displayText(person, Point(pos_x, pos_y), frame);
			}
		}
	}
}

void Recognizer::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator){
	//open file
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;

	//read file line by line
	while (getline(file, line)) {
		stringstream liness(line);
		
		//save the text before separator(;) as path
		getline(liness, path, separator);

		//save the text after separator(;) as classlabel
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			//add image at path to images vector
			images.push_back(imread(path, 0));

			//add classlabel to labels vector
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}