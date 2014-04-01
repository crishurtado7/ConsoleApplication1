#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "OpticalFlow.h"
#include <math.h>
#include <cstdio>

using namespace cv;
using namespace std;

#define MAX_COUNT = 500;

OpticalFlow::OpticalFlow() {
}

int OpticalFlow::calculaModul(Point p, Point q) {
	int u = q.x - p.x;
	int v = q.y - p.y;
	int mod = sqrt(u*u + v*v);
	return mod;
}

void OpticalFlow::drawArrow(Mat image, Point p, Point q, Scalar color, int arrowMagnitude = 2, int thickness=1, int line_type=8, int shift=0) {
	//Draw the principle line
	int mod = calculaModul(p, q);
	if(mod >= 2) color = Scalar(0,0,255); // Vermell
	else if(mod >= 1 && mod < 2) color = Scalar(0,255,0); // Verd
	else color = Scalar(255,0,0); // Blau

    line(image, p, q, color, thickness, line_type, shift);
    const double PI = 3.141592653;
    //compute the angle alpha
    double angle = atan2((double)p.y-q.y, (double)p.x-q.x);
    //compute the coordinates of the first segment
    p.x = (int) ( q.x +  arrowMagnitude * cos(angle + PI/4));
    p.y = (int) ( q.y +  arrowMagnitude * sin(angle + PI/4));
    //Draw the first segment
    line(image, p, q, color, thickness, line_type, shift);
    //compute the coordinates of the second segment
    p.x = (int) ( q.x +  arrowMagnitude * cos(angle - PI/4));
    p.y = (int) ( q.y +  arrowMagnitude * sin(angle - PI/4));
    //Draw the second segment
    line(image, p, q, color, thickness, line_type, shift);
}

Mat OpticalFlow::calculaPuntsClau(Mat frame1, Mat frame2) {
	Mat result(frame1);
	// Primer calculem la imatge diferencia (en valor absolut) entre frame1 i frame2 i seleccionem els punts en que supera un cert llindar
	absdiff(frame1, frame2, result);
	for (int i = 0; i < result.size().height; ++i) { 
		for (int j = 1; j < result.size().width; ++j) { 
			Scalar intensity = result.at<uchar>(i, j);
			if (intensity[0] <= 25) result.at<uchar>(i,j) = 0;
		}
	}
	return result;
}

void OpticalFlow::calcularOpticalFlow3D(Mat& frame1, Mat& frame2) {
	if(frame1.rows != frame2.rows && frame1.cols != frame2.cols) {
		printf("Images should be of equal sizes\n");
		exit(1);
	}
	if(frame1.type() != 16 || frame2.type() != 16) {
		printf("Images should be of equal type CV_8UC3\n");
	}
	
	printf("Read two images of size [rows = %d, cols = %d]\n", frame1.rows, frame1.cols);

	float start = (float)getTickCount();

	Mat grayFrames1, rgbFrames1, grayFrames2, rgbFrames2;
	Mat opticalFlow = Mat(frame1.rows,frame1.cols, CV_32FC3);

	Size img_sz = frame1.size();
    Mat imgC(img_sz,1);

	vector<Point2f> points1;
	vector<Point2f> points2;

	Point2f diff;

	vector<uchar> status;
	vector<float> err;

	RNG rng(12345);
	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
	rng.uniform(0, 255));
	bool needToInit = true;

	int i, k;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	namedWindow("Raw video", CV_WINDOW_AUTOSIZE);
	double angle;

	frame1.copyTo(rgbFrames1);
	cvtColor(rgbFrames1, grayFrames1, CV_BGR2GRAY);

	frame2.copyTo(rgbFrames2);
	cvtColor(rgbFrames2, grayFrames2, CV_BGR2GRAY);

	/*Mat C = calculaPuntsClau(grayFrames1, grayFrames2);
	cout << "Punts clau calculats..." << endl;
	points1.clear();
	for(int i = 0; i < C.size().height; ++i) {
		for(int j = 0; j < C.size().width; ++j) {
			Scalar intensity = C.at<uchar>(i, j);
			if(intensity[0] != 0) {
				Point2f aux(i, j);
				points1.push_back(aux);
			}
		}
	}

	cout << "Punts clau calculats222222..." << endl;
	cout << "Nombre de punts: " << points1.size() << endl;*/
	//for(int i = 0; i < points1.size(); ++i) cout << "Punt " << (int)i << ": " << (int)points1[i].x << "    " << (int)points1[i].y << endl;

	// Shi/Tomasi
	goodFeaturesToTrack(grayFrames1, points1, 500, 0.01, 5, Mat(), 3, 0, 0.04);
	goodFeaturesToTrack(grayFrames2, points2, 500, 0.01, 5, Mat(), 3, 0, 0.04);

	cout << "Shi tomasi acabat..." << endl;

	cout << "\n\n\nCalculating  calcOpticalFlowPyrLK\n\n\n\n\n";
	calcOpticalFlowPyrLK(grayFrames1, grayFrames2, points2, points1, status, err, winSize, 3, termcrit, 0, 0.001);
	int u = 0;
	for( int i=0; i < status.size(); i++ ){
        if(status[i] == 1) {
			if(err[i] > 2) {
				++u;
				Point p0( ceil( points1[i].x ), ceil( points1[i].y ) );
				Point p1( ceil( points2[i].x ), ceil( points2[i].y ) );
				drawArrow(rgbFrames1, p0, p1, CV_RGB(255, 0, 0));
			}
		}
    }
	cout << "Points1: " << (int)points1.size() << endl;
	cout << "Points2: " << (int)points2.size() << endl;
	cout << "Nombre de arrows dibuixades: " << (int)u << endl;
	imshow("Raw video", rgbFrames1);
	//imshow("Optical Flow Window", C);
	printf("calcOpticalFlowSF : %lf sec\n", (getTickCount() - start) / getTickFrequency());
}



void OpticalFlow::writeOpticalFlowToFile(Mat& flow, FILE* file) {
	int cols = flow.cols;
	int rows = flow.rows;

	fprintf(file, "PIEH");

	if(fwrite(&cols, sizeof(int), 1, file) != 1 || fwrite(&rows, sizeof(int), 1, file) != 1) {
		printf("writeOpticalFlowToFile : problem writing header\n");
		exit(1);
	}
	
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			Vec2f flow_at_point = flow.at<Vec2f>(i, j);

			if(fwrite(&(flow_at_point[0]), sizeof(float), 1, file) != 1 || fwrite(&(flow_at_point[1]), sizeof(float), 1, file) != 1) {
				printf("writeOpticalFlowToFile : problem writing data\n");
				exit(1);
			}
		}
	}
}



