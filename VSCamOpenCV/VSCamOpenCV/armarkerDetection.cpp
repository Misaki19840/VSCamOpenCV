#include "armarkerDetection.h"
#include "Parameters.h"

#include    <iomanip>

void game01(Mat& inimg, Mat& outimg, vector<int> ids, vector<vector<Point2f>> corners, Targetset& targetset, PointerSet& pointerset, int flgflpimg);

void armarkerDetection_main() {

	int waitTime = 10;

	cv::VideoCapture inputVideo;
	inputVideo.open(0);
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	
	while (inputVideo.grab()) {

		Mat image, imageCopy;
		inputVideo.retrieve(image);
		image.copyTo(imageCopy);
		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f> > corners;

		aruco::detectMarkers(image, dictionary, corners, ids);

		// if at least one marker detected
		if (ids.size() > 0)
			aruco::drawDetectedMarkers(imageCopy, corners, ids);
		imshow("out", imageCopy);

		char key = (char)waitKey(waitTime);
		if (key == 27)
			break;
	}

}


void armarkerDetection_main_game01() {

	int waitTime = 10;

	VideoCapture inputVideo;
	inputVideo.open(0);

	//inputVideo.set(CV_CAP_PROP_FRAME_WIDTH, CAPT_WIDTH);
	//inputVideo.set(CV_CAP_PROP_FRAME_HEIGHT, CAPT_HEIGHT);

	Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

	int width = inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
	Targetset targetset = Targetset(width,height);

	PointerSet pointerset = PointerSet();

	float scale = (((float)WINDOW_WIDTH / (float)width) < ((float)WINDOW_HEIGHT / (float)height)) ? ((float)WINDOW_WIDTH / (float)width) : ((float)WINDOW_HEIGHT / (float)height);

	while (inputVideo.grab()) {

		Mat inimage, outimage;
		inputVideo.retrieve(inimage);
		inimage.copyTo(outimage);
		vector<int> ids;
		vector<vector<Point2f>> corners;

		aruco::detectMarkers(inimage, dictionary, corners, ids);

		game01(inimage, outimage, ids, corners, targetset, pointerset, FLIPIMG);

		Mat resizedImg;
		resize(outimage, resizedImg, Size(), scale, scale);
		imshow("out", resizedImg);

		char key = (char)waitKey(waitTime);
		if (key == 27)
			break;
	}

}


void game01(Mat& inimg, Mat& outimg, vector<int> ids, vector<vector<Point2f>> corners, Targetset& targetset, PointerSet& pointerset, int flgflpimg) {
	//vector<Point2f> pointCents;
	Scalar color_pointer = Scalar(255, 0, 0);
	Scalar color_tgt = Scalar(0, 255, 0);

	//update target
	targetset.update();

	if (flgflpimg > 0) {
		flip(outimg, outimg, flgflpimg);
	}

	//draw target
	for (int i = 0; i < targetset.ids.size(); i++) {
		int posx = (int)targetset.posset[i].x;
		int posy = (int)targetset.posset[i].y;

		if (flgflpimg > 0) {
			posx = outimg.cols - posx;
		}

		int tgtImgWidth = targetset.imgs[targetset.ids[i]].cols;
		int tgtImgHeight = targetset.imgs[targetset.ids[i]].rows;
		int tgtStep = targetset.imgs[targetset.ids[i]].step;
		int tgtElemsize = targetset.imgs[targetset.ids[i]].elemSize();

		for (int y = 0; y < tgtImgHeight; ++y) {
			for (int x = 0; x < tgtImgWidth; ++x) {

				int posxx = posx - tgtImgWidth / 2;
				int posyy = posy - tgtImgHeight / 2;

				if (((posyy + y) >= outimg.rows) || ((posxx + x) >= outimg.cols)) break;

				int tgtAlpha = targetset.imgs[targetset.ids[i]].data[y * tgtStep + x * tgtElemsize + 3];
				if (tgtAlpha > 0) {
					outimg.data[(posyy + y) * outimg.step + (posxx + x) * outimg.elemSize() + 0] = targetset.imgs[targetset.ids[i]].data[y * tgtStep + x * tgtElemsize + 0];
					outimg.data[(posyy + y) * outimg.step + (posxx + x) * outimg.elemSize() + 1] = targetset.imgs[targetset.ids[i]].data[y * tgtStep + x * tgtElemsize + 1];
					outimg.data[(posyy + y) * outimg.step + (posxx + x) * outimg.elemSize() + 2] = targetset.imgs[targetset.ids[i]].data[y * tgtStep + x * tgtElemsize + 2];
				}
			}
		}

		//int size = 2;
		//rectangle(outimg, cvPoint(posx - size, posy - size), cvPoint(posx + size, posy + size), color_tgt, 3, 8, 0);

		//stringstream ss;
		//ss << "id: " << targetset.ids[i] << endl;
		//ss << "point: " << targetset.PointId[targetset.ids[i]] << endl;
		//putText(outimg, ss.str(), cvPoint(posx + 5, posy + 5), FONT_HERSHEY_SIMPLEX, 1, (0, 0, 0), 2, LINE_AA);
	}

	//draw pointer
	vector<int> flgEraceId(targetset.ids.size(),0);
	for (int i = 0; i < ids.size(); i++) {
		int posx = (int)(corners[i][0].x + corners[i][1].x + corners[i][2].x + corners[i][3].x);
		int posy = (int)(corners[i][0].y + corners[i][1].y + corners[i][2].y + corners[i][3].y);

		posx = posx / 4;
		posy = posy / 4;

		//
		for (int j = 0; j < targetset.ids.size(); j++) {
			double dist = (posx - targetset.posset[j].x) * (posx - targetset.posset[j].x) + (posy - targetset.posset[j].y) * (posy - targetset.posset[j].y);
			dist = sqrt(dist);
			if (dist < HITDIST) {
				pointerset.setPoint(ids[i], targetset.PointId[targetset.ids[j]]);
				//iderace.push_back(i);
				flgEraceId[j] = 1;
			}
		}

		if (flgflpimg > 0) {
			posx = outimg.cols - posx;
		}

		//cout << "posx, outimg.cols, outimg.rows: " << posx << " , " << outimg.cols << " , " << outimg.rows << endl;

		int size = 5;
		rectangle(outimg, cvPoint(posx - size, posy - size), cvPoint(posx + size, posy + size), pointerset.color[ids[i]], 3, 8, 0);

		//put text
		stringstream ss;
		//ss << "marker: " << ids[i] << "\n";
		ss << pointerset.point[ids[i]];
		putText(outimg, ss.str(), cvPoint(posx + 5, posy + 5), FONT_HERSHEY_SIMPLEX, 2, pointerset.color[ids[i]], 2, LINE_AA);

	}

	targetset.deleteTarget(flgEraceId);

}

void armarkerCreation() {
	
	Mat markerImage;
	Ptr<aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	for (int i = 0; i < 250; i++) {
		aruco::drawMarker(dictionary, i, 200, markerImage, 1);
		stringstream ss;
		ss << "Aruco_id" << std::setw(3) << std::setfill('0') << i << ".png";
		imwrite(ss.str(), markerImage);
	}
}