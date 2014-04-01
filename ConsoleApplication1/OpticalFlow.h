#include <iostream>
#include <opencv2/core/core.hpp>ç

using namespace cv;
using namespace std;

class OpticalFlow {
	public:
		// Atributs
			// Vectors de fluxe òptic
			// Quantitat de vectors
		// Funcions
			OpticalFlow();
			void drawArrow(Mat image, Point p, Point q, Scalar color, int arrowMagnitude, int thickness, int line_type, int shift);
			Mat calculaPuntsClau(Mat frame1, Mat frame2);
			void calcularOpticalFlow3D(Mat& frame1, Mat& frame2);
			void writeOpticalFlowToFile(Mat& flow, FILE* file);
			
	private:
			int calculaModul(Point p, Point q);

};