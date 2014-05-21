#include "Classificador.h"


Classificador::Classificador() {
}

void Classificador::conjuntDades(Mat dades) {
}
	
void Classificador::entrenar(Mat trainingData, Mat labels) {
	CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	Classify.train(trainingData, labels, Mat(), Mat(), params);
}	

Mat Classificador::classificar(Mat sampleData) { 
	Mat results = Mat::zeros(3, 1, CV_32F);
	Classify.predict(sampleData, results);
	return results;
}