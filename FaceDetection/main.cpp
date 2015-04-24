#include "stdafx.h"
#include "Recognizer.h"
#include "Detector.h"
#include "Person.h"

int main()
{
	/*grab the images from a file (or database) to be compared to and recognized from*/

	//initialize vector of images and labels to hold images and labels from csv file
	vector<Mat> images;
	vector<int> labels;
	string face_filename = "D:\\My Document\\Github\\OpenCV-Face-Recognition\\FaceDetection\\faces.csv";
	Recognizer recognizer = Recognizer();
	Detector detector = Detector();

	vector<Person> persons;
	Mat image = imread("D:\\My Document\\Github\\OpenCV-Face-Recognition\\FaceDetection\\image.jpg", 0);
	persons.push_back(Person(1, "Kenji", "Sexy", image));
	image = imread("D:\\My Document\\Github\\OpenCV-Face-Recognition\\FaceDetection\\laughing.jpg", 0);
	persons.push_back(Person(2, "Kenji", "Laughing", image));

	/*
	//read csv file
	try {
		Recognizer::read_csv(face_filename, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << face_filename << "\". Reason: " << e.msg << endl;
		exit(1);
	}
	*/
	images.push_back(persons[0].getImage());
	labels.push_back(persons[0].getPID());
	images.push_back(persons[1].getImage());
	labels.push_back(persons[1].getPID());

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

	recognizer.trainImages(images, labels);

	bool exit = false;
	string filename = "image";
	string color_img_name = "color";
	int file_number = 0;

	while (!exit) {
		vector<Rect> faces;
		Mat gray;

		//capture frame and convert to gray image
		detector.captureFrame();

		if (!detector.getFrame().empty()){
			//detect faces from gray image and save to vector of faces (vector<Rect> faces)
			faces = detector.detectAndDisplay(gray, CV_BGR2GRAY);
		}
		else {
			cout << "No frame captured" << endl;
			break;
		}

		recognizer.prediction(detector.getFrame(), faces, gray, size, persons);

		int c = waitKey(10);
		switch (char(c))
		{

		case 'c':
		case 'C':
			exit = true;
			break;

		case 'f':
		case 'F': {
			Mat img;
			Mat face;

			//resize. make sure all images taken are the same size
			cv::resize(gray(faces[0]), face, size, 1.0, 1.0, INTER_CUBIC);
			
			//save face as
			imwrite(filename + to_string(file_number++) + ".jpg", face);
			break;
		}

		case 's':
		case 'S':
			//take full screen color image
			imwrite(color_img_name + to_string(file_number++) + ".jpg", detector.getFrame());
			break;

		default:
			break;
		}

		imshow("face recognizer", detector.getFrame());
	}
	return 0;
}
