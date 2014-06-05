#include "Manager.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "HistogramaOF.h"
#include "Constants.h"
#include "Classificador.h"


Manager::Manager(void) {
	com = false;
}

typedef pair<String, vector<HistogramaOF>> HOFrepeticions;
vector<HOFrepeticions> HOFactivitats;

void Manager::calculaHistogrames(int divX, int divY, int divZ, float mod, bool vid, std::string ruta) {
		 /* Càlcul de l'histograma acumulat per cada execució de l'activitat */
	com = true;
	acabat = false;
	 float start = (float)getTickCount();
	 HistogramaOF HOF;
	 String activitats[] = {"cut", "eat", "stir"};
	 Mat histogramaAcumulat;
	 String nomActivitat;
	 vector<HistogramaOF> vec;
	 HOFrepeticions hofRep;
	 String num_imatges, nomVideo, nomImatgeRGBa, nomImatgeDa, nomImatgeRGBb, nomImatgeDb;
	 Mat imageA, depthA, imageB, depthB, resultat;
	 String path;
	 ifstream infile;

	for(int i = 0; i < NUMERO_ACTIVITATS; ++i) {
		 nomActivitat = activitats[i];
		 vec.clear();
		 for(int j = 0; j < REPETICIONS_ACTIVITAT; ++j) vec.push_back(HistogramaOF());
		 hofRep = make_pair(nomActivitat, vec);
		 HOFactivitats.push_back(hofRep);
		 path = "./Activitats/"+nomActivitat+"/";
		 infile.open (path+"Dades.txt");
		 for(int j = 1; j <= REPETICIONS_ACTIVITAT; ++j) {
			 String p = path + to_string(j);
			 getline(infile, num_imatges);
			 resultat = HOFactivitats.at(i).second.at(j-1).calcularHistogramaAcumulatOF(p, atoi(num_imatges.c_str()), nomActivitat, j, ruta, vid);
			String nm = "Histograma_acumulat_"+nomActivitat+"_"+to_string(j);
			HOFactivitats.at(i).second.at(j-1).escriureFitxer(nm+".txt");
			imwrite(ruta+"/"+nm+".png", resultat);
			
		 }
		 infile.close();
	 }
	acabat = true;
}

void Manager::carregaDesdeFitxer(std::string ruta) {
	 /* Càrrega dels histogrames desde fitxer */
	 float start = (float)getTickCount();
	 HistogramaOF HOF;
	 String activitats[] = {"cut", "eat", "stir"};
	 Mat resultat;
	 String nomActivitat;
	 vector<HistogramaOF> vec;
	 HOFrepeticions hofRep;

	for(int i = 0; i < NUMERO_ACTIVITATS; ++i) {
		 nomActivitat = activitats[i];
		 vec.clear();
		 for(int j = 0; j < REPETICIONS_ACTIVITAT; ++j) vec.push_back(HistogramaOF());
		 hofRep = make_pair(nomActivitat, vec);
		 HOFactivitats.push_back(hofRep);
		 for(int j = 1; j <= REPETICIONS_ACTIVITAT; ++j) {
			String nm = "Histograma_acumulat_"+nomActivitat+"_"+to_string(j);
			HOFactivitats.at(i).second.at(j-1).llegirFitxer(nm+".txt");
		 }
	 }
}

void Manager::calculaConfusionMatrix(std::string path) {
	 Mat res = Mat::zeros(9, 12*27, CV_32F);
	 Mat auxx = Mat::zeros(1, 12*27, CV_32F);
	 Mat labels = Mat::zeros(9, 1, CV_32F);
	 Mat test = Mat::zeros(6, 12*27, CV_32F);
	 int rowE = 0, posE = 0;
	 int rowT = 0;
	 int t = 0, t2 = 0;
	 for(int i = 0; i < HOFactivitats.size(); ++i) {
		 t = rand() % 5;
		 t2 = rand() % 5;
		 while(t == t2) t2 = rand() % 5;
		 for(int j = 0; j < (HOFactivitats.at(i).second.size()); ++j) {
			 auxx = HOFactivitats.at(i).second.at(j).obteVector();
			 for(int k = 0; k < auxx.cols; ++k) {
				 if(j != t && j != t2) res.at<float>(rowE, k) = auxx.at<float>(0, k);
				 else test.at<float>(rowT, k) = auxx.at<float>(0, k);
			 }
			 if(j != t && j != t2) {
				labels.at<int>(posE, 0) = i+1;
				++posE;
				++rowE;
			 }
			 else ++rowT;
		 }
	 }
	 float lab[9] = {1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0};
	 Mat labelsMat(9, 1, CV_32FC1, lab);

	 Classificador classif = Classificador();
	 classif.entrenar(res, labelsMat);

	 Mat labRes = Mat::zeros(6, 1, CV_32F);
	 labRes = classif.classificar(test);

	 for(int i = 0; i < labRes.cols; ++i) {
		 for(int j = 0; j < labRes.rows; ++j) {
			 cout << "Label: " << labRes.at<float>(j, i) << endl;
		 }
	 }
}
