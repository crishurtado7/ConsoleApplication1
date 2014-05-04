#include <iostream>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

const int MOVIMENT_PLA = 4;
const int MOVIMENT_ALT = 3;
const int ESPAI_X = 3;
const int ESPAI_Y = 3;
const int ESPAI_Z = 3;

typedef int ContingutHOF [ESPAI_X] [ESPAI_Y] [ESPAI_Z] [MOVIMENT_PLA] [MOVIMENT_ALT];
//typedef float ContingutHOF [MOVIMENT_PLA] [MOVIMENT_Z];

class HistogramaOF {
	public:
		// Atributs
		ContingutHOF dades; // Histograma d' Optical Flow
		int maxValor;
		// Funcions
		HistogramaOF();
		double calculaModul(Point3i p);
		int discretitzaMovimentPla(Point3i despl);
		int discretitzaMovimentAlt(Point3i despl);
		int discretitzaEspaiX(Point3i pos);
		int discretitzaEspaiY(Point3i pos);
		int discretitzaEspaiZ(Point3i pos);
		int calculaFactor(int valor);
		void representaHistograma();
		void calcularHistogramaOF(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d);
		// Algun mètode per calcular histogrames acumulats??

	private:
	
};

