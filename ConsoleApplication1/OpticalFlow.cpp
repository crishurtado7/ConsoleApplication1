#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "OpticalFlow.h"
#include <math.h>
#include <cstdio>
#include "Constants.h"

using namespace cv;
using namespace std;

/* Creadora */
OpticalFlow::OpticalFlow() {
	this->size = 0;
	origen.x = ORIGENX;
	origen.y = ORIGENY;
	origen.z = ORIGENZ;
}

/* Funció que calcula el mòdul d'un vector a partir del seu punt origen i el seu punt destí */
float OpticalFlow::calculaModul(Point p, Point q) {
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
	/*Mat aux;
	cvtColor(result, aux, CV_GRAY2RGB);
	Point p(180, 200);
	drawArrow(aux, p, p, Scalar(0, 0, 255));*/
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
	float startMasc = (float)getTickCount();
	Mat mask = calcularMascara(frame1_d);
	cout << "Màscara calculada" << endl;
	printf("Temps càlcul màscara: %lf sec\n", (getTickCount() - startMasc) / getTickFrequency());

	// Shi/Tomasi
	float startShiTom = (float)getTickCount();
	goodFeaturesToTrack(grayFrames1, points1, 200, 0.01, 5, mask, 3, 0, 0.04);
	cout << "Shi tomasi acabat" << endl;
	cout << "Punts calculats al Shi tomasi: " << points1.size() << endl;
	printf("Temps càlcul punts Shi tomasi: %lf sec\n", (getTickCount() - startShiTom) / getTickFrequency());

	cout << "Calculant Optical Flow 2D..." << endl;

	// Optical Flow Lukas-Kanade
	float startOF2D = (float)getTickCount();
	calcOpticalFlowPyrLK(grayFrames1, grayFrames2, points1, points2, status, err, winSize, 3, termcrit, 0, 0.001);
	printf("Temps càlcul Optical Flow 2D: %lf sec\n", (getTickCount() - startOF2D) / getTickFrequency());

	cout << "Calculant Optical Flow 3D..." << endl;
	float startOF3D = (float)getTickCount();
	// Per cada punt on hem calculat l'optical flow, calculem la component z amb la informació de la imatge de profunditat
	int grayLevel1, grayLevel2 = 0;
	int xA, yA, zA, xB, yB, zB;
	for(int i = 0; i < points2.size(); ++i) {
		grayLevel1 = frame1_d.at<uchar>(int(points1[i].y), int(points1[i].x));
		grayLevel2 = frame2_d.at<uchar>(int(points2[i].y), int(points2[i].x));
		// AQUI FALTA CONVERTIR EL NIVELL DE GRIS A PIXEL OR SOMETHING
		// Descartem aquells punts que cauen fora de la imatge registrada per la càmera de profunditats
		// També descartem aquells punts que tinguin un mòdul massa petit
		if(grayLevel1 != 0 && grayLevel2 != 0 && calculaModul(points1[i],points2[i]) >= 2) {
			// Dibuixar les fletxes per veure el resultat de l'optical flow
			Point p0( ceil( points1[i].x ), ceil( points1[i].y ) );
			Point p1( ceil( points2[i].x ), ceil( points2[i].y ) );
			drawArrow(rgbFrames1, p0, p1, CV_RGB(255, 0, 0));
			// Creem el punt 3D
			// AQUI FALTARIA CONVERTIR EL PUNT AL SISTEMA DE COORDENADES DE LA PERSONA
			xA = int(points1[i].x) - origen.x;
			xB = int(points2[i].x) - origen.x;
			yA = grayLevel1;
			yB = grayLevel2;
			zA = (int(points1[i].y) - origen.y)*-1;
			zB = (int(points2[i].y) - origen.y)*-1;
			Point3i inici(xA, yA, zA);
			Point3i desp(xB - xA, yB - yA, zB - zA);
			//Point3i inici(int(points1[i].x), int(points1[i].y), grayLevel1);
			//Point3i desp(int(points2[i].x) - int(points1[i].x), int(points2[i].y) - int(points1[i].y), grayLevel2 - grayLevel1);
			this->OpticalFlow3DInici.push_back(inici);
			this->OpticalFlow3DDespl.push_back(desp);
			this->size = (this->size) + 1;
			//cout << "Punt " << i << ": " << int(points2[i].x) - int(points1[i].x) << "    " << int(points2[i].y) - int(points1[i].y) << endl;
		}
	}
	imshow("Resultat Optical Flow 2D", rgbFrames1);
	cout << "Número de vectors a l'optical flow 3D: " << this->size << endl;
	printf("Temps càlcul Optical Flow 3D: %lf sec\n", (getTickCount() - startOF3D) / getTickFrequency());

	printf("Temps total Optical Flow: %lf sec\n", (getTickCount() - start) / getTickFrequency());
}

/* Funció que retorna la mida dels vectors d'Optical Flow */
int OpticalFlow::getSize() {
	return this->size;
}

/* Funció que retorna el vector de posicions dels punts calculats a l'Optical Flow */
vector<Point3i> OpticalFlow::getOpticalFlow3DInici() {
	return this->OpticalFlow3DInici;
}

/* Funció que retorna el vector de desplaçaments dels punts calculats a l'Optical Flow */
vector<Point3i> OpticalFlow::getOpticalFlow3DDespl() {
	return this->OpticalFlow3DDespl;
}



