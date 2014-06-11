#pragma once
#include "Manager.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

namespace ConsoleApplication1 {

	using namespace System::Collections::Generic;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// Resumen de ConfusionMatrix
	/// </summary>
	public ref class ConfusionMatrix : public System::Windows::Forms::Form
	{
	public:
		ConfusionMatrix(void)
		{
			InitializeComponent();
		}
		
		void MarshalString ( System::String ^ s, std::string& os ) {
			using namespace Runtime::InteropServices;
			const char* chars = 
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
		}

		ConfusionMatrix(System::String^ path) {
			InitializeComponent();
			ifstream myfile;
			std::string p;
			MarshalString(path, p);
			Manager m;
			textBox1->Text = m.llegirConfusionMatrix(p);
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén utilizando.
		/// </summary>
		~ConfusionMatrix()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  textBox1;
	protected: 

	protected: 

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(541, 310);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Nova Prova";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(51, 35);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(481, 254);
			this->textBox1->TabIndex = 1;
			// 
			// ConfusionMatrix
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(628, 345);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Name = L"ConfusionMatrix";
			this->Text = L"ConfusionMatrix";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
