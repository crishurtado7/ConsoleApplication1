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
float OpticalFlow::calculaModul(Point3i p) {
	// Calculem el mòdul del vector
	int mod = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
	return mod;
}

/* Funció que dibuixa una fletxa des de el punt p fins al punt q sobre la imatge "image" */
void OpticalFlow::drawArrow(Mat image, Point p, Point q, Scalar color, int arrowMagnitude = 2, int thickness=1, int line_type=8, int shift=0) {
	// Calculem el color que tindrà la fletxa, segons el mòdul que tingui el vector
	int u = q.x-p.x;
	int v = q.y - p.y;
	int mod = sqrt(u*u + v*v);
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
	return result;
}

/* Funció que transforma un punt de coordenades de càmera a coordenades de món */
Point3f OpticalFlow::transformaCoordenades(Point3f p, int width, int height) {
	float nivell_gris = 124.5;
	float mm = 1000;
	float Ox = width/2;
	float Oy = height/2;
	float INVERS_LAMBDA = 2.0/525.0;
	// Passem de coordenades de píxel a coordenades de món (passant per coordenades de càmera)
	float z_world = (p.z*mm)/nivell_gris;
	float x_world = (p.x - Ox)*z_world*INVERS_LAMBDA;
	float y_world = (p.y - Oy)*z_world*INVERS_LAMBDA;
	// Passem al sistema de coordenades que volem
	float z_origen = (Z_CAMERA*mm)/nivell_gris;
	float x_origen = (ORIGENX - Ox)*z_origen*INVERS_LAMBDA;
	float y_origen = (ORIGENY - Oy)*z_origen*INVERS_LAMBDA;

	float mat[12] = {-1.0, 0, 0, x_origen, 0, 0, -1, z_origen, 0, -1, 0, y_origen};
	Mat transformacio(3, 4, CV_32FC1, mat);
	Mat vector = (Mat_<float>(4,1) << x_world, y_world, z_world, 1);
	Mat res = transformacio*vector;
	Point3f coord_world;
	coord_world.x = res.at<float>(0, 0);
	coord_world.y = res.at<float>(1, 0);
	coord_world.z = res.at<float>(2, 0);
	return coord_world;
}

/* Funció que calcula l'optical flow 3D entre dos frames consecutius */
Mat OpticalFlow::calcularOpticalFlow3D(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d) {
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
		if(int(points2[i].x) < frame1.cols && int(points2[i].x) >= 0 && int(points2[i].y) >= 0 && int(points2[i].y) < frame1.rows) {
			grayLevel1 = frame1_d.at<uchar>(int(points1[i].y), int(points1[i].x));
			grayLevel2 = frame2_d.at<uchar>(int(points2[i].y), int(points2[i].x));
			// AQUI FALTA CONVERTIR EL NIVELL DE GRIS A PIXEL OR SOMETHING
			// Descartem aquells punts que cauen fora de la imatge registrada per la càmera de profunditats
			// També descartem aquells punts que tinguin un mòdul massa petit
			if(grayLevel1 != 0 && grayLevel2 != 0 && abs(grayLevel1 - grayLevel2) < 5) {
				// Dibuixar les fletxes per veure el resultat de l'optical flow 2D
				Point p0( ceil( points1[i].x ), ceil( points1[i].y ) );
				Point p1( ceil( points2[i].x ), ceil( points2[i].y ) );
				// Creem el punt 3D
				// Convertim al sistema de coordenades respecte la persona
				Point3f point1(points1[i].x, points1[i].y, grayLevel1);
				Point3f point2(points2[i].x, points2[i].y, grayLevel2);

				Point3i inici = transformaCoordenades(point1, frame1.cols, frame1.rows);
				Point3i aux = transformaCoordenades(point2, frame1.cols, frame1.rows);
				Point3i desp(aux.x - inici.x, aux.y - inici.y, aux.z - inici.z);
				if(/*calculaModul(desp) > 15*/ sqrt(desp.x*desp.x + desp.z*desp.z) > 2 && abs(desp.y) < 15) {
					drawArrow(rgbFrames1, p0, p1, CV_RGB(255, 0, 0));
					this->OpticalFlow3DInici.push_back(inici);
					this->OpticalFlow3DDespl.push_back(desp);
					this->size = (this->size) + 1;
				}
				//cout << "Punt " << i << ": " << xA << "    " << yA << "     " << zA << endl;
			}
		}
	}
	cout << "Número de vectors a l'optical flow 3D: " << this->size << endl;
	printf("Temps càlcul Optical Flow 3D: %lf sec\n", (getTickCount() - startOF3D) / getTickFrequency());

	printf("Temps total Optical Flow: %lf sec\n", (getTickCount() - start) / getTickFrequency());

	return rgbFrames1;
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



