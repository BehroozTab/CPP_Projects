#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char** argv)
{
	int i, j;

	Mat img_color1 = imread(argv[1], CV_LOAD_IMAGE_COLOR); 
	Mat img_color2 = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if (img_color1.empty())
	{
		char ch;
		cin >> ch;
		return -1;
	}

	for (i = 0; i < img_color2.rows; i++)
		for (j = 0; j < img_color2.cols; j++)
			if (img_color2.at<Vec3b>(i, j)[0]<160)
				img_color2.at<Vec3b>(i, j)[0] = 0;

	namedWindow("Color Image 1", CV_WINDOW_FREERATIO);
	namedWindow("Color Image 2", CV_WINDOW_FREERATIO);

	imshow("Color Image 1", img_color1);
	imshow("Color Image 2", img_color2);

	moveWindow("Color Image 1",100,100);
	moveWindow("Color Image 2", img_color2.cols+100, 100);

	imwrite("color_img.jpg", img_color2);

	waitKey(0);
	return 0;
}
