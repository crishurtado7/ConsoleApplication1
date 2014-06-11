#pragma once
#include "SeleccionarParametres.h"
#include "SeleccionarCarpeta.h"


namespace ConsoleApplication1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de PantallaPrincipal
	/// </summary>
	public ref class PantallaPrincipal : public System::Windows::Forms::Form
	{
	public:
		PantallaPrincipal(void)
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
		~PantallaPrincipal()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 
	private: System::Windows::Forms::Button^  button2;

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
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(63, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(142, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Calcular histogrames";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &PantallaPrincipal::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(63, 51);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(142, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Càrrega desde fitxer";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &PantallaPrincipal::button2_Click);
			// 
			// PantallaPrincipal
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(273, 87);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"PantallaPrincipal";
			this->Text = L"Pantalla Principal";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 SeleccionarParametres^ r = gcnew SeleccionarParametres();
				 this->Hide();
				 r->ShowDialog();
				 this->Close();
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 SeleccionarCarpeta^ r = gcnew  SeleccionarCarpeta();
				 this->Hide();
				 r->ShowDialog();
				 this->Close();
			 }
	};
}
