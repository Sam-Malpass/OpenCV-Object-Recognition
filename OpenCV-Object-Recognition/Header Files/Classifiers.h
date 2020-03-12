#pragma once
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/* SVM for the handling of training and testing of support vector machine(s) */
class SVM {
	// The actual SVM - Needed in test mode
	Ptr<ml::SVM> model;
	// balanceData organizes the datasets so that each object has the same number of frames
	vector<vector<Mat>> balanceData(vector<vector<Mat>> inputs);
	// shuffleRows shuffles the training data and the corresponding training labels in the same manner
	ShuffledData shuffleRows(Mat data, vector<int> labels);
public:
	// Constructor with argument - for test mode
	SVM(bool mode);
	// Constructor with no arguments - for train mode
	SVM();
	// trainSVM will train an SVM classifier on the data using a given kernel for the SVM
	void trainSVM(string kernel, vector<vector<Mat>> data);
	// predictSVM will predict the class of the given data point
	int predictSVM(Mat data);
};