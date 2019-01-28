#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char** argv)
{
	/// Load an image
	Mat img1 = imread(argv[1],CV_LOAD_IMAGE_COLOR); 
	
	if (img1.empty())
	{
		char ch;
		cin >> ch;
		return -1;
	}

	cout<<"\n"<<img1.cols;
	cout << "\n" << img1.rows;

	/// show an image

	imshow("My Image", img1);

	waitKey(0);
	return 0;
}
