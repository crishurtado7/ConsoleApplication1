#pragma once
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "DataController.h"

ref class Manager {
public:
	// Atributs
	bool com;
	// Funcions
	Manager(void);
	void calculaHistogrames(bool vid, std::string ruta);
	void carregaDesdeFitxer(std::string ruta);
	cv::Mat calculaConfusionMatrix(std::string path);
	System::String^ llegirConfusionMatrix(std::string path);
private: 
	// Atributs
	bool acabat;
	DataController dc;
};

