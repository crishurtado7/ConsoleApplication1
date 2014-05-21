#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

class Classificador {
public:
	// Atributs
	CvSVM Classify;

	//Funcions
	Classificador();
	void conjuntDades(Mat dades);
	void entrenar(Mat trainingData, Mat labels);
	Mat classificar(Mat sampleData);
};

