#pragma once
#include "Progres.h"
#include <iostream>
#include <fstream>
#include <string>

namespace ConsoleApplication1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de SeleccionarParametres
	/// </summary>
	public ref class SeleccionarParametres : public System::Windows::Forms::Form
	{
	public:
		SeleccionarParametres(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén utilizando.
		/// </summary>
		~SeleccionarParametres()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::CheckBox^  checkBox1;










	private: System::Windows::Forms::Button^  button1;








	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;



	protected: 

	private:
		/// <summary>
		/// Variable del diseñador requerida.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido del método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->SuspendLayout();
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(19, 73);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(177, 17);
			this->checkBox1->TabIndex = 1;
			this->checkBox1->Text = L"Gravar vídeo de les execucions";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(314, 95);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 12;
			this->button1->Text = L"Continuar";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &SeleccionarParametres::boto_Continuar);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(19, 34);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(61, 23);
			this->button2->TabIndex = 21;
			this->button2->Text = L"Explorar";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &SeleccionarParametres::button2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Enabled = false;
			this->textBox1->Location = System::Drawing::Point(86, 37);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(303, 20);
			this->textBox1->TabIndex = 22;
			this->textBox1->Text = L"C:\\Users\\Cris\\Desktop";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(16, 9);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(235, 13);
			this->label5->TabIndex = 23;
			this->label5->Text = L"Selecciona una carpeta on guardar els resultats:";
			// 
			// SeleccionarParametres
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(415, 125);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->checkBox1);
			this->Name = L"SeleccionarParametres";
			this->Text = L"Escollir opcions";
			this->Load += gcnew System::EventHandler(this, &SeleccionarParametres::SeleccionarParametres_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SeleccionarParametres_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
			 void MarshalString ( System::String ^ s, std::string& os ) {
					using namespace Runtime::InteropServices;
					const char* chars = 
					(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
					os = chars;
					Marshal::FreeHGlobal(IntPtr((void*)chars));
			 }
	private: System::Void boto_Continuar(System::Object^  sender, System::EventArgs^  e) {
				 std::string path;
				 MarshalString(textBox1->Text, path);
				 Progres^ r = gcnew Progres(checkBox1->Checked, path);
				 this->Hide();
				 r->ShowDialog();
				 this->Close();
			 }

private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->folderBrowserDialog1->ShowDialog();
			 this->textBox1->Text = this->folderBrowserDialog1->SelectedPath;
		 }
};
}
