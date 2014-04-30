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

/* Funci� que calcula el m�dul d'un vector */
double HistogramaOF::calculaModul(Point3i p) {
	// Calculem el m�dul del vector
	return sqrt(p.x*p.x + p.y*p.y);
}

int signe(int x) {
	int s = 0;
	if(x > 0) s = 1;
	else if(x == 0) s = 0;
	else s = -1;
	return s;
}

/* Funci� que discretitza un vector de despla�ament segons el seu moviment en el pla */
int HistogramaOF::discretitzaMovimentPla(Point3i despl) {
	// C�lcul de l'angle d'Azimut
	double angle = 0;
	if(despl.x > 0 && despl.y > 0) angle = atan(despl.y/despl.x);
	else if(despl.x > 0 && despl.y < 0) angle = 2*M_PI + atan(despl.y/despl.x);
	else if(despl.x == 0) angle = (M_PI/2)*signe(despl.y);
	else if(despl.x < 0) angle = M_PI + atan(despl.y/despl.x);
	angle = angle*180.0/M_PI;
	if(angle < 0) angle = angle + 360;
	int pos = 0;
	// Discretitzaci� de la posici� en 4 sectors
	if(angle <= 45 || angle > 315) pos = 0; //Endavant
	else if(angle > 45 && angle <= 135) pos = 1; // Dreta
	else if(angle > 135 && angle <= 225) pos = 2; // Enrere
	else pos = 3; // Esquerre
	return pos;
}

/* Funci� que discretitza un vector de despla�ament segons el seu moviment en al�ada */
int HistogramaOF::discretitzaMovimentAlt(Point3i despl) {
	// C�lcul de l'angle de colatitud
	double angle = 0;
	// El c�lcul del primer i l'�ltim no es correcte
	if(despl.z > 0) angle = atan((sqrt(despl.x*despl.x + despl.y*despl.y))/despl.z)*180.0/M_PI;
	else if(despl.z == 0) angle = M_PI/2*180.0/M_PI;
	else angle = (M_PI + atan((sqrt(despl.x*despl.x + despl.y*despl.y))/despl.z))*180.0/M_PI;
	if(angle < 0) angle = angle + 360;
	int pos = 0;
	// Discretitzaci� de la posici� en 3 sectors
	if(angle < 60) pos = 0; // Amunt
	else if(angle >= 60 && angle <= 120) pos = 1; // Pla
	else pos = 2; // Avall
	return pos;
}

void HistogramaOF::representaHistograma() {
	float start = (float)getTickCount();
	int rows = ESPAI_X*MOVIMENT_PLA*6 + 2*ESPAI_X + 1;
	int cols = ESPAI_ALT*ESPAI_Y*MOVIMENT_ALT*6 + 2*ESPAI_ALT*ESPAI_Y + 1;
	int pos_x = 2; 
	int pos_y = 0;
	int nivell = 0;
	Scalar s;
	Mat representacio = Mat::zeros(cols, rows, CV_8UC3);
	for(int i = 0; i < ESPAI_ALT; ++i) {
		if(i == 0) s = Scalar(0,0,255);
		else if(i == 1) s = Scalar(0,255,255);
		else s = Scalar(0,255,0);
		nivell = i*(ESPAI_Y*MOVIMENT_ALT*6 + ESPAI_Y*2);
		pos_y = nivell + 2;
		pos_x = 2;
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_X; ++k) {
				pos_x = k*(MOVIMENT_PLA*6 + 2) + 2;
				pos_y = nivell + j*(ESPAI_Y*6 + 2) + 2;
				for(int u = 0; u < MOVIMENT_ALT; ++u) {
					for(int v = 0; v < MOVIMENT_PLA; ++v) {
						rectangle(representacio, Point(pos_x, pos_y), Point(pos_x+4, pos_y+4), s, -1, 8);
						pos_x += 6;
					}
					pos_x = k*(MOVIMENT_PLA*6 + 2) + 2;
					pos_y += 6;
				}
			} 
		}
	}
	imshow("Representaci� Histograma", representacio);
	printf("Temps representaci� histograma: %lf sec\n", (getTickCount() - start) / getTickFrequency());
}

/* Funci� que calcula l'histograma d'Optical Flow entre dos frames consecutius */
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
		// Discretitzem per direcci� del moviment (amb les dades del despla�ament de cada punt)
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
	representaHistograma();
	printf("Temps total Histograma Optical Flow: %lf sec\n", (getTickCount() - start) / getTickFrequency());
}