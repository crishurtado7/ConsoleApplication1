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

/* Creadora */
OpticalFlow::OpticalFlow() {
	this->size = 0;
}

/* Funció que calcula el mòdul d'un vector a partir del seu punt origen i el seu punt destí */
int OpticalFlow::calculaModul(Point p, Point q) {
	// Calculem les dues components del vector
	int u = q.x - p.x;
	int v = q.y - p.y;
	// Calculem el mòdul del vector
	int mod = sqrt(u*u + v*v);
	return mod;
}

/* Funció que dibuixa una fletxa des de el punt p fins al punt q sobre la imatge "image" */
void OpticalFlow::drawArrow(Mat image, Point p, Point q, Scalar color, int arrowMagnitude = 2, int thickness=1, int line_type=8, int shift=0) {
	// Calculem el color que tindrà la fletxa, segons el mòdul que tingui el vector
	int mod = calculaModul(p, q);
	if(mod >= 2) color = Scalar(0,0,255); // Vermell
	else if(mod >= 1 && mod < 2) color = Scalar(0,255,0); // Verd
	else color = Scalar(255,0,0); // Blau
	// Dibuixem la línia principal, de p a q
    line(image, p, q, color, thickness, line_type, shift);
    const double PI = 3.141592653;
    // Calculem l'angle alpha
    double angle = atan2((double)p.y-q.y, (double)p.x-q.x);
    // Calculem les coordenades del primer segment
    p.x = (int) ( q.x +  arrowMagnitude * cos(angle + PI/4));
    p.y = (int) ( q.y +  arrowMagnitude * sin(angle + PI/4));
    // Dibuixem el primer segment
    line(image, p, q, color, thickness, line_type, shift);
    // Calculem les coordenades del segon segment
    p.x = (int) ( q.x +  arrowMagnitude * cos(angle - PI/4));
    p.y = (int) ( q.y +  arrowMagnitude * sin(angle - PI/4));
    // Dibuixem el segon segment
    line(image, p, q, color, thickness, line_type, shift);
}

/* Funció que calcula una màscara a partir de la binarització d'una imatge en escala de grisos */
Mat OpticalFlow::calcularMascara(Mat frame_d) {
	// Inicialitzem els valors necessaris
	Mat result;
	double thresh = 1;
	double maxval = 255;
	// Binaritzem la imatge. Només volem separar entre els valors que són 0 (negre) dels que no ho són
	threshold(frame_d, result, thresh, maxval, THRESH_BINARY);
	imshow("Màscara", result);
	return result;
}

/* Funció que calcula l'optical flow 3D entre dos frames consecutius */
void OpticalFlow::calcularOpticalFlow3D(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d) {
	// Comprovem que les imatges tinguin la mateixa mida
	if(frame1.rows != frame2.rows && frame1.cols != frame2.cols) {
		printf("Les imatges han de ser de la mateixa mida\n");
		exit(1);
	}

	// Comprovem que les imatges siguin del mateix tipus
	if(frame1.type() != 16 || frame2.type() != 16) {
		printf("Les imatges han de ser del mateix tipus\n");
	}
	
	printf("Tamany de les imatges: [rows = %d, cols = %d]\n", frame1.rows, frame1.cols);

	// Comencem a comptar el temps
	float start = (float)getTickCount();

	Mat grayFrames1, rgbFrames1, grayFrames2, rgbFrames2;

	vector<Point2f> points1;
	vector<Point2f> points2;

	Point2f diff;

	// Inicialitzem valors per calcular l'optical flow
	vector<uchar> status;
	vector<float> err;
	int i, k;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(45, 45);
	namedWindow("Resultat Optical Flow 2D", CV_WINDOW_AUTOSIZE);
	double angle;

	// Passem els frames RGB a escala de grisos
	frame1.copyTo(rgbFrames1);
	cvtColor(rgbFrames1, grayFrames1, CV_BGR2GRAY);

	frame2.copyTo(rgbFrames2);
	cvtColor(rgbFrames2, grayFrames2, CV_BGR2GRAY);

	// Càlcul de la màscara que delimita la regió d'interès on buscar punts
	Mat mask = calcularMascara(frame1_d);
	cout << "Màscara calculada" << endl;

	// Shi/Tomasi
	goodFeaturesToTrack(grayFrames1, points1, 500, 0.01, 5, mask, 3, 0, 0.04);
	goodFeaturesToTrack(grayFrames2, points2, 500, 0.01, 5, mask, 3, 0, 0.04);

	cout << "Shi tomasi acabat" << endl;

	cout << "Calculant Optical Flow 2D..." << endl;

	// Optical Flow Lukas-Kanade
	calcOpticalFlowPyrLK(grayFrames1, grayFrames2, points1, points2, status, err, winSize, 3, termcrit, 0, 0.001);

	// Dibuixar les fletxes per veure el resultat de l'optical flow
	for( int i=0; i < status.size(); i++ ){
				Point p0( ceil( points1[i].x ), ceil( points1[i].y ) );
				Point p1( ceil( points2[i].x ), ceil( points2[i].y ) );
				drawArrow(rgbFrames1, p0, p1, CV_RGB(255, 0, 0));
    }

	cout << "Calculant Optical Flow 3D..." << endl;
	// Per cada punt on hem calculat l'optical flow, calculem la component z amb la informació de la imatge de profunditat
	int grayLevel1, grayLevel2 = 0;
	for(int i = 0; i < points2.size(); ++i) {
		grayLevel1 = frame1_d.at<uchar>(int(points1[i].y), int(points1[i].x));
		grayLevel2 = frame2_d.at<uchar>(int(points2[i].y), int(points2[i].x));
		// AQUI FALTA CONVERTIR EL NIVELL DE GRIS A PIXEL
		// Descartem aquells punts que cauen fora de la imatge registrada per la càmera de profunditats
		if(grayLevel1 != 0 && grayLevel2 != 0) {
			cout << "GrayLevel1: " << grayLevel1 << "   , GrayLevel2: " << grayLevel2 << endl;
			Point3i inici(int(points1[i].x), int(points1[i].y), grayLevel1);
			Point3i desp(int(points2[i].x) - int(points1[i].x), int(points2[i].y) - int(points1[i].y), grayLevel2 - grayLevel1);
			this->OpticalFlow3DInici.push_back(inici);
			this->OpticalFlow3DDespl.push_back(desp);
			this->size = (this->size) + 1;
		}
	}
	imshow("Resultat Optical Flow 2D", rgbFrames1);
	cout << "Número de vectors a l'optical flow 3D: " << this->size << endl;

	printf("Temps total : %lf sec\n", (getTickCount() - start) / getTickFrequency());
}

int OpticalFlow::getSize() {
	return this->size;
}




