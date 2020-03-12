#pragma once
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/* FileReader class is used for reading class labels from a given file and saving/loading models from a given file */
class FileReader {
public:
	// readLabels takes a filePath and then reads all the class labels from the file
	vector<string> readLabels(string filePath);
	// loadModel takes a filePath and loads the SVM
	Ptr<ml::SVM> loadModel(string filePath);
	// saveModel takes a model and a filePath and saves the model to the file
	void saveModel(Ptr<ml::SVM> model, string filePath);
};