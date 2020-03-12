#include "Classifiers.h"

/* Finds the class with the smallest number of frames, then ensures that all the classes have that number of frames */
vector<vector<Mat>> SVM::balanceData(vector<vector<Mat>> inputs)
{
	// Creates a vector to store the balanced training data
	vector<vector<Mat>> balanced;
	// Declares the smallest sized class to be max int
	int smallest = INT_MAX;

	// For all class samples
	for (vector<Mat> object : inputs) {
		// Check if the sample size is less than the current smallest
		if (sizeof(object) < INT_MAX) {
			// Update the smallest sample size
			smallest = sizeof(object);
		}
	}

	// For all class samples
	for (vector<Mat> object : inputs) {
		// Declare a new vector to hold the balanced sample
		vector<Mat> balanced_object;
		// Declare a vector to track used entries (Sample without replacement)
		vector<int> used;
		// For the number of frames we want in the sample
		for (int i = 0; i < smallest; i++) {
			// Declare an int
			int rnd;
			do {
				// Find a random int in the range 0 to sample size
				rnd = rand() % smallest;
				// Check whether that element is in the list
			} while (find(used.begin(), used.end(), rnd) != used.end());
			balanced_object.push_back(object[rnd]);
			// Add the index to the list of used indices
			used.push_back(rnd);
		}
		// Push back the balanced class sample into the balanced set
		balanced.push_back(balanced_object);
		// Clear the object sample
		balanced_object.clear();
		// Clear the index tracker
		used.clear();
	}
	// Return the balanced data
	return balanced;
}

/* Shuffles the rows of the training data so that cross validation is actually worthwhile */
ShuffledData SVM::shuffleRows(Mat data, vector<int> labels)
{
	ShuffledData processed;
	// Declare a matrix to hold the shuffled data
	Mat shuffled;
	vector<int> shuffledLabels;
	// Declare a vector to hold the indices of the rows
	vector<int> indices;
	// For all rows
	for (int i = 0; i < data.rows; i++) {
		// Generate a row index
		indices.push_back(i);
	}

	// Shuffle the indices' order
	randShuffle(indices);

	// For all the rows in the data
	for (int i = 0; i < data.rows; i++) {
		// Add the row at the given random index to the shuffled data
		int index = indices[i];
		shuffled.push_back(data.row(index));
		shuffledLabels.push_back(labels[index]);
	}
	processed.trainingData = shuffled;
	processed.trainingLabels = shuffledLabels;
	// Return the shuffled data
	return processed;
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
