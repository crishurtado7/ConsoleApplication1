#define _USE_MATH_DEFINES

#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "HistogramaOF.h"
#include "OpticalFlow.h"
#include "Constants.h"
#include <math.h>
#include <cstdio>

using namespace cv;
using namespace std;

//const float PI = 3.1415927;

/* Creadora */
HistogramaOF::HistogramaOF() { // Mirar si cal inicialitzar
	maxValor = 0;
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						dades[i][j][k][u][v] = 0;
					}
				}
			}
		}
	}
	/*for(int i = 0; i < MOVIMENT_PLA; ++i) {
		for(int j = 0; j < MOVIMENT_ALT; ++j) {
			dades[i][j] = 0;
		}
	}*/
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

/* Funció que discretitza un vector de posició segons la seva coordenada x a l'espai */
int HistogramaOF::discretitzaEspaiX(Point3i pos) {
	if(pos.x >= DIVISIO_X) return 0;
	else if(pos.x <= -DIVISIO_X) return 2;
	else return 1;
}

/* Funció que discretitza un vector de posició segons la seva coordenada y a l'espai */
int HistogramaOF::discretitzaEspaiY(Point3i pos) {
	if(pos.y <= DIVISIO_Y) return 2;
	else if(pos.y <= 2*DIVISIO_Y) return 1;
	else return 0;
}

/* Funció que discretitza un vector de posició segons la seva coordenada z a l'espai */
int HistogramaOF::discretitzaEspaiZ(Point3i pos) {
	if(pos.z <= DIVISIO_Z) return 2;
	else if(pos.z <= 2*DIVISIO_Z) return 1;
	else return 0;
}

/* Funció que calcula la intensitat del color que haurà de tenir l'histograma segons un dels seus valors */
int HistogramaOF::calculaFactor(int valor) {
	float perc = (float)valor/(float)maxValor;
	perc = perc*100;
	int res = perc;
	return res;
}

Mat HistogramaOF::representaHistograma() {
	float start = (float)getTickCount();
	int rows = ESPAI_X*MOVIMENT_PLA*6 + 2*ESPAI_X + 1;
	int cols = ESPAI_Z*ESPAI_Y*MOVIMENT_ALT*6 + 2*ESPAI_Z*ESPAI_Y + 1;
	int pos_x = 2; 
	int pos_y = 0;
	int nivell = 0;
	Scalar s;
	Mat representacio = Mat::zeros(cols, rows, CV_8UC3);
	int valor, factor;
	for(int i = 0; i < ESPAI_Z; ++i) {
		// Rang de valors: 155 - 255
		if(i == 0) s = Scalar(0,0,155); // Vermell
		else if(i == 1) s = Scalar(0,155,155); // Groc
		else s = Scalar(155,0,155); // Vermell
		Scalar sum;
		nivell = i*(ESPAI_Y*MOVIMENT_ALT*6 + ESPAI_Y*2);
		pos_y = nivell + 2;
		pos_x = 2;
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_X; ++k) {
				pos_x = k*(MOVIMENT_PLA*6 + 2) + 2;
				pos_y = nivell + j*(ESPAI_Y*6 + 2) + 2;
				for(int u = 0; u < MOVIMENT_ALT; ++u) {
					for(int v = 0; v < MOVIMENT_PLA; ++v) {
						// Aqui falta agafar les dades de l'histograma i modificar el valor de l'escalar del color
						valor = dades[k][j][i][v][u];
						factor = calculaFactor(valor);
						if(i == 0) sum = Scalar(0, 0, factor);
						else if(i == 1) sum = Scalar(0, factor, factor);
						else sum = Scalar(factor, 0, factor);
						add(s, sum, sum);
						rectangle(representacio, Point(pos_x, pos_y), Point(pos_x+4, pos_y+4), sum, -1, 8);
						pos_x += 6;
					}
					pos_x = k*(MOVIMENT_PLA*6 + 2) + 2;
					pos_y += 6;
				}
			} 
		}
	}
	//imshow("Representació Histograma", representacio);
	imwrite("representacio.jpg", representacio);
	printf("Temps representació histograma: %lf sec\n", (getTickCount() - start) / getTickFrequency());

	return representacio;
}

/* Funció que calcula l'histograma d'Optical Flow entre dos frames consecutius */
Mat HistogramaOF::calcularHistogramaOF(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d) {
	cout << "Calculant Histograma Optical Flow..." << endl;
	// Comencem a comptar el temps
	float start = (float)getTickCount();
	// Calculem l'Optical Flow entre els dos frames
	OpticalFlow OF = OpticalFlow();
	Mat OptFlow = OF.calcularOpticalFlow3D(frame1, frame2, frame1_d, frame2_d);
	vector<Point3i> ini = OF.getOpticalFlow3DInici();
	vector<Point3i> despl = OF.getOpticalFlow3DDespl();
	// Recorrem tots els punts calculats per l'Optical Flow
	int mov_pla, mov_alt, espai_x, espai_y, espai_z = 0;
	for(int i = 0; i < ini.size(); ++i) {
		// Discretitzem per posicio (agafant les dades de ini)
		Point3i init = ini.at(i);
		espai_x = discretitzaEspaiX(init);
		espai_y = discretitzaEspaiY(init);
		espai_z = discretitzaEspaiZ(init);
		// Discretitzem per direcció del moviment (amb les dades del desplaçament de cada punt)
		Point3i des = despl.at(i);
		mov_pla = discretitzaMovimentPla(des);
		mov_alt = discretitzaMovimentAlt(des);
		dades[espai_x][espai_y][espai_z][mov_pla][mov_alt]++;
		if(dades[espai_x][espai_y][espai_z][mov_pla][mov_alt] > maxValor) maxValor = dades[espai_x][espai_y][espai_z][mov_pla][mov_alt];
	}
	
	cout << "Histograma calculat" << endl;
	/*for(int i = 0; i < MOVIMENT_PLA; ++i) {
		for(int j = 0; j < MOVIMENT_ALT; ++j) {
			cout << dades[i][j] << "    ";
		}
		cout << endl;
	}*/
	cout << "Valor màxim: " << maxValor << endl;
	Mat representacio = representaHistograma();

	// Combinem la imatge amb la representació de l'Optical Flow 2D i la representació de l'histograma
	Mat img_matches(OptFlow.rows, OptFlow.cols + representacio.cols, OptFlow.type());
	Mat left(img_matches, Rect(0, 0, OptFlow.cols, OptFlow.rows)); // Copy constructor
	OptFlow.copyTo(left);
	Mat right(img_matches, Rect(OptFlow.cols, 0, representacio.cols, representacio.rows)); // Copy constructor
	representacio.copyTo(right);
	//imshow("Imatges", img_matches);
	cout << "Mida imatge: " << img_matches.cols << "  " << img_matches.rows << endl;
	printf("Temps total Histograma Optical Flow: %lf sec\n", (getTickCount() - start) / getTickFrequency());

	return img_matches;
}