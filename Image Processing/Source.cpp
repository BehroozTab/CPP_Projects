#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char** argv)
{
	// Load the image
	Mat img_color = imread(argv[1],CV_LOAD_IMAGE_COLOR); 
	Mat img_gray = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	if (img_color.empty())
	{
		char ch;
		cin >> ch;
		return -1;
	}

	// show the image
	namedWindow("Color Image", CV_WINDOW_FREERATIO);
	namedWindow("Gray Image", CV_WINDOW_AUTOSIZE);


	imshow("Color Image", img_color);
	imshow("Gray Image", img_gray);

	resizeWindow("Color Image", img_color.cols-100,img_color.rows-100);
	resizeWindow("Gray Image", img_color.cols-100, img_color.rows-100);

	moveWindow("Color Image",100,100);
	moveWindow("Gray Image", img_gray.cols+100, 100);


	imwrite("gray_img.jpg", img_gray);

	waitKey(0);
	return 0;
}
