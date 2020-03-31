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

/* Calculate the precisions for the class */
vector<double> Calculations::precision(Confusion_Matrix matrix)
{
	vector<double> precisions;
	// Temp variables
	int index = 0;
	double true_positives = 0;
	double false_positives = 0;
	double precision = 0;
	// For all rows
	for (Confusion_Matrix_Row row : matrix.rows) {
		// Reset the variables
		true_positives = 0;
		false_positives = 0;
		// For all the counts
		for (int i = 0; i < row.counts.size(); i++) {
			// If the index is equal to i
			if (i == index) {
				// Increase true positives
				true_positives += row.counts[i];
			}
			// Otherwise
			else {
				// Increase false positives
				false_positives += row.counts[i];
			}
		}
		// Calculate the precision
		precision = (true_positives / (true_positives + false_positives));
		// Add to the list
		precisions.push_back(precision);
		// Increment the index
		index++;
	}

	// Return the precision
	return precisions;
}

/* Calculate the recalls of each class */
vector<double> Calculations::recall(Confusion_Matrix matrix)
{
	// Declare the list
	vector<double> recalls;
	// Temp variables
	double true_positive = 0;
	double false_negative = 0;
	double recall;
	// For all columns
	for (int j = 0; j < matrix.rows[0].counts.size(); j++) {
		true_positive = 0;
		false_negative = 0;
		// For all rows
		for (int i = 0; i < matrix.rows.size(); i++) {
			// If the row number equals the column number
			if (i == j) {
				// Add to true positives
				true_positive += matrix.rows[i].counts[j];
			}
			// Otherwise
			else {
				// Add to false negatives
				false_negative += matrix.rows[i].counts[j];
			}
		}
		// Calculate the recall
		recall = (true_positive / (true_positive + false_negative));
		// Push it to the list
		recalls.push_back(recall);
	}
	// Return the list
	return recalls;
}

/* Calculates the macro F1 score */
double Calculations::fmeasure(Confusion_Matrix matrix)
{
	double final_f1 = 0;
	// Get the precisions using the dedicated function
	vector<double> precisions = Calculations::precision(matrix);
	// Get the recalls using the dedicated function
	vector<double> recalls = Calculations::recall(matrix);
	
	// Declare scores
	vector<double> scores;
	// Temp variable
	double score;
	// For all the precisions and recalls
	for (int i = 0; i < precisions.size(); i++) {
		// Calculate the score
		score = 2 * ((precisions[i] * recalls[i]) / (precisions[i] + recalls[i]));
		// Append to the list
		scores.push_back(score);
	}

	// Temp variable
	double macro = 0;
	// For all scores
	for (double d : scores) {
		// Add together
		macro += d;
	}
	// Calculate the final F1 score
	final_f1 = (macro / scores.size());

	// Return the F measure
	return final_f1;
}
