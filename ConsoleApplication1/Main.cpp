#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "HistogramaOF.h"
#include "Constants.h"
#include "Classificador.h"
#include "OpticalFlow.h"

 using namespace cv;
 using namespace std;

 typedef pair<String, vector<HistogramaOF>> HOFrepeticions;

 int main( int argc, char** argv ) {
	 // Càlcul de histograma per cada frame
	 HistogramaOF HOF = HistogramaOF();
	 String activitats[] = {"cut", "eat", "stir"};
	 String num_imatges, nomVideo, nomImatgeRGBa, nomImatgeDa, nomImatgeRGBb, nomImatgeDb;
	 VideoWriter outputVideo;
	 double fps = 15;
	 IplImage* res;
	 CvSize mida = cvSize(399, 240);
	 Mat imageA, depthA, imageB, depthB, resultat;

	 for(int i = 0; i < NUMERO_ACTIVITATS; ++i) {
		 String p = "./Activitats/"+activitats[i]+"/";
		 ifstream infile;
		 infile.open (p+"Dades.txt");
		 for(int j = 1; j <= REPETICIONS_ACTIVITAT; ++j) {
			nomVideo = activitats[i]+"_"+to_string(j)+".avi";
			outputVideo.open(nomVideo, 0, fps, mida, true);
			getline(infile, num_imatges);
			nomImatgeRGBa = p+to_string(j)+"/c_0.png";
			nomImatgeDa = p+to_string(j)+"/d_0.png";
			imageA = imread(nomImatgeRGBa, IMREAD_COLOR);
			depthA = imread(nomImatgeDa, IMREAD_GRAYSCALE);
			HOF = HistogramaOF();
			resultat = HOF.calcularHistogramaOF(imageA, imageA, depthA, depthA);
			outputVideo << resultat;
			for(int k = 1; k <= atoi(num_imatges.c_str()); ++k) {
				cout << "Imatge: " << k << endl;
				nomImatgeRGBb = p+to_string(j)+"/c_"+to_string(k)+".png";
				nomImatgeDb = p+to_string(j)+"/d_"+to_string(k)+".png";
				imageB = imread(nomImatgeRGBb, IMREAD_COLOR);
				depthB= imread(nomImatgeDb, IMREAD_GRAYSCALE);
				HOF = HistogramaOF();
				resultat = HOF.calcularHistogramaOF(imageA, imageB, depthA, depthB);
				outputVideo << resultat;
				nomImatgeRGBa = nomImatgeRGBb;
				nomImatgeDa = nomImatgeDb;
				imageB.copyTo(imageA);
				depthB.copyTo(depthA);
			}
			outputVideo.release();
		 }
		 infile.close();
	 }

	 //printf("Temps total càlcul histogrames + comparacions amb grau de similitud: %lf sec\n", (getTickCount() - start) / getTickFrequency());

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}