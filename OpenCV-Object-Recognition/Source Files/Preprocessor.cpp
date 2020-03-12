#include "Preprocessor.h"

/* Constructor with no arguments */
Preprocessor::Preprocessor()
{
	// Empty, just needed to create an object
}

/* Takes an RGB image matrix and coverts it into a singular grayscale matrix */
Mat Preprocessor::toGrayscale(Mat rgb)
{
	// Create the matrix to hold the grayscale
	Mat grayscale;
	// use the RGB data to create the grayscale image
	cvtColor(rgb, grayscale, COLOR_RGB2GRAY);
	// Threshold the grayscale image
	threshold(grayscale, grayscale, 190, 255, THRESH_BINARY);
	// Return the now gray image
	return grayscale;
}

/* Translates a depth component matrix to be more inline with RGB */
Mat Preprocessor::translateDepth(Mat depth)
{
	// Create a copy of the depth Component
	Mat depthComp = depth;
	// Create a translation matrix that will be used to translate the depth component
	Mat translation = (Mat_<double>(2, 3) << 1, 0, -38, 0, 1, 25);
	// Apply the transformation to the depth component
	warpAffine(depth, depthComp, translation, depthComp.size(), INTER_LINEAR, BORDER_CONSTANT, 255);
	// THis pushed the background pixel values towards infinity (Making normalization more effective at separating the arm and the object)
	depthComp = depthComp + 158;
	// Return the translated component
	return depthComp;
}

Mat Preprocessor::thresholdImage(Mat depth, Mat grayscale)
{
	return Mat();
}

Contour_Data Preprocessor::findContours(Mat thresholded_image)
{
	return Contour_Data();
}
