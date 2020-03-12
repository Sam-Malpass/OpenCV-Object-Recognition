#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

/* Structure for storing the image data of the frames, keeping both the RGB Components and the Depth Components */
struct Image {
	// The RGB component of the frame
	Mat RGB_Component;
	// The Depth component of the frame
	Mat Depth_Component;
};