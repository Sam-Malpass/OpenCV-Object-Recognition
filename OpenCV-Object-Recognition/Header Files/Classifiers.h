#pragma once
#include <opencv2/opencv.hpp>
#include "FileHandler.h"
#include "Structures.h"

#define CONF_THRESHOLD 0.3

using namespace std;
using namespace cv;

/* SVM for the handling of training and testing of support vector machine(s) */
class SVM {
	// To allow for the saving and loading of models
	FileHandler fileHandler;
	// Holds the location of the file to save to or load from
	string fileName;
	// The actual SVM - Needed in test mode
	Ptr<ml::SVM> model;
	vector<vector<Mat>> testSet;
	// balanceData organizes the datasets so that each object has the same number of frames
	vector<vector<Mat>> balanceData(vector<vector<Mat>> inputs);
	vector<vector<Mat>> removeTest(vector<vector<Mat>> inputs);
	void afterTest();
	// shuffleRows shuffles the training data and the corresponding training labels in the same manner
	Data shuffleRows(Mat data, vector<int> labels);
public:
	// Constructor with argument - determines train or test mode
	SVM(bool mode, string filePath);
	// trainSVM will train an SVM classifier on the data using a given kernel for the SVM
	void trainSVM(string kernel, vector<vector<Mat>> data);
	// predictSVM will predict the class of the given data point
	int predictSVM(Mat data);
};