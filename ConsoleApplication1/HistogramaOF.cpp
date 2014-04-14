#define _USE_MATH_DEFINES

#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "HistogramaOF.h"
#include "OpticalFlow.h"
#include <math.h>
#include <cstdio>

using namespace cv;
using namespace std;

//const float PI = 3.1415927;

/* Creadora */
HistogramaOF::HistogramaOF() { // Mirar si cal inicialitzar
	/*for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_ALT; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						dades[i][j][k][u][v] = 0;
					}
				}
			}
		}
	}*/
	for(int i = 0; i < MOVIMENT_PLA; ++i) {
		for(int j = 0; j < MOVIMENT_ALT; ++j) {
			dades[i][j] = 0;
		}
	}
}

/* Funció que calcula el mòdul d'un vector */
double HistogramaOF::calculaModul(Point3i p) {
	// Calculem el mòdul del vector
	return sqrt(p.x*p.x + p.y*p.y);
}

int signe(int x) {
	int s = 0;
	if(x > 0) s = 1;
	else if(x == 0) s = 0;
	else s = -1;
	return s;
}

/* Funció que discretitza un vector de desplaçament segons el seu moviment en el pla */
int HistogramaOF::discretitzaMovimentPla(Point3i despl) {
	// Càlcul de l'angle d'Azimut
	double angle = 0;
	if(despl.x > 0 && despl.y > 0) angle = atan(despl.y/despl.x);
	else if(despl.x > 0 && despl.y < 0) angle = 2*M_PI + atan(despl.y/despl.x);
	else if(despl.x == 0) angle = (M_PI/2)*signe(despl.y);
	else if(despl.x < 0) angle = M_PI + atan(despl.y/despl.x);
	angle = angle*180.0/M_PI;
	if(angle < 0) angle = angle + 360;
	int pos = 0;
	// Discretització de la posició en 4 sectors
	if(angle <= 45 || angle > 315) pos = 0; //Endavant
	else if(angle > 45 && angle <= 135) pos = 1; // Dreta
	else if(angle > 135 && angle <= 225) pos = 2; // Enrere
	else pos = 3; // Esquerre
	return pos;
}

/* Funció que discretitza un vector de desplaçament segons el seu moviment en alçada */
int HistogramaOF::discretitzaMovimentAlt(Point3i despl) {
	// Càlcul de l'angle de colatitud
	double angle = 0;
	// El càlcul del primer i l'últim no es correcte
	if(despl.z > 0) angle = atan((sqrt(despl.x*despl.x + despl.y*despl.y))/despl.z)*180.0/M_PI;
	else if(despl.z == 0) angle = M_PI/2*180.0/M_PI;
	else angle = (M_PI + atan((sqrt(despl.x*despl.x + despl.y*despl.y))/despl.z))*180.0/M_PI;
	if(angle < 0) angle = angle + 360;
	int pos = 0;
	// Discretització de la posició en 3 sectors
	if(angle < 60) pos = 0; // Amunt
	else if(angle >= 60 && angle <= 120) pos = 1; // Pla
	else pos = 2; // Avall
	return pos;
}

/* Funció que calcula l'histograma d'Optical Flow entre dos frames consecutius */
void HistogramaOF::calcularHistogramaOF(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d) {
	cout << "Calculant Histograma Optical Flow..." << endl;
	// Comencem a comptar el temps
	float start = (float)getTickCount();
	// Calculem l'Optical Flow entre els dos frames
	OpticalFlow OF;
	OF.calcularOpticalFlow3D(frame1, frame2, frame1_d, frame2_d);
	vector<Point3i> ini = OF.getOpticalFlow3DInici();
	vector<Point3i> despl = OF.getOpticalFlow3DDespl();
	// Recorrem tots els punts calculats per l'Optical Flow
	int mov_pla, mov_alt = 0;
	for(int i = 0; i < ini.size(); ++i) {
		// Discretitzem per posicio
		// Agafant les dades de ini
		// Discretitzem per direcció del moviment (amb les dades del desplaçament de cada punt)
		Point3i des = despl.at(i);
		mov_pla = discretitzaMovimentPla(des);
		mov_alt = discretitzaMovimentAlt(des);
		dades[mov_pla][mov_alt]++;
	}
	
	cout << "Histograma calculat" << endl;
	for(int i = 0; i < MOVIMENT_PLA; ++i) {
		for(int j = 0; j < MOVIMENT_ALT; ++j) {
			cout << dades[i][j] << "    ";
		}
		cout << endl;
	}
	printf("Temps total Histograma Optical Flow: %lf sec\n", (getTickCount() - start) / getTickFrequency());
}