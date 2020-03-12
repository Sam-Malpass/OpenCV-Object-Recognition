#pragma once
#include <string>

#define DESKTOP_TRAIN "C:/Users/Sam/source/repos/OpenCV-Object-Recognition/Data/Set1/Set1/"
#define DESKTOP_TEST "C:/Users/Sam/source/repos/OpenCV-Object-Recognition/Data/Set2/Set2/"
#define LAPTOP_TRAIN "C:/Users/sam/source/repos/OpenCV-Object-Recognition/Data/Set1/Set1/"
#define LAPTOP_TEST "C:/Users/sam/source/repos/OpenCV-Object-Recognition/Data/Set2/Set2/"
#define LAPTOP_LABELS "C:/Users/sam/source/repos/OpenCV-Object-Recognition/Data/Set1Labels.txt"
#define DESKTOP_LABELS "C:/Users/Sam/source/repos/OpenCV-Object-Recognition/Data/Set1Labels.txt"

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
	// Generates command line arguments for creating a confusion matrix on my laptop
	string* confusion_laptop(); //5
	// Generates command line arguments for creating a confusion matrix on my desktop
	string* confusion_desktop(); //6
public:
	string* test(int test);
};