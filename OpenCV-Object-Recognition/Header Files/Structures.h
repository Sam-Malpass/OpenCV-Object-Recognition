#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/* Structure for storing the image data of the frames, keeping both the RGB Components and the Depth Components */
struct Image {
	// The RGB component of the frame
	Mat RGB_Component;
	// The Depth component of the frame
	Mat Depth_Component;
};

/* Structure for storing the processed frame data */
struct Processed_Image {
	// A copy of the original image
	Image original_image;
	// The Masked RGB component of the frame
	Mat RGB_Masked;
	// A vector of points that make up the contour of the image
	vector<Point> outline;
};

/* Structure for storing contour data in the image */
/* Includes the outline of the object, a bounding rectangle and a bounding circle*/
struct Contour_Data {
	// List of lists of points that define sections of the object
	vector<vector<Point>> contours;
	// Bounding rectangle
	Rect rectangle;
	// Center of the bounding circle
	Point2f center;
	// Radius of the bounding circle
	float radius;
};

/* Structure to hold data with corresponding labels */
struct Data {
	// List of lists of frames
	Mat trainingData;
	// List of labels that correspond to each frame
	vector<int> trainingLabels;
};
