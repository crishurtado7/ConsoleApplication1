#include <iostream>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

const int MOVIMENT_PLA = 4;
const int MOVIMENT_ALT = 3;
const int ESPAI_X = 5;
const int ESPAI_Y = 3;
const int ESPAI_ALT = 3;

//typedef float ContingutHOF [ESPAI_X] [ESPAI_Y] [ESPAI_ALT] [MOVIMENT_PLA] [MOVIMENT_ALT];
typedef float ContingutHOF [MOVIMENT_PLA] [MOVIMENT_ALT];

class HistogramaOF {
	public:
		// Atributs
		ContingutHOF dades; // Histograma d' Optical Flow
		// Funcions
		HistogramaOF();
		double calculaModul(Point3i p);
		int discretitzaMovimentPla(Point3i despl);
		int discretitzaMovimentAlt(Point3i despl);
		void calcularHistogramaOF(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d);
		// Algun mètode per calcular histogrames acumulats??

	private:
	
};

