#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> 
#include <iostream> 

#include "faceDetection.h"
#include "armarkerDetection.h"

using namespace std;
using namespace cv;

// Function for Face Detection 
void detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale);
string cascadeName;

int main(int argc, const char** argv)
{
	char cmd = 'g';

	switch (cmd)
	{
	case 'f':
	{faceDetection_main(); break; }
	case 'a':
	{
		armarkerDetection_main();
	break; 
	}
	case 'g':
	{
		armarkerDetection_main_game01();
		break;
	}
	case 'c':
	{
		armarkerCreation();
	}
	default:
		break;
	}

	return 0;
}