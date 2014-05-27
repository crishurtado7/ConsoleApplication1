#pragma once
#include "Resultats.h"
#include "Manager.h"

namespace ConsoleApplication1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de Progres
	/// </summary>
	public ref class Progres : public System::Windows::Forms::Form
	{
	private: System::Windows::Forms::Timer^  timer1;
	public:
		Manager f;
		Progres(void)
		{
			InitializeComponent();
			timer1->Start();
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén utilizando.
		/// </summary>
		~Progres()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 


	private: System::Windows::Forms::Label^  label2;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Variable del diseñador requerida.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido del método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(87, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(183, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Espera mentre es realitzen els càlculs";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(77, 44);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(207, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Aquest procés pot trigar uns quants minuts";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Progres::timer1_Tick);
			// 
			// Progres
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(369, 99);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"Progres";
			this->Text = L"Processant...";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 if(f.com == false) f.calculaHistogrames();
				 this->timer1->Stop();
				 Resultats^ r = gcnew Resultats();
				 this->Hide();
				 r->ShowDialog();
				 this->Close();
			 }
};
}
