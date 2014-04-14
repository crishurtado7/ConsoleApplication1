#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "HistogramaOF.h"

 using namespace cv;
 using namespace std;

 int main( int argc, char** argv ) {

	// Llegim el primer frame	
	printf("%s \n", "Llegint imatge A...");
	Mat imageA, depthA;
	imageA = imread("cut_91.png", IMREAD_COLOR); 
	depthA = imread("cut_d_91.png", IMREAD_GRAYSCALE);

	// Llegim el segon frame
	printf("%s \n", "Llegint imatge B...");
	Mat imageB, depthB;
	imageB = imread("cut_92.png", IMREAD_COLOR);
	depthB = imread("cut_d_92.png", IMREAD_GRAYSCALE);
	
	// Computem l'optical flow
	printf("%s \n", "Computant OpticalFlow...");
	HistogramaOF HOF = HistogramaOF();
	HOF.calcularHistogramaOF(imageA, imageB, depthA, depthB);

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}