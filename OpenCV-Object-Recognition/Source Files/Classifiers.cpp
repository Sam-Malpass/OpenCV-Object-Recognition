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
Data SVM::shuffleRows(Mat data, vector<int> labels)
{
	Data processed;
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

#/* Trains an SVM using the specified kernel and training data */
void SVM::trainSVM(string kernel, vector<vector<Mat>> data)
{
	// Create the model
	model = ml::SVM::create();
	// Set the type to classifier
	model->setType(ml::SVM::C_SVC);
	// Set the termination criteria
	model->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 500, 1e-6));
	// Declare the training labels list
	vector<int> training_labels;
	// Declare the input training data
	Mat training_data;
	// Declare the dummy label
	int label = 0;
	// Balance the input data
	cout << "Balancing Data..." << endl;
	data = balanceData(data);
	cout << "Generating Input Matrix..." << endl;
	// Setup training labels - For all class specific frames
	for (vector<Mat> object : data) {
		// For all frames in the sample
		for (Mat m : object) {
			// Add the frame to the training data matrix
			training_data.push_back(m);
			// Push the current label onto the list
			training_labels.push_back(label);
		}
		// Increment the label (Coincides with object change)
		label++;
	}
	// Output current status
	cout << "Shuffling the data..." << endl;
	// Shuffle the training data
	Data shuffled = shuffleRows(training_data, training_labels);
	// Output current status
	cout << "Shuffling completed!" << endl;
	// Grab the shuffled data and labels
	training_data = shuffled.trainingData;
	training_labels = shuffled.trainingLabels;
	// Output current status
	cout << "Starting optimization/training process. This may take some time..." << endl;
	// Depending on the desired kernel
	if (kernel == "lin") {
		// A linear kernel
		model->setKernel(ml::SVM::LINEAR);
		// Train and attempt to optimize hyperparameters
		model->trainAuto(training_data, ml::ROW_SAMPLE, training_labels, 10, ml::SVM::getDefaultGridPtr(ml::SVM::C), ml::SVM::getDefaultGridPtr(ml::SVM::GAMMA), ml::SVM::getDefaultGridPtr(ml::SVM::P), cv::ml::SVM::getDefaultGridPtr(ml::SVM::NU), ml::SVM::getDefaultGridPtr(ml::SVM::COEF), ml::SVM::getDefaultGridPtr(ml::SVM::DEGREE));
	}
	else if (kernel == "rbf") {
		// An RBF kernel
		model->setKernel(ml::SVM::RBF);
		// Train and attempt to optimize hyperparameters
		model->trainAuto(training_data, ml::ROW_SAMPLE, training_labels, 10, ml::SVM::getDefaultGridPtr(ml::SVM::C), ml::SVM::getDefaultGridPtr(ml::SVM::GAMMA));
	}
	// Save the model to the file
	fileHandler.saveModel(model, fileName);
	// Output current status
	cout << "Process complete, you may now close the application" << endl;
}

/* Predicts the class of the given data point */
int SVM::predictSVM(Mat data)
{
	// Declare the class number
	int class_number = -1;
	// Declare a matrix to get the resuts of the SVM
	Mat result;
	// Get the full outputs of the SVM
	model->predict(data, result, ml::StatModel::Flags::RAW_OUTPUT);
	// Get the classification of the SVM
	class_number = model->predict(data);
	// Get the distance
	float dist = result.at<float>(0, 0);
	// Get then the confidence that the class is correct
	float confidence = (1.0 / (1.0 + exp(-dist)));
	// If the confidence is greater than the threshold
	if (confidence > CONF_THRESHOLD) {
		// Return the new class number
		return class_number;
	}
	// Otherwise
	else {
		// Return -1 (Unrecognized)
		return -1;
	}
}
