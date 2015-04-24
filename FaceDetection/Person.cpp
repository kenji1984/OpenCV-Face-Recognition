#include "stdafx.h"
#include "Person.h"

Person::Person(int id, string nm, string desc, Mat img){
	pid = id;
	name = nm;
	description = desc;
	image = img;
}

void Person::setName(string nm){
	name = nm;
}
void Person::setDescription(string desc){
	description = desc;
}
string Person::getName(){
	return name;
}
string Person::getDescription() {
	return description;
}
Mat Person::getImage(){
	return image;
}
int Person::getPID(){
	return pid;
}