#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char** argv)
{
	/// Load an image
	Mat img_color = imread(argv[1],CV_LOAD_IMAGE_COLOR); 
	Mat img_gray = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	
	if (img_color.empty())
	{
		char ch;
		cin >> ch;
		return -1;
	}

	cout<<"\n"<< img_color.cols;
	cout << "\n" << img_color.rows;

	cout << "\n" << img_gray.cols;
	cout << "\n" << img_gray.rows;

	/// show an image

	imshow("Color Image", img_color);
	imshow("Gray Image", img_gray);
	
        
	imwrite("gray_img.jpg", img_gray);


	waitKey(0);
	return 0;
}
