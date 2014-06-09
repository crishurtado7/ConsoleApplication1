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
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  checkBox1;




	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::Button^  button1;




	private: System::Windows::Forms::NumericUpDown^  numericUpDown4;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown5;
	private: System::Windows::Forms::Label^  label8;
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown5 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(208, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Escull els valors dels paràmetres següents:";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(19, 132);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(177, 17);
			this->checkBox1->TabIndex = 1;
			this->checkBox1->Text = L"Gravar vídeo de les execucions";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(16, 47);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(48, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Divisió X";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(16, 73);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(48, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Divisió Y";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(16, 99);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(48, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Divisió Z";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(70, 45);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(46, 20);
			this->numericUpDown1->TabIndex = 9;
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(70, 71);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(46, 20);
			this->numericUpDown2->TabIndex = 10;
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(70, 97);
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(46, 20);
			this->numericUpDown3->TabIndex = 11;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(328, 226);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 12;
			this->button1->Text = L"Continuar";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &SeleccionarParametres::boto_Continuar);
			// 
			// numericUpDown4
			// 
			this->numericUpDown4->Location = System::Drawing::Point(193, 45);
			this->numericUpDown4->Name = L"numericUpDown4";
			this->numericUpDown4->Size = System::Drawing::Size(51, 20);
			this->numericUpDown4->TabIndex = 17;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(151, 47);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(36, 13);
			this->label7->TabIndex = 18;
			this->label7->Text = L"Mòdul";
			// 
			// numericUpDown5
			// 
			this->numericUpDown5->Location = System::Drawing::Point(193, 71);
			this->numericUpDown5->Name = L"numericUpDown5";
			this->numericUpDown5->Size = System::Drawing::Size(51, 20);
			this->numericUpDown5->TabIndex = 19;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(137, 73);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(51, 13);
			this->label8->TabIndex = 20;
			this->label8->Text = L"Punts OF";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(19, 174);
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
			this->textBox1->Location = System::Drawing::Point(86, 176);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(303, 20);
			this->textBox1->TabIndex = 22;
			this->textBox1->Text = L"C:\\Users\\Cris\\Desktop";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(19, 155);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(235, 13);
			this->label5->TabIndex = 23;
			this->label5->Text = L"Selecciona una carpeta on guardar els resultats:";
			// 
			// SeleccionarParametres
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(415, 261);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->numericUpDown5);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->numericUpDown4);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->numericUpDown3);
			this->Controls->Add(this->numericUpDown2);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->label1);
			this->Name = L"SeleccionarParametres";
			this->Text = L"Escollir paràmetres";
			this->Load += gcnew System::EventHandler(this, &SeleccionarParametres::SeleccionarParametres_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown5))->EndInit();
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
				 Progres^ r = gcnew Progres(int(numericUpDown1->Value), int(numericUpDown2->Value), int(numericUpDown3->Value), float(numericUpDown4->Value), checkBox1->Checked, path);
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
