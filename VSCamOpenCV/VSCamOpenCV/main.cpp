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
	string cmd = argv[1];

	cout << "cmd: " << cmd << endl;

	if(cmd.compare("fd") == 0) faceDetection_main();
	else if(cmd.compare("ar_d") == 0) armarkerDetection_main();
	else if(cmd.compare("ar_g01") == 0) armarkerDetection_main_game01();
	else if(cmd.compare("ar_c") == 0) armarkerCreation();
	else cout << "no cmmand named " << cmd << endl;

	return 0;
}