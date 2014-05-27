#include "PantallaPrincipal.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread] 
void Main2(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ConsoleApplication1::PantallaPrincipal p;
	Application::Run(%p);
}


