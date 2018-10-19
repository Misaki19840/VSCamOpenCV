#include "faceDetection.h"

void detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale);

void faceDetection_main() {

	CascadeClassifier cascade;
	VideoCapture capture;
	Mat frame, image;
	double scale = 1;

	cascade.load("haarcascade_frontalface_default.xml");

	capture.open(0);
	if (capture.isOpened())
	{
		// Capture frames from video and detect faces 
		cout << "Face Detection Started...." << endl;
		while (1)
		{
			capture >> frame;
			if (frame.empty())
				break;
			Mat frame1 = frame.clone();
			detectAndDraw(frame1, cascade, scale);
			char c = (char)waitKey(10);

			// Press q to exit from window 
			if (c == 27 || c == 'q' || c == 'Q')
				break;
		}
	}
	else
		cout << "Could not Open Camera";
}

void detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale)
{
	vector<Rect> faces, faces2;
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale 
	double fx = 1 / scale;

	// Resize the Grayscale Image  
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	// Detect faces of different sizes using cascade classifier  
	cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Draw circles around the faces 
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = Scalar(255, 0, 0); // Color for Drawing tool 
		int radius;

		rectangle(img, cvPoint(r.x, r.y), cvPoint(r.x + r.width, r.y + r.height), color, 3, 8, 0);

	}

	// Show Processed Image with detected faces 
	imshow("Face Detection", img);
}