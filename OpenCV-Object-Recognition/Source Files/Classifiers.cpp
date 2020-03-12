#include "Classifiers.h"

vector<vector<Mat>> SVM::balanceData(vector<vector<Mat>> inputs)
{
	return vector<vector<Mat>>();
}

ShuffledData SVM::shuffleRows(Mat data, vector<int> labels)
{
	return ShuffledData();
}

/* Constructor that takes mode and filePath */
SVM::SVM(bool mode, string filePath)
{
	// Save the filePath
	fileName = filePath;
	// If in test mode
	if (mode == false) {
		// Output status
		cout << "Attempting to load model..." << endl;
		// Load the model from the given file
		model = fileHandler.loadModel(filePath);
		// Output success
		cout << "Model loaded successfully!" << endl;
	}
}

void SVM::trainSVM(string kernel, vector<vector<Mat>> data)
{
}

int SVM::predictSVM(Mat data)
{
	return 0;
}
