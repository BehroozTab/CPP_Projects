#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int main(int, char** argv)
{
	/// Load an image
	Mat img1 = imread(argv[1]); 
	
	if (img1.empty())
	{
		char ch;
		scanf("%c", &ch);
		return -1;
	}

	printf("\n%d", img1.cols);
	printf("\n%d", img1.rows);

	/// show an image

	imshow("My Image", img1);

	waitKey(0);
	return 0;
}
