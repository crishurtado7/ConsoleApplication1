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
#include <fstream>

using namespace cv;
using namespace std;

const int MOVIMENT_PLA = 4;
const int MOVIMENT_ALT = 3;
const int ESPAI_X = 3;
const int ESPAI_Y = 3;
const int ESPAI_Z = 3;


/* Creadora */
HistogramaOF::HistogramaOF() {
	inicialitzaHistograma();
}

/* Funci� per inicialitzar l'estructura de dades de l'histograma */
void HistogramaOF::inicialitzaHistograma() {
	maxValor = 0;
	sumaValors = 0;
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
}

/* Funci� que calcula el m�dul d'un vector */
float HistogramaOF::calculaModul(Point3i p) {
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
vector<float> HistogramaOF::discretitzaMovimentPla(Point3i despl) {
	// Si el gradient de x es positiu, sumem dreta. Si es negatiu, sumem esquerre (en valor absolut)
	// Si el gradient de y es positiu sumem endavant. Si es negatiu, sumem enrere (en valor absolut)
	vector<float> movPla;
	for(int i = 0; i < 4; ++i) movPla.push_back(0.0);
	if(despl.x >= 0) movPla.at(1) = despl.x;
	else movPla.at(3) = abs(despl.x);

	if(despl.y >= 0) movPla.at(0) = despl.y;
	else movPla.at(2) = abs(despl.y);
	//0 endavant, 1 dreta, 2 enrere, 3 esquerre
	return movPla;
}

/* Funci� que discretitza un vector de despla�ament segons el seu moviment en al�ada */
vector<float> HistogramaOF::discretitzaMovimentAlt(Point3i despl) {
	// C�lcul de l'angle de colatitud
	double angle = 0;
	if(despl.z > 0) angle = atan((sqrt(despl.x*despl.x + despl.y*despl.y))/despl.z)*180.0/M_PI;
	else if(despl.z == 0) angle = M_PI/2*180.0/M_PI;
	else angle = (M_PI + atan((sqrt(despl.x*despl.x + despl.y*despl.y))/despl.z))*180.0/M_PI;
	if(angle < 0) angle = angle + 360;
	int pos = 0;
	// Discretitzaci� de la posici� en 3 sectors
	vector<float> movAlt;
	float factor;
	float mod = abs(despl.z);
	for(int i = 0; i < 3; ++i) movAlt.push_back(0.0);
	if(angle < 30) movAlt.at(0) = mod; //Amunt
	else if(angle >= 30 && angle <= 90) { //Amunt - Pla
		factor = (float)calculaFactor(angle, 60, 30)/100;
		if(angle <= 60) {
			movAlt.at(1) = factor*mod;
			movAlt.at(0) = (1-factor)*mod;
		}
		else {
			movAlt.at(0) = factor*mod;
			movAlt.at(1) = (1-factor)*mod;
		}
	}
	else if(angle > 90 && angle <= 150) { //Pla - Avall
		factor = (float)calculaFactor(angle, 60, 90)/100;
		if(angle <= 120) {
			movAlt.at(2) = factor*mod;
			movAlt.at(1) = (1-factor*mod);
		}
		else {
			movAlt.at(1) = factor*mod;
			movAlt.at(2) = (1-factor)*mod;
		}
	}
	else movAlt.at(2) = mod; //Avall
	//0 amunt, 1 pla, 2 avall
	return movAlt;
}

/* Funci� que discretitza un vector de posici� segons la seva coordenada x a l'espai */
int HistogramaOF::discretitzaEspaiX(Point3i pos) {
	if(pos.x <= -DIVISIO_X) return 0;
	else if(pos.x >= DIVISIO_X) return 2;
	else return 1;
}

/* Funci� que discretitza un vector de posici� segons la seva coordenada y a l'espai */
int HistogramaOF::discretitzaEspaiY(Point3i pos) {
	if(pos.y <= DIVISIO_Y) return 2;
	else if(pos.y <= 2*DIVISIO_Y) return 1;
	else return 0;
}

/* Funci� que discretitza un vector de posici� segons la seva coordenada z a l'espai */
int HistogramaOF::discretitzaEspaiZ(Point3i pos) {
	if(pos.z <= DIVISIO_Z) return 2;
	else if(pos.z <= 2*DIVISIO_Z) return 1;
	else return 0;
}

/* Funci� que calcula la intensitat del color que haur� de tenir l'histograma segons un dels seus valors */
int HistogramaOF::calculaFactor(float valor, float maxim, float minim) {
	float perc = 0;
	if(maxim != 0) perc = (valor - minim)/maxim;
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
	float valor;
	int factor;
	for(int i = 0; i < ESPAI_Z; ++i) {
		// Rang de valors: 155 - 255
		if(i == 0) s = Scalar(0,0,155); // Vermell
		else if(i == 1) s = Scalar(0,155,155); // Groc
		else s = Scalar(155,0,155); // Lila
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
						factor = calculaFactor(valor, maxValor, 0);
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
	printf("Temps representaci� histograma: %lf sec\n", (getTickCount() - start) / getTickFrequency());
	repr = representacio;
	return representacio;
}

void HistogramaOF::construirHistograma() {
	maxValor = 0;
	sumaValors = 0;
	/*for(int i = 0; i < ESPAI_X; ++i) {*/
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						for(int l = 0; l < 10; ++l) {
							dades[0][j][k][u][v]++;
							if(dades[0][j][k][u][v] > maxValor) maxValor = dades[0][j][k][u][v];
							sumaValors++;
						}
					}
				}
			}
		}
	/*}*/
}

/* Funci� que calcula l'histograma d'Optical Flow entre dos frames consecutius */
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
	int espai_x, espai_y, espai_z = 0;
	vector<float> mov_pla, mov_alt;
	for(int i = 0; i < ini.size(); ++i) {
		// Discretitzem per posicio (agafant les dades de ini)
		Point3i init = ini.at(i);
		espai_x = discretitzaEspaiX(init);
		espai_y = discretitzaEspaiY(init);
		espai_z = discretitzaEspaiZ(init);
		// Discretitzem per direcci� del moviment (amb les dades del despla�ament de cada punt)
		Point3i des = despl.at(i);
		mov_pla = discretitzaMovimentPla(des);
		mov_alt = discretitzaMovimentAlt(des);
		for(int j = 0; j < mov_pla.size(); ++j) {
			for(int k = 0; k < mov_alt.size(); ++k) {
				if(mov_pla.at(j) > 0 && mov_alt.at(k) > 0) {
					/*float modul = sqrt(des.x*des.x + des.y*des.y + des.z*des.z);
					float valor = mov_pla.at(j)*mov_alt.at(k)*modul;*/
					//dades[espai_x][espai_y][espai_z][j][k] = valor;
					dades[espai_x][espai_y][espai_z][j][k] += mov_pla.at(j);
					dades[espai_x][espai_y][espai_z][j][k] += mov_alt.at(k);
					if(dades[espai_x][espai_y][espai_z][j][k] > maxValor) maxValor = dades[espai_x][espai_y][espai_z][j][k];
					sumaValors  = sumaValors + mov_pla.at(j) + mov_alt.at(k);
				}
			}
		}
	}
	
	cout << "Histograma calculat" << endl;
	
	Mat representacio = representaHistograma();

	// Combinem la imatge amb la representaci� de l'Optical Flow 2D i la representaci� de l'histograma
	Mat img_matches(OptFlow.rows, OptFlow.cols + representacio.cols, OptFlow.type());
	Mat left(img_matches, Rect(0, 0, OptFlow.cols, OptFlow.rows)); // Copy constructor
	OptFlow.copyTo(left);
	Mat right(img_matches, Rect(OptFlow.cols, 0, representacio.cols, representacio.rows)); // Copy constructor
	representacio.copyTo(right);

	printf("Temps total Histograma Optical Flow: %lf sec\n", (getTickCount() - start) / getTickFrequency());

	return img_matches;
}

/* Funci� que calcula l'histograma acumulat (per separat) de cada una de les execucions d'una activitat */
Mat HistogramaOF::calcularHistogramaAcumulatOF(String path, int num_imatges, String nom_activitat, int num_repeticio, string ruta, bool video) {
	String nomVideo, nomImatgeRGBa, nomImatgeDa, nomImatgeRGBb, nomImatgeDb;
	VideoWriter outputVideo;
	if(video) {
		double fps = 15;
		CvSize mida = cvSize(399, 240);
		nomVideo = ruta+"/"+nom_activitat+"_acumulat_"+to_string(num_repeticio)+".avi";
		outputVideo.open(nomVideo, 0, fps, mida, true);
	}
	Mat imageA, depthA, imageB, depthB, resultat;
	nomImatgeRGBa = path+"/c_0.png";
	nomImatgeDa = path+"/d_0.png";
	imageA = imread(nomImatgeRGBa, IMREAD_COLOR);
	depthA = imread(nomImatgeDa, IMREAD_GRAYSCALE);
	resultat = calcularHistogramaOF(imageA, imageA, depthA, depthA);
	if(video) outputVideo << resultat;
	for(int k = 1; k <= num_imatges; ++k) {
		cout << "Imatge: " << num_repeticio << " - " << k << "/" << num_imatges << endl;
		nomImatgeRGBb = path+"/c_"+to_string(k)+".png";
		nomImatgeDb = path+"/d_"+to_string(k)+".png";
		imageB = imread(nomImatgeRGBb, IMREAD_COLOR);
		depthB= imread(nomImatgeDb, IMREAD_GRAYSCALE);
		resultat = calcularHistogramaOF(imageA, imageB, depthA, depthB);
		if(video) outputVideo << resultat;
		nomImatgeRGBa = nomImatgeRGBb;
		nomImatgeDa = nomImatgeDb;
		imageB.copyTo(imageA);
		depthB.copyTo(depthA);
	}
	if(video) outputVideo.release();

	return repr;
}

void HistogramaOF::escriureFitxer(String nom_fitxer) {
	ofstream out(nom_fitxer, ios::binary);
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						if(out.is_open()) out.write((char *)&dades[i][j][k][u][v], sizeof(float));
					}
				}
			}
		}
	}
	out.close();
}

void HistogramaOF::llegirFitxer(String nom_fitxer) {
	maxValor = 0;
	sumaValors = 0;
	ifstream in(nom_fitxer, ios::binary);
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						if(in.is_open()) in.read((char *)&dades[i][j][k][u][v], sizeof(float));
						if(dades[i][j][k][u][v] > maxValor) maxValor = dades[i][j][k][u][v];
						sumaValors += dades[i][j][k][u][v];
					}
				}
			}
		}
	}
	in.close();
}

/* Funci� que normalitza els valors d'un histograma respecte a la suma total */
void HistogramaOF::normalitzaHOF() {
	maxValor = 0;
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						dades[i][j][k][u][v] = dades[i][j][k][u][v]/sumaValors;
						if(dades[i][j][k][u][v] > maxValor) maxValor = dades[i][j][k][u][v];
					}
				}
			}
		}
	}
}

/* Funci� que retorna un valor de l'histograma */
float HistogramaOF::getValor(int i, int j, int k, int u, int v) {
	return dades[i][j][k][u][v];
}

/* Funci� que suma a l'histograma de la classe els valors de l'histograma passat per par�metre */
void HistogramaOF::sumaHOF(HistogramaOF hof) {
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						dades[i][j][k][u][v] = dades[i][j][k][u][v] + hof.getValor(i, j, k, u, v);
						if(dades[i][j][k][u][v] > maxValor) maxValor = dades[i][j][k][u][v];
					}
				}
			}
		}
	}
}

/* Funci� que calcula el m�nim entre dos histogrames */
void HistogramaOF::minimHOF(HistogramaOF hof1, HistogramaOF hof2) {
	maxValor = 0;
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						dades[i][j][k][u][v] = min(hof1.getValor(i, j, k, u, v), hof2.getValor(i, j, k, u, v));
						if(dades[i][j][k][u][v] > maxValor) maxValor = dades[i][j][k][u][v];
					}
				}
			}
		}
	}
}

/* Funci� que calcula la mitjana aritm�tica entre tots els histogrames de ArrayHOF */
void HistogramaOF::mitjanaAritHOF(vector<HistogramaOF> ArrayHOF, String nom_activitat) {
	float valor = 0;
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						for(int h = 0; h < ArrayHOF.size(); ++h) valor += ArrayHOF.at(h).getValor(i, j, k, u, v);
						dades[i][j][k][u][v] = valor/ArrayHOF.size();
						if(dades[i][j][k][u][v] > maxValor) maxValor = dades[i][j][k][u][v];
						sumaValors += dades[i][j][k][u][v];
						valor = 0;
					}
				}
			}
		}
	}
	Mat repr = representaHistograma();
	//imshow("Histograma mitjana", repr);
	String nom = "Histograma_mitjana_"+nom_activitat+".png";
	imwrite(nom, repr);
}

/* Funci� que calcula el grau de similitud entre dos histogrames */
float HistogramaOF::calculaGrauSimilitud(HistogramaOF hof1, HistogramaOF hof2) {
	//normalitzaHOF();
	//hof.normalitzaHOF();
	minimHOF(hof1, hof2);
	float grauSimilitud = 0;
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						grauSimilitud += dades[i][j][k][u][v];
					}
				}
			}
		}
	}
	cout << "Grau de similitud: " << grauSimilitud << endl;
	return grauSimilitud;
}

Mat HistogramaOF::obteVector() {
	int size = SECTORS_X*SECTORS_Y*SECTORS_Z*MOVIMENT_ALT*MOVIMENT_PLA;
	int pos = 0;
	Mat dadesMat = Mat::zeros(1, size, CV_32FC1);
	for(int i = 0; i < ESPAI_X; ++i) {
		for(int j = 0; j < ESPAI_Y; ++j) {
			for(int k = 0; k < ESPAI_Z; ++k) {
				for(int u = 0; u < MOVIMENT_PLA; ++u) {
					for(int v = 0; v < MOVIMENT_ALT; ++v) {
						dadesMat.at<float>(0, pos) = dades[i][j][k][u][v];
						++pos;
					}
				}
			}
		}
	} 
	return dadesMat;
}