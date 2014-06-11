#include "Classificador.h"

/* Creadora */
Classificador::Classificador() {
}

/* Funci� que entrena el classificador amb el training set */
void Classificador::entrenar(Mat trainingData, Mat labels) {
	CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	Classify.train(trainingData, labels, Mat(), Mat(), params);
}	

/* Funci� que retorna el resultat de la classificaci� del test set */
Mat Classificador::classificar(Mat sampleData) { 
	Mat results = Mat::zeros(3, 1, CV_32F);
	Classify.predict(sampleData, results);
	return results;
}