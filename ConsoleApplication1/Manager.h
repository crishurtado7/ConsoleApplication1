#pragma once
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

ref class Manager {
public:
	bool com;
	bool acabat;
	Manager(void);
	void calculaHistogrames(int divX, int divY, int divZ, float mod, bool vid, std::string ruta);
	void carregaDesdeFitxer(std::string ruta);
	cv::Mat calculaConfusionMatrix(std::string path);
};

