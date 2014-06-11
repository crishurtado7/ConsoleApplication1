#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
ref class DataController
{
public:
	// Funcions
	DataController(void);
	void guardarConfusionMatrix(Mat confusionMatrix, string path);
	void guardarImatge(Mat image, string path, string name);
	void guardarHistograma(Mat histo, string path, string name);
	Mat llegirHistograma(string path, string name);
	string llegirConfMatrix(string path);
};

