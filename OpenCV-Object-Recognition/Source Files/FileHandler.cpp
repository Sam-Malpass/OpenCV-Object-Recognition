#include "FileHandler.h"

/* Takes a file path as argument then reads and returns a list of class labels */
vector<string> FileReader::readLabels(string filePath)
{
	// Create the list
	vector<string> labels;
	// Open the input file stream 
	ifstream labelFile(filePath);
	// If the file opened successfully
	if (labelFile.is_open())
	{
		// Dummy variable to hole the line
		string line;
		// Read the next line of the file, until no lines left
		while (getline(labelFile, line))
		{
			// Add the line to the list of labels
			labels.push_back(line);
		}
	}
	// Otherwise there has been an error
	else {
		// Error and return
		cerr << "Failed to open labels file." << endl;
		return labels;
	}
	return labels;
}

Ptr<ml::SVM> FileReader::loadModel(string filePath)
{
	return Ptr<ml::SVM>();
}

void FileReader::saveModel(Ptr<ml::SVM> model, string filePath)
{
}
