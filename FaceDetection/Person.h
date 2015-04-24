#ifndef PERSON_H
#define PERSON_H

#include "opencv2/imgproc/imgproc.hpp"

#include <string>
using namespace std;
using namespace cv;

class Person {
private:
	int pid;
	string name;
	string description;
	Mat image;

public:
	/*
	Person(int id, string nm, string desc, Mat img) : pid(id), name(nm), description(desc), image(img)
	{}

	void setName(string name) { this->name = name; }
	void setDescription(string desc) { description = desc; }
	string getName() { return name; }
	string getDescription() { return description; }
	Mat getImage() { return image; }
	int getPID() { return pid; }
	*/
	Person(int id, string nm, string desc, Mat img);
	void setName(string name);
	void setDescription(string desc);
	string getName();
	string getDescription();
	Mat getImage();
	int getPID();
};

#endif