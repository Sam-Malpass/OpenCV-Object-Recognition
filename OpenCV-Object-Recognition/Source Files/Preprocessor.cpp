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

/* Thresholds an image and finds the object by using many small regions of interests and blurring the
	areas that are not background together (Providing the margin between regions is small enough) */
Mat Preprocessor::thresholdImage(Mat depth, Mat grayscale)
{
	// Declare an image to use for the thresholded image
	Mat image;
	// Threshold the depth component using the predefined threshold
	threshold(depth, image, THRESHOLD, 255, THRESH_BINARY_INV);

	// Create a region of interest that removed the last 9 pizels of the depth component
	// This is infinite because the Kinect uses a correlation window 9 pixels wide
	// So in order to get an accurate mean, this needs to be removed
	Rect region(0, 0, image.size().width - 9, image.size().height);
	Mat region_image = image(region);

	// Then if the mean is 255, we know the depth is uniform (so no object)
	if (mean(region_image)[0] == 255) {
		// We set it to black, making it really obvious in the output where the object is
		image = Mat::zeros(image.rows, image.cols, CV_8UC1);
	}

	// Then we clean up th thresholded image
	bitwise_and(image, grayscale, grayscale);
	bitwise_or(image, grayscale, image);

	// Next will use dilation on the thresholded image to joind regions that are separated by a small margin
	// This is done through blurring
	// This should result in the small regions that make up the object being blurred together
	// then show the object shape as a whole
	int blur_constant = 4;
	// Use an elliptic element for the blurring/dilation
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * blur_constant + 1, 2 * blur_constant + 1), Point(blur_constant, blur_constant));
	// Apply the process
	dilate(image, image, element);
	// Return the image
	return image;
}

Contour_Data Preprocessor::findContours(Mat thresholded_image)
{
	return Contour_Data();
}
