#include "DataController.h"
#include <iostream>
#include <fstream>
#include "Constants.h"

/* Creadora */
DataController::DataController(void)
{
}

/* Funció que guarda les dades d'una confusion matrix en un fitxer de text */
void DataController::guardarConfusionMatrix(Mat confusionMatrix, string path) {
	 ofstream myfile;
	 myfile.open (path+"/ConfusionMatrix.txt");
	 myfile << "CONFUSION MATRIX";
	 myfile << "\nClasse Real/Classe Assignada";
	 myfile << "\n";
	 myfile << "\n        C0      C1      C2";
	 int total = 3*2;
	 int correctes = 0;
	 for(int i = 0; i < confusionMatrix.cols; ++i) {
		myfile << "\nC";
		myfile << i;
		for(int j = 0; j < confusionMatrix.rows; ++j) {
			myfile << "       ";
			myfile << confusionMatrix.at<int>(i, j);
			if(i == j) 
				correctes += confusionMatrix.at<int>(i, j);		
		}
	}
	myfile << "\n";
	myfile << "\nAccuracy: ";
	myfile << correctes/total;
	myfile.close();
}

/* Funció que guarda una imatge en un fitxer .png */
void DataController::guardarImatge(Mat image, string path, string name) {
	imwrite(path+"/"+name+".png", image);
}

/* Funció que guarda les dades d'un histograma en un fitxer de text */
void DataController::guardarHistograma(Mat histo, string path, string name) {
    ofstream out(name+".txt", ios::binary);
	for(int i = 0; i < histo.cols; ++i) {
		if(out.is_open()) out.write((char *)&histo.at<float>(0, i), sizeof(float));
	}
	out.close();
}

/* Funció que llegeix d'un fitxer els valors necessaris per construir un histograma de flux òptic */
Mat DataController::llegirHistograma(string path, string name) {
	ifstream in(name+".txt", ios::binary);
	int size = ESPAI_X*ESPAI_Y*ESPAI_Z*MOVIMENT_ALT*MOVIMENT_PLA;
	int pos = 0;
	Mat histo = Mat::zeros(1, size, CV_32FC1);
	for(int i = 0; i < size; ++i) {
		if(in.is_open()) in.read((char *)&histo.at<float>(0, i), sizeof(float));
	}
	in.close();
	return histo;
}

/* Funció que llegeix les dades d'una Confusion Matrix guardades en un fitxer i les guarda en un string */
string DataController::llegirConfMatrix(string path) {
	ifstream myfile;
	string res = "";
	myfile.open(path+"/ConfusionMatrix.txt");
	for (std::string line; std::getline(myfile, line); ) 
		res += line + "\r\n";
	myfile.close();
	return res;
}