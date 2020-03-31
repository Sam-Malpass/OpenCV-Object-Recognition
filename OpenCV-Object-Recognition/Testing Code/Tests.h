#pragma once
#include <string>

#define DESKTOP_TRAIN "C:/Users/Sam/source/repos/OpenCV-Object-Recognition/Data/Set1/Set1/"
#define DESKTOP_TEST "C:/Users/Sam/source/repos/OpenCV-Object-Recognition/Data/Set2/Set2/"
#define LAPTOP_TRAIN "C:/Users/sam/source/repos/OpenCV-Object-Recognition/Data/Set1/Set1/"
#define LAPTOP_TEST "C:/Users/sam/source/repos/OpenCV-Object-Recognition/Data/Set2/Set2/"
#define LAPTOP_LABELS "C:/Users/sam/source/repos/OpenCV-Object-Recognition/Data/Set1Labels.txt"
#define DESKTOP_LABELS "C:/Users/Sam/source/repos/OpenCV-Object-Recognition/Data/Set1Labels.txt"

const int SET1_ORDER[14] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };
const int SET2_ORDER[15] = { 6,1,13,0,7,2,12,-1,9,10,3,5,4,8,4 };

using namespace std;

/* Tester class has access to argument generating functions */
class Tester {
	// Generates command line arguments for training on my laptop
	string* train_laptop(); //1
	// Generates command line arguments for testing on my laptop
	string* test_laptop(); //2
	// Generates command line arguments for training on my desktop
	string* train_desktop(); //3
	// Generates command line arguments for testing on my desktop
	string* test_desktop(); //4
public:
	string* test(int test);
};