#include "Manager.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "HistogramaOF.h"
#include "Constants.h"
#include "Progres.h"


Manager::Manager(void) {
	com = false;
}

typedef pair<String, vector<HistogramaOF>> HOFrepeticions;
void Manager::calculaHistogrames() {
		 /* Càlcul de l'histograma acumulat per cada execució de l'activitat */
	com = true;
	acabat = false;
	 float start = (float)getTickCount();
	 HistogramaOF HOF;
	 String activitats[] = {"cut", "eat", "stir"};
	 vector<HOFrepeticions> HOFactivitats;
	 Mat histogramaAcumulat;
	 String nomActivitat;
	 vector<HistogramaOF> vec;
	 HOFrepeticions hofRep;
	 String num_imatges, nomVideo, nomImatgeRGBa, nomImatgeDa, nomImatgeRGBb, nomImatgeDb;
	 //VideoWriter outputVideo;
	 //double fps = 15;
	 //CvSize mida = cvSize(399, 240);
	 Mat imageA, depthA, imageB, depthB, resultat;
	 String path;
	 ifstream infile;

	for(int i = 0; i < /*NUMERO_ACTIVITATS*/ 1; ++i) {
		 nomActivitat = activitats[i];
		 vec.clear();
		 for(int j = 0; j < REPETICIONS_ACTIVITAT; ++j) vec.push_back(HistogramaOF());
		 hofRep = make_pair(nomActivitat, vec);
		 HOFactivitats.push_back(hofRep);
		 path = "./Activitats/"+nomActivitat+"/";
		 infile.open (path+"Dades.txt");
		 for(int j = 1; j <= /*REPETICIONS_ACTIVITAT*/ 1; ++j) {
			 String p = path + to_string(j);
			 getline(infile, num_imatges);
			 resultat = HOFactivitats.at(i).second.at(j-1).calcularHistogramaAcumulatOF(p, atoi(num_imatges.c_str()), nomActivitat, j);
			String nm = "Histograma_"+nomActivitat+"_"+to_string(j);
			//HOFactivitats.at(i).second.at(j-1).escriureFitxer(nm+".txt");
			//HOFactivitats.at(i).second.at(j-1).llegirFitxer(nm+".txt");
			imwrite(nm+".png", resultat);
			
		 }
		 infile.close();
	 }
	acabat = true;
}
