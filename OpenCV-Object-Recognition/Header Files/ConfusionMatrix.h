#pragma once
#include<vector>
#include<string>

using namespace std;

/* Structure for the row of the confusion matrix */
struct Confusion_Matrix_Row {
	// The object number being tested
	string object_no;
	// The counts for the different classes
	vector<int> counts;
};

/* Structure for the actual matrix */
struct Confusion_Matrix {
	// A list of confusion matrix rows
	vector<Confusion_Matrix_Row> rows;
};

class Calculations {
	vector<double> precision(Confusion_Matrix matrix);
	vector<double> recall(Confusion_Matrix matrix);
public:
	double accuracy(Confusion_Matrix matrix);
	double fmeasure(Confusion_Matrix matrix);
};