#include "ConfusionMatrix.h"

/* Calculates the accuracy for the confusion matrix */
double Calculations::accuracy(Confusion_Matrix matrix)
{
	// Declare the total correct
	double total_correct = 0;
	// Declare the total predicted
	double total_predicted = 0;

	// Find the total correct -> Equivalent to the diagonal 
	for (int i = 0; i < matrix.rows[0].counts.size(); i++) {
		total_correct += matrix.rows[i].counts[i];
	}

	// Find the total predictions
	for (int i = 0; i < matrix.rows[0].counts.size(); i++) {
		for (int j = 0; j < matrix.rows.size(); j++) {
			total_predicted += matrix.rows[i].counts[j];
		}
	}

	// Return the accuracy as a percentage
	return ((total_correct / total_predicted) * 100);
}

double Calculations::precision(Confusion_Matrix matrix)
{
	double true_positive = 0;
	double false_positive = 0;

	// Find the true positive -> Equivalent to the diagonal 
	for (int i = 0; i < matrix.rows[0].counts.size(); i++) {
		true_positive += matrix.rows[i].counts[i];
	}

	// Find the false positives -> Equivalent to where it should have been something but was predicted the wrong class
	for (int i = 0; i < matrix.rows[0].counts.size(); i++) {
		for (int j = 0; j < matrix.rows.size(); j++) {
			if (i != j) {
				false_positive += matrix.rows[i].counts[j];
			}
		}
	}

	// Return the precision
	return (true_positive / (true_positive + false_positive));
}

double Calculations::recall(Confusion_Matrix matrix)
{
	double true_positive = 0;
	double false_negative = 0;
	return (true_positive / (true_positive + false_negative));
}

double Calculations::fmeasure(Confusion_Matrix matrix)
{
	// Get the precision using the dedicated function
	double precision = Calculations::precision(matrix);
	// Get the recall using the dedicated function
	double recall = Calculations::recall(matrix);
	// Return the F measure using the equation
	return (2 * ((precision * recall)/(precision + recall)));
}
