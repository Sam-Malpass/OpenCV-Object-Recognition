#include "Classifiers.h"
#include "FileHandler.h"
#include "Preprocessor.h"
#include "Structures.h"
#include "ConfusionMatrix.h"
#include "Tests.h"
#include "freenect-playback-wrapper.h"
#include "OpenCV-Object-Recognition.h"

using namespace std;
using namespace cv;

void main(int argc, string args[])
{
	bool debug = false;

	// Declare object for the testing
	Tester tester;
	args = tester.test(1);
	argc = sizeof(args);

	// Declare variables for the application to run
	string dataPath;
	string labelPath;
	string modelPath;
	string kernel;
	bool mode;
	int set;
	// Code to interpret command line args
	if (argc < 5) {
		// Output error and return
		cout << "Invalid use of application. Correct use:" << endl << "OpenCV-Object-Recognition.exe <File Path to Data> <File Path for labels> <File Path for Model> <Kernel Type> <Mode>" << endl;
		return;
	}
	else {
		// Get the arguments
		dataPath = args[1];
		string set1 = "Set1";
		string set2 = "Set2";
		if (!(dataPath.find(set1) != string::npos) && !(dataPath.find(set2) != string::npos)) {
			cout << "Please use a valid Data folder here" << endl << "Must be either Set1 or Set2" << endl;
			return;
		}
		labelPath = args[2];
		modelPath = args[3];
		kernel = args[4];
		// Check kernel is valid
		if (!(kernel == "lin") && !(kernel == "rbf")) {
			// Output error and return
			cout << "Invalid kernel used. Kernel can be:" << endl << "lin\trbf" << endl;
			return;
		}
		// Check mode is valid
		if (args[5] == "-train") {
			mode = true;
		}
		else if (args[5] == "-test") {
			if (dataPath.find(set1) != string::npos) {
				set = 1;
			}
			else if (dataPath.find(set2) != string::npos) {
				set = 2;
			}
			else {
				cout << "Please use a valid Data folder" << endl << "Must be either Set1 or Set2" << endl;
				return;
			}
			mode = false;
		}
		else {
			// Output error and return
			cout << "Invalid mode selected. Modes can be:" << endl << "-train -test" << endl;
			return;
		}
	}

	// Declare the preprocessor to handle different stages of the process
	Preprocessor preprocessor;
	FileHandler fileReader;

	// Use the supplied Freenect wrapper
	FreenectPlaybackWrapper wrap(dataPath);
	// Current Frame will be stored in a struct to tie the RGB and Depth data together easily
	Image curr_frame;
	// This is a list of all object's individual frame lists
	vector<vector<Mat>> all_objects;
	// All frames of a given object will be stored here
	vector<Mat> current_object;
	// Read in labels from file
	vector<string> unique_labels = fileReader.readLabels(labelPath);

	// Check that the labels have been returned correctly
	if (!(unique_labels.size() > 0)) {
		// Output error and return
		cout << "No labels were found in the file, is the file correct?" << endl;
		return;
	}

	// Create the SVM Object
	SVM svm_object(mode, modelPath);

	// object_name holds the name of the object
	string object_name;
	// object_present is a flag for if there is an object in the scene
	bool object_present = false;
	// curr_object is an indexer for the labels
	int curr_object = 0;
	int numRight = 0;
	int numWrong = 0;

	// The key value represents a key pressed on the keyboard,
	// where 27 is the ESCAPE key
	char key = '0';

	// The status represents the current status of the Playback
	// wrapper. 
	//
	// A value of 0 represents that it has finished
	// playback.
	//
	// The status can by bitwise AND to determine if the RGB or
	// Depth image has been updated using the State enum.
	uint8_t status = 255;
	cout << "Program is now gathering all frame data please wait for all frames to run (This may take some time)" << endl;
	while (key != 27 && status != 0)
	{
		// Loads in the next frame of Kinect data into the
		// wrapper. Also pauses between the frames depending
		// on the time between frames.
		status = wrap.GetNextFrame();

		// Determine if RGB is updated, and grabs the image
		// if it has been updated
		if (status & State::UPDATED_RGB) {
			curr_frame.RGB_Component = wrap.RGB;
		}

		// Get the grayscale version of the frame using the RGB component
		Mat grayscale = preprocessor.toGrayscale(curr_frame.RGB_Component);

		// Determine if Depth is updated, and grabs the image
		// if it has been updated
		if (status & State::UPDATED_DEPTH) {
			// Get the depth component and translate it to be more inline with the RBG component of the frame
			// Process will increase the background intensity value towards infinity
			// This means that when normalization occurs, the difference between object and arm will then
			// be more pronounced
			curr_frame.Depth_Component = preprocessor.translateDepth(wrap.Depth);
			// If the debug is enabled
			if (debug) {
				// Output the translated depth to show what it looks like
				imshow("Translated Depth", curr_frame.Depth_Component);
			}
		}
		// Initialize a clone of the depth component
		Mat raw_depth = curr_frame.Depth_Component.clone();
		// Normalize the depth to better distinguish between arm and object
		normalize(curr_frame.Depth_Component, curr_frame.Depth_Component, 0, 255, NORM_MINMAX, CV_8UC1);

		// Threshold the image, the threshold value is determined in the data handler
		// The preprocessor will then use a "region of interest" approach to find regions with a mean value of 255 (meaning background).
		// Other areas will then be not uniform depth so it is assumed there is an object there
		// More information on how the function works available in the function body
		Mat thresholded_image = preprocessor.thresholdImage(curr_frame.Depth_Component, grayscale);

		// If the debug is enabled
		if (debug) {
			// Output the thresholded depth image
			imshow("Thresholded Depth", thresholded_image);
		}

		// This function will gather all the information about the contours of the object
		// It does this by finding the largest contour based on the area of the contour
		Contour_Data drawing_info = preprocessor.findContours(thresholded_image);

		// These matrices are used to generate the bounding contour, rectangles and circles
		Mat drawing = Mat::zeros(grayscale.size(), CV_8UC3);
		Mat masked = Mat::zeros(grayscale.size(), CV_8UC3);
		Mat tmp;

		bool code_run = false;
		// If there is at least one contour in the list
		if (drawing_info.contours.size() > 0) {
			//object_present_prev = true;
			code_run = true;
			// Define the colour of the bounds (in this case blue)
			Scalar col = Scalar(255, 0, 0);
			// If the rectangle has a bottom right corner of 590 or more it means
			// it is still entering the scene
			if (drawing_info.rectangle.area() > 0 && drawing_info.rectangle.br().x <= 590) {

				// REMOVE LATER MAYBE
				drawContours(masked, drawing_info.contours, 0, Scalar(255, 255, 255), FILLED);
				bitwise_and(curr_frame.RGB_Component, masked, masked);

				// Alert the console if an object has appeared and toggle the flag
				if (!object_present)
				{
					object_present = true;
				}


				// Declare matrices for the svmVector of the fram
				Mat standardSize;
				Mat svmVector;
				
				/*
				// THIS SECTION IS FOR RGB Component

				// Resize each frame/region of interest to a specific size
				resize(masked(drawing_info.rectangle).clone(), standardSize, SIZE, INTER_CUBIC);
				imshow("Trimmed Masked", masked(drawing_info.rectangle));
				// Reshape that matrix to a vector and push it into the svmMatrix
				svmVector.push_back(standardSize.reshape(1, 1));
				*/

				
				// Convert to HSV
				Mat hsv;
				cvtColor(masked(drawing_info.rectangle).clone(), hsv ,COLOR_BGR2HSV);
				resize(hsv, standardSize, SIZE, INTER_CUBIC);
				imshow("Trimmed Masked", masked(drawing_info.rectangle));
				// Reshape that matrix to a vector and push it into the svmMatrix
				svmVector.push_back(standardSize.reshape(1, 1));
				

				
				
				// Now we want to add the depth component to the vector
				cvtColor(raw_depth, raw_depth, COLOR_GRAY2BGR);
				// Resize the depth component in the region of interest
				resize(raw_depth(drawing_info.rectangle).clone(), standardSize, SIZE, INTER_CUBIC);
				imshow("Trimmed Depth", raw_depth(drawing_info.rectangle));
				// Reshape that matrix into a vector and push it into the svmMatrix
				svmVector.push_back(standardSize.reshape(1, 1));

				// Then reshape the two vectors in the matrix into a single vector
				svmVector = svmVector.reshape(1, 1);
				// Convert it  to the right data type for the SVM
				svmVector.convertTo(svmVector, CV_32FC1);

				// If in test mode
				if (mode == false) {

					// Get the class number
					int class_num = svm_object.predictSVM(svmVector);
					if (class_num != -1) {
						// Derive the object's name
						object_name = unique_labels[class_num];
						if (set == 1) {
							if (class_num == SET1_ORDER[curr_object]) {
								numRight += 1;
							}
							else {
								numWrong += 1;
							}
						}
						else {
							if (SET2_ORDER[curr_object] != -1) {
								if (class_num == SET2_ORDER[curr_object]) {
									numRight += 1;
								}
								else {
									numWrong += 1;
								}
							}
						}
					}
					else {
						object_name = "Unrecognized Object";
					}
				}
				// Otherwise we must be in train mode
				else {
					// Add the vector to the current object's sample
					current_object.push_back(svmVector);
				}

			}
			// These then draw the contours, rectangle and circle bounds to the drawing matrix
			drawContours(drawing, drawing_info.contours, 0, col, 1, 8, vector<Vec4i>(), 0, Point());
			rectangle(drawing, drawing_info.rectangle.tl(), drawing_info.rectangle.br(), col, 2, 8, 0);
			circle(drawing, drawing_info.center, drawing_info.radius, col, 2, 8, 0);
		}
		// If the object is present but the code has not been executed
		if (object_present && code_run == false) {
			// It means the object isn't actually present to a meaningful degree
			object_present = false;
			// Add the object's sample to the training set
			all_objects.push_back(current_object);
			// Clear the sample holder
			current_object.clear();
			// Change the object name
			object_name = "No Object";
			curr_object++ ;
		}
		// Clone the RGB_component
		Mat output = curr_frame.RGB_Component.clone();
		// Add the bounding box to it
		rectangle(output, drawing_info.rectangle.tl(), drawing_info.rectangle.br(), Scalar(0, 0, 255), 2, 8, 0);
		if (mode == false) {
			putText(output, object_name, Point2f(masked.rows / 16, 460), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 3);
		}
		// Show the RGB_component but with the box overlayed
		imshow("RGB", output);
		// If debug is enabled
		if (debug) {
			// Show the all bounds on an image
			imshow("Object", drawing);
			// Show the masked image
			imshow("Masked", masked);
		}
		// Check for keyboard input
		key = cv::waitKey(10);
	}
	if (mode == true) {
		cout << "Training of the SVM will now commence, please wait..." << endl;
		svm_object.trainSVM(kernel, all_objects);
		cout << "Training complete!" << endl;
	}
	else {
		double total = (double)numRight + (double)numWrong;
		double accuracy = (numRight / total);
		accuracy = accuracy * 100;
		cout << "Accuracy of Model: " << to_string(accuracy) << "%";
	}
	return;
}
