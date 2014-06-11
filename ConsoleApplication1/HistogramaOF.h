#include <iostream>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

//typedef float ContingutHOF [ESPAI_X] [ESPAI_Y] [ESPAI_Z] [MOVIMENT_PLA] [MOVIMENT_ALT];
typedef float ContingutHOF [3] [3] [3] [4] [3];
//typedef float ContingutHOF [MOVIMENT_PLA] [MOVIMENT_Z];

class HistogramaOF {
	public:
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
		Mat calcularHistogramaAcumulatOF(String path, int num_imatges, String nom_activitat, int num_repeticio, string ruta, bool video);
		void llegirHistograma(Mat histo);
		float getValor(int i, int j, int k, int u, int v);
		Mat obteVector();

	private:
		// Atributs
		ContingutHOF dades; // Histograma d' Optical Flow
		float maxValor;
		float sumaValors;
		Mat repr;
	
};
