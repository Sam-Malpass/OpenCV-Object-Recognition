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

Mat Preprocessor::translateDepth(Mat depth)
{
	return Mat();
}

Mat Preprocessor::thresholdImage(Mat depth, Mat grayscale)
{
	return Mat();
}

Contour_Data Preprocessor::findContours(Mat thresholded_image)
{
	return Contour_Data();
}
