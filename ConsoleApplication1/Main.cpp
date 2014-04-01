#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "OpticalFlow.h"

 using namespace cv;
 using namespace std;

 int main( int argc, char** argv ) {

	// Llegim el primer frame	
	printf("%s \n", "Llegint imatge A...");
	Mat imageA;
	imageA = imread("cut_0.png", IMREAD_COLOR); 

	// Llegim el segon frame
	printf("%s \n", "Llegint imatge B...");
	Mat imageB;
	imageB = imread("cut_1.png", IMREAD_COLOR);

	// Computem l'optical flow
	printf("%s \n", "Computant OpticalFlow...");
	OpticalFlow OF = OpticalFlow();
	OF.calcularOpticalFlow3D(imageA, imageB);

	// Creem una finestra per mostrar el resultat
	//printf("%s \n", "Creant nova finestra...");
	//cvNamedWindow( "Optical Flow", WINDOW_AUTOSIZE ); // Create a window for display.
	//imshow("Optical Flow", flow ); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}