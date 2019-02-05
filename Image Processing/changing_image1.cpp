#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char** argv)
{
	int i, j;

	Mat img_gray1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_gray2 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	if (img_gray1.empty())
	{
		char ch;
		cin >> ch;
		return -1;
	}

	for (i = 0; i < img_gray2.rows; i++)
		for (j = 0; j < img_gray2.cols; j++)
			if (img_gray2.at<unsigned char>(i, j)<160)
			   img_gray2.at<unsigned char>(i, j) = img_gray2.at<unsigned char>(i, j)+40;

	namedWindow("Gray Image 1", CV_WINDOW_FREERATIO);
	namedWindow("Gray Image 2", CV_WINDOW_FREERATIO);

	imshow("Gray Image 1", img_gray1);
	imshow("Gray Image 2", img_gray2);

	moveWindow("Gray Image 1",100,100);
	moveWindow("Gray Image 2", img_gray2.cols+100, 100);

	imwrite("gray_img.jpg", img_gray2);

	waitKey(0);
	return 0;
}
