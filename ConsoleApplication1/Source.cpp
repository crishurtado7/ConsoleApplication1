#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

 using namespace cv;
 using namespace std;

 int main( int argc, char** argv ) {

	// Llegim el primer frame	
	Mat imageA;
	imageA = imread("logo.png", IMREAD_COLOR); // Read the file

	// Llegim el segon frame


	// Computem l'optical flow


	// Creem una finestra per mostrar el resultat

	namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
	imshow( "Display window", imageA ); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}