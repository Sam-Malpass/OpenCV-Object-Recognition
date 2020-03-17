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

/* Finds the contours, rectangle and circle bounds of the thresholded image */
Contour_Data Preprocessor::findContours(Mat thresholded_image)
{
	// Declare a list of contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// Use the OpenCV variant to find all the contours
	cv::findContours(thresholded_image, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	// Dummy variables to help us find the largest contour
	// In theory this will be the object
	int largest_contour = -1;
	double area = 0;

	// For all the contorus in the list
	for (int i = 0; i < contours.size(); i++) {
		// Set a temporary variable to the area of the contour
		double tmp = contourArea(contours[i], false);
		// If that temporary variable is then greater than the area of the contour
		if (tmp > area) {
			// Set the area tracking variable to the temporary one
			area = tmp;
			// Set the largest contour to be the index of the contour
			largest_contour = i;
		}
	}

	// Now we need to approximate the contours into polygons
	// as well as define the bounding rectangle and circle
	vector<vector<Point>> polygon(contours.size());
	Rect bound;
	Point2f center;
	float rad = 0;

	// If there are contours and we successfully found a largest contour
	if (contours.size() > 0 && largest_contour != -1) {
		// Approximate the polygon for the largest contour
		// Basically the outline of the object
		approxPolyDP(Mat(contours[largest_contour]), polygon[0], 3, true);
		// Create the rectangle using said polygon
		bound = boundingRect(Mat(polygon[0]));
		// Create the smallest circle that encapsulates the polygon
		minEnclosingCircle((Mat)polygon[0], center, rad);

		// Remove noise
		if (bound.size().width < 100 && (bound.size().height < 40 || bound.br().x > 590 || (bound.size().height < 100 && bound.br().y > 440))) {
			bound = Rect(0, 0, 0, 0);
			polygon.clear();
			center = Point2f(0, 0);
			rad = 0;
		}
	}

	// Then we convert the found variables into the struct
	Contour_Data packet;
	packet.contours = polygon;
	packet.rectangle = bound;
	packet.center = center;
	packet.radius = rad;
	// Return the struct
	return packet;
}
