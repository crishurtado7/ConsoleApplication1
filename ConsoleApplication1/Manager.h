#pragma once
#include <string>
#include <iostream>

ref class Manager {
public:
	bool com;
	bool acabat;
	Manager(void);
	void calculaHistogrames(int divX, int divY, int divZ, float mod, bool vid, std::string ruta);
	void carregaDesdeFitxer(std::string ruta);
	void calculaConfusionMatrix(std::string path);
};

