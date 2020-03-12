#pragma once
#include <opencv2/opencv.hpp>
#include "Structures.h"

#define THRESHOLD 100
#define SIZE Size(64,64)

using namespace cv;

/* Preprocessor class handles the preprocessing of frames so they can be used in conjunction with the SVM */
class Preprocessor {
public:
	// Constructor
	Preprocessor();
	// Converts the RGB component into grayscale
	Mat toGrayscale(Mat rgb);
	// Translates the Depth component to be more in-line with the RGB component
	Mat translateDepth(Mat depth);
	// Thresholds the image
	Mat thresholdImage(Mat depth, Mat grayscale);
	// Find the contours in the image - assume largest area is the object
	Contour_Data findContours(Mat thresholded_image);
};