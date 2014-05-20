#include <iostream>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

const int MOVIMENT_PLA = 4;
const int MOVIMENT_ALT = 3;
const int ESPAI_X = 3;
const int ESPAI_Y = 3;
const int ESPAI_Z = 3;

typedef float ContingutHOF [ESPAI_X] [ESPAI_Y] [ESPAI_Z] [MOVIMENT_PLA] [MOVIMENT_ALT];
//typedef float ContingutHOF [MOVIMENT_PLA] [MOVIMENT_Z];

class HistogramaOF {
	public:
		// Atributs
		ContingutHOF dades; // Histograma d' Optical Flow
		float maxValor;
		float sumaValors;
		// Funcions
		HistogramaOF();
		void inicialitzaHistograma();
		float calculaModul(Point3i p);
		vector<float> discretitzaMovimentPla(Point3i despl);
		vector<float> discretitzaMovimentAlt(Point3i despl);
		int discretitzaEspaiX(Point3i pos);
		int discretitzaEspaiY(Point3i pos);
		int discretitzaEspaiZ(Point3i pos);
		int calculaFactor(float valor, float maxim, float minim);
		Mat representaHistograma();
		Mat calcularHistogramaOF(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d);
		Mat calcularHistogramaAcumulatOF(String path, int num_imatges, String nom_activitat, int num_repeticio);
		void normalitzaHOF();
		float getValor(int i, int j, int k, int u, int v);
		void sumaHOF(HistogramaOF hof);
		void minimHOF(HistogramaOF hof1, HistogramaOF hof2);
		void mitjanaAritHOF(vector<HistogramaOF> ArrayHOF, String nom_activitat);
		float calculaGrauSimilitud(HistogramaOF hof1, HistogramaOF hof2);
		Mat obteVector();

	private:
	
};

