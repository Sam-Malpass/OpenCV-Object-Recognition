#include "Tests.h"

/* Creates the arguments for training on my laptop */
string* Tester::train_laptop()
{
	// Declare the list
	string* arguments = new string[5];
	// Add the arguments
	arguments[0] = "ProgName";
	arguments[1] = LAPTOP_TRAIN;
	arguments[2] = "SVM.xml";
	arguments[3] = "lin";
	arguments[4] = "-train";
	// Return the arguments
	return arguments;
}

/* Creates the arguments for testing on my laptop */
string* Tester::test_laptop()
{
	// Declare the list
	string* arguments = new string[3];
	// Add the arguments
	arguments[0] = "ProgName";
	arguments[1] = LAPTOP_TEST;
	arguments[2] = "SVM.xml";
	arguments[3] = "lin";
	arguments[4] = "-test";
	// Return the arguments
	return arguments;
}

/* Creates the arguments for training on my desktop */
string* Tester::train_desktop()
{
	// Declare the list
	string* arguments = new string[3];
	// Add the arguments
	arguments[0] = "ProgName";
	arguments[1] = DESKTOP_TRAIN;
	arguments[2] = "SVM.xml";
	arguments[3] = "lin";
	arguments[4] = "-train";
	// Return the arguments
	return arguments;
}

/* Creates the arguments for testing on my desktop */
string* Tester::test_desktop()
{
	// Declare the list
	string* arguments = new string[3];
	// Add the arguments
	arguments[0] = "ProgName";
	arguments[1] = DESKTOP_TEST;
	arguments[2] = "SVM.xml";
	arguments[3] = "lin";
	arguments[4] = "-test";
	// Return the arguments
	return arguments;
}


string* Tester::confusion_laptop()
{
	// Declare the list
	string* arguments = new string[3];
	// Add the arguments
	arguments[0] = "ProgName";
	arguments[1] = LAPTOP_TRAIN;
	arguments[2] = "SVM.xml";
	arguments[3] = "lin";
	arguments[4] = "-test";
	// Return the arguments
	return arguments;
}

string* Tester::confusion_desktop()
{
	// Declare the list
	string* arguments = new string[3];
	// Add the arguments
	arguments[0] = "ProgName";
	arguments[1] = DESKTOP_TRAIN;
	arguments[2] = "SVM.xml";
	arguments[3] = "lin";
	arguments[4] = "-test";
	// Return the arguments
	return arguments;
}

/* Takes the integer argument and returns the corresponding arguments */
string* Tester::test(int test)
{
	// Test the variable
	switch (test) {
	case 1:
		return train_laptop();
	case 2:
		return test_laptop();
	case 3:
		return train_desktop();
	case 4:
		return test_desktop();
	case 5:
		return confusion_laptop();
	case 6:
		return confusion_desktop();
	default:
		return test_laptop();
	}
}
