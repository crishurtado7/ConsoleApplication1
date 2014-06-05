#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "HistogramaOF.h"
#include "Constants.h"
#include "Classificador.h"

 using namespace cv;
 using namespace std;

 typedef pair<String, vector<HistogramaOF>> HOFrepeticions;

 int main( int argc, char** argv ) {
	 /* Càlcul de l'histograma acumulat per cada execució de l'activitat */
	 HistogramaOF hof = HistogramaOF();
	 Mat representacio = hof.representaHistograma();
	 imwrite("ReprHist.png", representacio);
	 imshow("Representacio", representacio);
	 float start = (float)getTickCount();
	 /*HistogramaOF HOF;
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
	 /*imageA = imread("Histograma_cut_1.png");
	 imshow("Res", imageA);*/


	/*for(int i = 0; i < NUMERO_ACTIVITATS; ++i) {
		 nomActivitat = activitats[i];
		 vec.clear();
		 for(int j = 0; j < REPETICIONS_ACTIVITAT; ++j) vec.push_back(HistogramaOF());
		 hofRep = make_pair(nomActivitat, vec);
		 HOFactivitats.push_back(hofRep);
		 path = "./Activitats/"+nomActivitat+"/";
		 //infile.open (path+"Dades.txt");
		 for(int j = 1; j <= REPETICIONS_ACTIVITAT; ++j) {
			 String p = path + to_string(j);
			 //getline(infile, num_imatges);
			//resultat = HOFactivitats.at(i).second.at(j-1).calcularHistogramaAcumulatOF(p, atoi(num_imatges.c_str()), nomActivitat, j);
			String nm = "Histograma_"+nomActivitat+"_"+to_string(j);
			//HOFactivitats.at(i).second.at(j-1).escriureFitxer(nm+".txt");
			HOFactivitats.at(i).second.at(j-1).llegirFitxer(nm+".txt");
			//imwrite(nm+".png", resultat);
		 }
		 infile.close();
	 }

	 Mat res = Mat::zeros(12, 12*27, CV_32F);
	 Mat auxx = Mat::zeros(1, 12*27, CV_32F);
	 Mat labels = Mat::zeros(12, 1, CV_32F);
	 int row = 0, pos = 0;
	 for(int i = 0; i < HOFactivitats.size(); ++i) {
		 for(int j = 0; j < (HOFactivitats.at(i).second.size() - 1); ++j) {
			 auxx = HOFactivitats.at(i).second.at(j).obteVector();
			 cout << "Vector obtingut" << endl;
			 for(int k = 0; k < auxx.cols; ++k) res.at<float>(row, k) = auxx.at<float>(0, k);
			 cout << "Vector passat a la matriu" << endl;
			 labels.at<int>(pos, 0) = i+1;
			 cout << "Label colocada" << endl;
			 ++pos;
			 ++row;
		 }
	 }
	 float lab[12] = {1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 3.0};
	Mat labelsMat(12, 1, CV_32FC1, lab);
	 cout << "Matriu entrenament muntada" << endl;
	 for(int i = 0; i < labelsMat.rows; ++i) {
		 for(int j = 0; j < labelsMat.cols; ++j) cout << labelsMat.at<float>(i, j) << endl;
	 }
	 Classificador classif = Classificador();
	 classif.entrenar(res, labelsMat);
	 cout << "Entrenament fet" << endl;
	 row = 0;
	 Mat test = Mat::zeros(3, 12*27, CV_32F);
	 for(int i = 0; i < HOFactivitats.size(); ++i) {
		 auxx = HOFactivitats.at(i).second.at(HOFactivitats.at(i).second.size()-1).obteVector();
		 for(int k = 0; k < auxx.cols; ++k) test.at<float>(row, k) = auxx.at<float>(0, k);
		 ++row;
	 }
	 Mat labRes = Mat::zeros(3, 1, CV_32F);
	 labRes = classif.classificar(test);
	 cout << "Classificació feta" << endl;
	 cout << "Rows: " << labRes.rows << endl;
	 cout << "Cols: " << labRes.cols << endl;
	 for(int i = 0; i < labRes.cols; ++i) {
		 for(int j = 0; j < labRes.rows; ++j) {
			 cout << "Label: " << labRes.at<float>(j, i) << endl;
		 }
	 }


	 HistogramaOF cut = HistogramaOF();
	 cut.mitjanaAritHOF(HOFactivitats.at(0).second, "cut");
	 cut.normalitzaHOF();
	 HOFactivitats.at(0).second.at(0).normalitzaHOF();
	 HOFactivitats.at(0).second.at(1).normalitzaHOF();
	 HOFactivitats.at(0).second.at(2).normalitzaHOF();
	 HOFactivitats.at(0).second.at(3).normalitzaHOF();
	 HOFactivitats.at(0).second.at(4).normalitzaHOF();

	 HistogramaOF eat = HistogramaOF();
	 eat.mitjanaAritHOF(HOFactivitats.at(1).second, "eat");
	 eat.normalitzaHOF();
	 HOFactivitats.at(1).second.at(0).normalitzaHOF();
	 HOFactivitats.at(1).second.at(1).normalitzaHOF();
	 HOFactivitats.at(1).second.at(2).normalitzaHOF();
	 HOFactivitats.at(1).second.at(3).normalitzaHOF();
	 HOFactivitats.at(1).second.at(4).normalitzaHOF();

	 HistogramaOF stir = HistogramaOF();
	 stir.mitjanaAritHOF(HOFactivitats.at(2).second, "stir");
	 stir.normalitzaHOF();
	 HOFactivitats.at(2).second.at(0).normalitzaHOF();
	 HOFactivitats.at(2).second.at(1).normalitzaHOF();
	 HOFactivitats.at(2).second.at(2).normalitzaHOF();
	 HOFactivitats.at(2).second.at(3).normalitzaHOF();
	 HOFactivitats.at(2).second.at(4).normalitzaHOF();

	 HistogramaOF aux = HistogramaOF();
	 cout << "Cut vs Cut" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(0), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(1), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(2), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(3), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(4), cut);
	 cout << "Cut vs Eat" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(0), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(1), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(2), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(3), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(4), eat);
	 cout << "Cut vs Stir" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(0), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(1), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(2), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(3), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(0).second.at(4), stir);

	 cout << "Eat vs Cut" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(0), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(1), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(2), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(3), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(4), cut);
	 cout << "Eat vs Eat" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(0), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(1), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(2), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(3), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(4), eat);
	 cout << "Eat vs Stir" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(0), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(1), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(2), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(3), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(1).second.at(4), stir);

	 cout << "Stir vs Cut" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(0), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(1), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(2), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(3), cut);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(4), cut);
	 cout << "Stir vs Eat" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(0), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(1), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(2), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(3), eat);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(4), eat);
	 cout << "Stir vs Stir" << endl;
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(0), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(1), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(2), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(3), stir);
	 aux.calculaGrauSimilitud(HOFactivitats.at(2).second.at(4), stir); 
	*/
	 printf("Temps total càlcul histogrames + comparacions amb grau de similitud: %lf sec\n", (getTickCount() - start) / getTickFrequency());

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}