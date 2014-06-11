#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

class Classificador {
public:
	//Funcions
	Classificador();
	void entrenar(Mat trainingData, Mat labels);
	Mat classificar(Mat sampleData);
private:
	// Atributs
	CvSVM Classify;
};

