#pragma once

// per-movie locks and saved choices
#include <map>

class Budget;   // бо в класі є поле Budget* budget
class Movie;    // бо є метод MakeMovieCard(const Movie&)
class Contract;        // ← додали
struct ContractDraft;


namespace Project12 {

	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;




	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ac_cb->CheckedChanged += gcnew System::EventHandler(this, &MyForm::ac_cb_CheckedChanged);

			// viewer-side: react to time-block change and Buy click
			this->cs_cb->SelectedIndexChanged += gcnew System::EventHandler(this, &Project12::MyForm::cs_cb_SelectedIndexChanged);
			this->Buy->Click += gcnew System::EventHandler(this, &Project12::MyForm::Buy_Click);

		}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e);


	private:int _signedDemand = -1;     
	private: System::Windows::Forms::ComboBox^ contracts_cb;
	private: System::Windows::Forms::Button^ applyContract_b;
	private: System::Windows::Forms::DateTimePicker^ DateT;
	private: System::Windows::Forms::Label^ DateLab;
	private: System::Windows::Forms::Label^ EmployMinMax;
	private: System::Windows::Forms::Label^ RelD_L;
	private: System::Windows::Forms::Label^ Author_L;
	private: System::Windows::Forms::Label^ RelDate_l;
	private: System::Windows::Forms::Label^ Auth_LL;









		   int _signedMovieCode = -1;

	private: int GetEffectiveDemand();


	private: bool _rebuildingHalls = false;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm() {
			if (components) delete components;
			delete budget; budget = nullptr;
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ View_B;
	private: System::Windows::Forms::Button^ Employee_B;
	private: System::Windows::Forms::Label^ Elabel1;


	private: System::Windows::Forms::FlowLayoutPanel^ moviesFlow;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::TextBox^ textBox2;



	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Button^ b_Emply;
	private: System::Windows::Forms::Label^ PIB_L;
	private: System::Windows::Forms::Label^ Code_L;
	private: System::Windows::Forms::Label^ Position_L;
	private: System::Windows::Forms::Label^ T_label;



	private: System::Windows::Forms::Label^ MB_L;
	private: System::Windows::Forms::Label^ Sal_L;
	private: System::Windows::Forms::Label^ Employ_L;



	private: System::Windows::Forms::Label^ S_label;



	private: System::Windows::Forms::Label^ Bud_label;
		   Budget* budget;
	private: System::Windows::Forms::Button^ Acc_B;

	private: System::Windows::Forms::ComboBox^ Emp_CB;



	private: System::Windows::Forms::Label^ M_label;
	private: System::Windows::Forms::Label^ TP_L;
	private: System::Windows::Forms::Button^ backb;
	private: System::Windows::Forms::Label^ Contr_L;
	private: System::Windows::Forms::Button^ AccC_B;
	private: System::Windows::Forms::Label^ Title_L;
	private: System::Windows::Forms::Label^ Genre_L;
	private: System::Windows::Forms::Label^ AgeR_L;
	private: System::Windows::Forms::Label^ DM_L;
	private: System::Windows::Forms::Label^ StD_L;
	private: System::Windows::Forms::Label^ EndD_L;
	private: System::Windows::Forms::Label^ Dem_L;
	private: System::Windows::Forms::TextBox^ Mt_text;
	private: System::Windows::Forms::TextBox^ Genre_text;
	private: System::Windows::Forms::TextBox^ AgeR_text;
	private: System::Windows::Forms::TextBox^ Durm_text;
	private: System::Windows::Forms::DateTimePicker^ Startd_picker;
	private: System::Windows::Forms::DateTimePicker^ Endd_picker;
	private: System::Windows::Forms::TextBox^ Demand_text;
	private: System::Windows::Forms::TextBox^ Price_text;



private: System::Windows::Forms::Label^ Pr_L;

private: System::Windows::Forms::Label^ Welcome_L;
private: System::Windows::Forms::Button^ Entry_b;
private: System::Windows::Forms::ComboBox^ Seans_cb;
private: System::Windows::Forms::ComboBox^ Hall_cb;


	private: // ==== Budget availability helper ====
		double GetAvailableTotal();

		// ==== Handlers ====
		System::Void Sal_text_Leave(System::Object^ sender, System::EventArgs^ e);

private: System::Windows::Forms::Label^ Seans_l;
private: System::Windows::Forms::Label^ Hall_l;






private: System::Windows::Forms::Button^ reqm_b;
private: System::Windows::Forms::CheckBox^ ac_cb;
private: System::Windows::Forms::CheckBox^ ad_cb;





private: System::Windows::Forms::Panel^ panel1;
private: System::Windows::Forms::Button^ AcceprM_B;





private: System::Windows::Forms::Label^ PriceCh_l;

private: System::Windows::Forms::Label^ GenreCh_L;
private: System::Windows::Forms::Label^ DurationCh_l;


private: System::Windows::Forms::Label^ DemCh_l;
private: System::Windows::Forms::Label^ Ageres_L;


private: System::Windows::Forms::Label^ TitleCh_L;

private: System::Windows::Forms::Label^ MovieCh_L;
private: System::Windows::Forms::Label^ Prich_l;

private: // --- антидубль для "дистриб’ютора" ---
	bool _awaitingDistrib = false;                             // чи вже чекаємо на відповідь
	int  _distribReqId = 0;                                    // номер активного запиту
	System::Windows::Forms::Timer^ _distribTimer = nullptr;    // єдиний таймер


private: System::Windows::Forms::Label^ MGch_l;

private: void UpdateViewerTicketPriceLabel();



private: System::Windows::Forms::Label^ DurCh_l;

private: System::Windows::Forms::Label^ Agr_L;

private: System::Windows::Forms::Label^ TCh_l;
private: System::Windows::Forms::Label^ Dch_l;
private: System::Windows::Forms::Button^ Buy;



private: System::Windows::Forms::ComboBox^ ch_cb;

private: System::Windows::Forms::ComboBox^ cs_cb;
private: System::Windows::Forms::ComboBox^ cpl_cb;


private: System::Windows::Forms::Label^ Ct_l;
private: System::Windows::Forms::Label^ Mt_l;




private: System::Windows::Forms::Label^ Hallc_l;
private: System::Windows::Forms::Label^ C_seans;
private: System::Windows::Forms::Label^ Cpl_l;




	   // Допоміжні методи
private: void PopulateContractsCombo();
private: void ApplySelectedContract();
private: System::Void ApplyContract_ClickShim(System::Object^ sender, System::EventArgs^ e);


private: System::Windows::Forms::Panel^ panel2;
private: System::Windows::Forms::TextBox^ T_text;
private: System::Windows::Forms::TextBox^ Sal_text;
private: System::Windows::Forms::Label^ BM_l;
private: System::Windows::Forms::Label^ Contr_LM;
private: System::Windows::Forms::Label^ PrefS_L;
private: System::Windows::Forms::Label^ PrefHall_L;



private: System::Windows::Forms::TextBox^ MB_text;

private: void RebuildHallComboForSelectedSeans();



	private: System::Windows::Forms::Panel^ MakeMovieCard(const Movie& m);

private: System::Void OnAcceptMovie(System::Object^ sender, System::EventArgs^ e);

private: System::Void OnDistributorReply(System::Object^ sender, System::EventArgs^ e);

private: System::Void ac_cb_CheckedChanged(System::Object^ sender, System::EventArgs^ e);

private: void UpdatePreferredSeansAndHall();

private: int selectedMovieCode = -1;
private: double remainingMovieBudget = -1.0; // залишок після покупок

void ShowMoviesForBudget(double maxBudget);

	protected:


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

	private: System::Void MB_text_Leave(System::Object^ sender, System::EventArgs^ e);
    private: System::Void T_text_Leave(System::Object^ sender, System::EventArgs^ e);





#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->moviesFlow = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->View_B = (gcnew System::Windows::Forms::Button());
			this->Employee_B = (gcnew System::Windows::Forms::Button());
			this->Elabel1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->b_Emply = (gcnew System::Windows::Forms::Button());
			this->PIB_L = (gcnew System::Windows::Forms::Label());
			this->Code_L = (gcnew System::Windows::Forms::Label());
			this->Position_L = (gcnew System::Windows::Forms::Label());
			this->T_label = (gcnew System::Windows::Forms::Label());
			this->MB_L = (gcnew System::Windows::Forms::Label());
			this->Sal_L = (gcnew System::Windows::Forms::Label());
			this->Employ_L = (gcnew System::Windows::Forms::Label());
			this->S_label = (gcnew System::Windows::Forms::Label());
			this->Bud_label = (gcnew System::Windows::Forms::Label());
			this->Acc_B = (gcnew System::Windows::Forms::Button());
			this->Emp_CB = (gcnew System::Windows::Forms::ComboBox());
			this->M_label = (gcnew System::Windows::Forms::Label());
			this->TP_L = (gcnew System::Windows::Forms::Label());
			this->backb = (gcnew System::Windows::Forms::Button());
			this->Contr_L = (gcnew System::Windows::Forms::Label());
			this->AccC_B = (gcnew System::Windows::Forms::Button());
			this->Title_L = (gcnew System::Windows::Forms::Label());
			this->Genre_L = (gcnew System::Windows::Forms::Label());
			this->AgeR_L = (gcnew System::Windows::Forms::Label());
			this->DM_L = (gcnew System::Windows::Forms::Label());
			this->StD_L = (gcnew System::Windows::Forms::Label());
			this->EndD_L = (gcnew System::Windows::Forms::Label());
			this->Dem_L = (gcnew System::Windows::Forms::Label());
			this->Mt_text = (gcnew System::Windows::Forms::TextBox());
			this->Genre_text = (gcnew System::Windows::Forms::TextBox());
			this->AgeR_text = (gcnew System::Windows::Forms::TextBox());
			this->Durm_text = (gcnew System::Windows::Forms::TextBox());
			this->Startd_picker = (gcnew System::Windows::Forms::DateTimePicker());
			this->Endd_picker = (gcnew System::Windows::Forms::DateTimePicker());
			this->Demand_text = (gcnew System::Windows::Forms::TextBox());
			this->Price_text = (gcnew System::Windows::Forms::TextBox());
			this->Pr_L = (gcnew System::Windows::Forms::Label());
			this->Welcome_L = (gcnew System::Windows::Forms::Label());
			this->Entry_b = (gcnew System::Windows::Forms::Button());
			this->Seans_cb = (gcnew System::Windows::Forms::ComboBox());
			this->Hall_cb = (gcnew System::Windows::Forms::ComboBox());
			this->Seans_l = (gcnew System::Windows::Forms::Label());
			this->Hall_l = (gcnew System::Windows::Forms::Label());
			this->reqm_b = (gcnew System::Windows::Forms::Button());
			this->ac_cb = (gcnew System::Windows::Forms::CheckBox());
			this->ad_cb = (gcnew System::Windows::Forms::CheckBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->Prich_l = (gcnew System::Windows::Forms::Label());
			this->MGch_l = (gcnew System::Windows::Forms::Label());
			this->DurCh_l = (gcnew System::Windows::Forms::Label());
			this->Agr_L = (gcnew System::Windows::Forms::Label());
			this->TCh_l = (gcnew System::Windows::Forms::Label());
			this->Dch_l = (gcnew System::Windows::Forms::Label());
			this->AcceprM_B = (gcnew System::Windows::Forms::Button());
			this->PriceCh_l = (gcnew System::Windows::Forms::Label());
			this->GenreCh_L = (gcnew System::Windows::Forms::Label());
			this->DurationCh_l = (gcnew System::Windows::Forms::Label());
			this->DemCh_l = (gcnew System::Windows::Forms::Label());
			this->Ageres_L = (gcnew System::Windows::Forms::Label());
			this->TitleCh_L = (gcnew System::Windows::Forms::Label());
			this->MovieCh_L = (gcnew System::Windows::Forms::Label());
			this->Buy = (gcnew System::Windows::Forms::Button());
			this->ch_cb = (gcnew System::Windows::Forms::ComboBox());
			this->cs_cb = (gcnew System::Windows::Forms::ComboBox());
			this->cpl_cb = (gcnew System::Windows::Forms::ComboBox());
			this->Ct_l = (gcnew System::Windows::Forms::Label());
			this->Mt_l = (gcnew System::Windows::Forms::Label());
			this->Hallc_l = (gcnew System::Windows::Forms::Label());
			this->C_seans = (gcnew System::Windows::Forms::Label());
			this->Cpl_l = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->T_text = (gcnew System::Windows::Forms::TextBox());
			this->Sal_text = (gcnew System::Windows::Forms::TextBox());
			this->MB_text = (gcnew System::Windows::Forms::TextBox());
			this->BM_l = (gcnew System::Windows::Forms::Label());
			this->Contr_LM = (gcnew System::Windows::Forms::Label());
			this->PrefS_L = (gcnew System::Windows::Forms::Label());
			this->PrefHall_L = (gcnew System::Windows::Forms::Label());
			this->contracts_cb = (gcnew System::Windows::Forms::ComboBox());
			this->applyContract_b = (gcnew System::Windows::Forms::Button());
			this->DateT = (gcnew System::Windows::Forms::DateTimePicker());
			this->DateLab = (gcnew System::Windows::Forms::Label());
			this->EmployMinMax = (gcnew System::Windows::Forms::Label());
			this->Auth_LL = (gcnew System::Windows::Forms::Label());
			this->RelDate_l = (gcnew System::Windows::Forms::Label());
			this->Author_L = (gcnew System::Windows::Forms::Label());
			this->RelD_L = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// moviesFlow
			// 
			this->moviesFlow->AutoScroll = true;
			this->moviesFlow->BackColor = System::Drawing::Color::LightCyan;
			this->moviesFlow->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->moviesFlow->ForeColor = System::Drawing::Color::MidnightBlue;
			this->moviesFlow->Location = System::Drawing::Point(13, 125);
			this->moviesFlow->Name = L"moviesFlow";
			this->moviesFlow->Size = System::Drawing::Size(360, 420);
			this->moviesFlow->TabIndex = 0;
			this->moviesFlow->Visible = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Broadway", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->label1->Location = System::Drawing::Point(40, 151);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(285, 68);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Cinema";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// View_B
			// 
			this->View_B->BackColor = System::Drawing::Color::DarkSlateGray;
			this->View_B->Cursor = System::Windows::Forms::Cursors::Hand;
			this->View_B->FlatAppearance->BorderColor = System::Drawing::Color::PapayaWhip;
			this->View_B->FlatAppearance->BorderSize = 3;
			this->View_B->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->View_B->Font = (gcnew System::Drawing::Font(L"Broadway", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->View_B->ForeColor = System::Drawing::Color::PapayaWhip;
			this->View_B->Location = System::Drawing::Point(12, 368);
			this->View_B->Name = L"View_B";
			this->View_B->Size = System::Drawing::Size(120, 47);
			this->View_B->TabIndex = 1;
			this->View_B->Text = L"Viewer";
			this->View_B->UseVisualStyleBackColor = false;
			this->View_B->Click += gcnew System::EventHandler(this, &MyForm::View_B_Click);
			// 
			// Employee_B
			// 
			this->Employee_B->BackColor = System::Drawing::Color::DarkSlateGray;
			this->Employee_B->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->Employee_B->Cursor = System::Windows::Forms::Cursors::Hand;
			this->Employee_B->FlatAppearance->BorderSize = 3;
			this->Employee_B->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->Employee_B->Font = (gcnew System::Drawing::Font(L"Broadway", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Employee_B->ForeColor = System::Drawing::Color::PapayaWhip;
			this->Employee_B->Location = System::Drawing::Point(250, 368);
			this->Employee_B->Name = L"Employee_B";
			this->Employee_B->Size = System::Drawing::Size(139, 47);
			this->Employee_B->TabIndex = 2;
			this->Employee_B->Text = L"Employee";
			this->Employee_B->UseVisualStyleBackColor = false;
			this->Employee_B->Click += gcnew System::EventHandler(this, &MyForm::Employee_B_Click);
			// 
			// Elabel1
			// 
			this->Elabel1->AutoSize = true;
			this->Elabel1->BackColor = System::Drawing::Color::Transparent;
			this->Elabel1->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 16.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Elabel1->ForeColor = System::Drawing::Color::Transparent;
			this->Elabel1->Location = System::Drawing::Point(64, 12);
			this->Elabel1->Name = L"Elabel1";
			this->Elabel1->Size = System::Drawing::Size(198, 39);
			this->Elabel1->TabIndex = 3;
			this->Elabel1->Text = L"Registration";
			this->Elabel1->Visible = false;
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::Color::DarkSlateGray;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->textBox1->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->textBox1->Location = System::Drawing::Point(9, 87);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(380, 35);
			this->textBox1->TabIndex = 4;
			this->textBox1->Visible = false;
			// 
			// textBox2
			// 
			this->textBox2->BackColor = System::Drawing::Color::DarkSlateGray;
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->textBox2->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->textBox2->Location = System::Drawing::Point(15, 171);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 35);
			this->textBox2->TabIndex = 5;
			this->textBox2->Visible = false;
			// 
			// comboBox1
			// 
			this->comboBox1->BackColor = System::Drawing::Color::DarkSlateGray;
			this->comboBox1->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->comboBox1->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Accountant", L"Manager", L"Cinemacoordinator" });
			this->comboBox1->Location = System::Drawing::Point(12, 260);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(234, 36);
			this->comboBox1->TabIndex = 6;
			this->comboBox1->Visible = false;
			// 
			// b_Emply
			// 
			this->b_Emply->BackColor = System::Drawing::Color::DarkSlateGray;
			this->b_Emply->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->b_Emply->ForeColor = System::Drawing::SystemColors::Menu;
			this->b_Emply->Location = System::Drawing::Point(209, 135);
			this->b_Emply->Name = L"b_Emply";
			this->b_Emply->Size = System::Drawing::Size(116, 77);
			this->b_Emply->TabIndex = 7;
			this->b_Emply->Text = L"Accept";
			this->b_Emply->UseVisualStyleBackColor = false;
			this->b_Emply->Visible = false;
			this->b_Emply->Click += gcnew System::EventHandler(this, &MyForm::b_Emply_Click);
			// 
			// PIB_L
			// 
			this->PIB_L->AutoSize = true;
			this->PIB_L->BackColor = System::Drawing::Color::Transparent;
			this->PIB_L->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->PIB_L->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->PIB_L->Location = System::Drawing::Point(6, 59);
			this->PIB_L->Name = L"PIB_L";
			this->PIB_L->Size = System::Drawing::Size(83, 24);
			this->PIB_L->TabIndex = 8;
			this->PIB_L->Text = L"Fullname";
			this->PIB_L->Visible = false;
			// 
			// Code_L
			// 
			this->Code_L->AutoSize = true;
			this->Code_L->BackColor = System::Drawing::Color::Transparent;
			this->Code_L->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Code_L->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->Code_L->Location = System::Drawing::Point(9, 140);
			this->Code_L->Name = L"Code_L";
			this->Code_L->Size = System::Drawing::Size(55, 24);
			this->Code_L->TabIndex = 9;
			this->Code_L->Text = L"Code";
			this->Code_L->Visible = false;
			// 
			// Position_L
			// 
			this->Position_L->AutoSize = true;
			this->Position_L->BackColor = System::Drawing::Color::Transparent;
			this->Position_L->Font = (gcnew System::Drawing::Font(L"Kristen ITC", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Position_L->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->Position_L->Location = System::Drawing::Point(9, 226);
			this->Position_L->Name = L"Position_L";
			this->Position_L->Size = System::Drawing::Size(78, 24);
			this->Position_L->TabIndex = 10;
			this->Position_L->Text = L"Position";
			this->Position_L->Visible = false;
			// 
			// T_label
			// 
			this->T_label->AutoSize = true;
			this->T_label->BackColor = System::Drawing::Color::Transparent;
			this->T_label->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->T_label->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->T_label->Location = System::Drawing::Point(205, 290);
			this->T_label->Name = L"T_label";
			this->T_label->Size = System::Drawing::Size(0, 23);
			this->T_label->TabIndex = 11;
			this->T_label->Visible = false;
			// 
			// MB_L
			// 
			this->MB_L->AutoSize = true;
			this->MB_L->BackColor = System::Drawing::Color::Transparent;
			this->MB_L->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MB_L->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->MB_L->Location = System::Drawing::Point(6, 48);
			this->MB_L->Name = L"MB_L";
			this->MB_L->Size = System::Drawing::Size(132, 20);
			this->MB_L->TabIndex = 14;
			this->MB_L->Text = L"Movie budget";
			this->MB_L->Visible = false;
			// 
			// Sal_L
			// 
			this->Sal_L->AutoSize = true;
			this->Sal_L->BackColor = System::Drawing::Color::Transparent;
			this->Sal_L->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Sal_L->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->Sal_L->Location = System::Drawing::Point(7, 135);
			this->Sal_L->Name = L"Sal_L";
			this->Sal_L->Size = System::Drawing::Size(68, 20);
			this->Sal_L->TabIndex = 15;
			this->Sal_L->Text = L"Salary";
			this->Sal_L->Visible = false;
			// 
			// Employ_L
			// 
			this->Employ_L->AutoSize = true;
			this->Employ_L->BackColor = System::Drawing::Color::Transparent;
			this->Employ_L->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Employ_L->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->Employ_L->Location = System::Drawing::Point(172, 135);
			this->Employ_L->Name = L"Employ_L";
			this->Employ_L->Size = System::Drawing::Size(111, 23);
			this->Employ_L->TabIndex = 16;
			this->Employ_L->Text = L"Employee";
			this->Employ_L->Visible = false;
			// 
			// S_label
			// 
			this->S_label->AutoSize = true;
			this->S_label->Font = (gcnew System::Drawing::Font(L"MingLiU-ExtB", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->S_label->ForeColor = System::Drawing::Color::Navy;
			this->S_label->Location = System::Drawing::Point(854, 21);
			this->S_label->Name = L"S_label";
			this->S_label->Size = System::Drawing::Size(0, 23);
			this->S_label->TabIndex = 17;
			this->S_label->Visible = false;
			// 
			// Bud_label
			// 
			this->Bud_label->AutoSize = true;
			this->Bud_label->BackColor = System::Drawing::Color::Transparent;
			this->Bud_label->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Bud_label->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->Bud_label->Location = System::Drawing::Point(14, 12);
			this->Bud_label->Name = L"Bud_label";
			this->Bud_label->Size = System::Drawing::Size(44, 23);
			this->Bud_label->TabIndex = 19;
			this->Bud_label->Text = L"grn";
			this->Bud_label->Visible = false;
			// 
			// Acc_B
			// 
			this->Acc_B->BackColor = System::Drawing::Color::SteelBlue;
			this->Acc_B->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Acc_B->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->Acc_B->Location = System::Drawing::Point(158, 464);
			this->Acc_B->Name = L"Acc_B";
			this->Acc_B->Size = System::Drawing::Size(125, 87);
			this->Acc_B->TabIndex = 20;
			this->Acc_B->Text = L"Accept";
			this->Acc_B->UseVisualStyleBackColor = false;
			this->Acc_B->Visible = false;
			this->Acc_B->Click += gcnew System::EventHandler(this, &MyForm::Acc_B_Click);
			// 
			// Emp_CB
			// 
			this->Emp_CB->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->Emp_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Emp_CB->FormattingEnabled = true;
			this->Emp_CB->Location = System::Drawing::Point(175, 162);
			this->Emp_CB->Name = L"Emp_CB";
			this->Emp_CB->Size = System::Drawing::Size(321, 28);
			this->Emp_CB->TabIndex = 36;
			this->Emp_CB->Visible = false;
			// 
			// M_label
			// 
			this->M_label->AutoSize = true;
			this->M_label->BackColor = System::Drawing::Color::Transparent;
			this->M_label->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->M_label->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->M_label->Location = System::Drawing::Point(211, 83);
			this->M_label->Name = L"M_label";
			this->M_label->Size = System::Drawing::Size(85, 23);
			this->M_label->TabIndex = 39;
			this->M_label->Text = L"label12";
			this->M_label->Visible = false;
			// 
			// TP_L
			// 
			this->TP_L->AutoSize = true;
			this->TP_L->BackColor = System::Drawing::Color::Transparent;
			this->TP_L->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TP_L->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->TP_L->Location = System::Drawing::Point(7, 250);
			this->TP_L->Name = L"TP_L";
			this->TP_L->Size = System::Drawing::Size(120, 20);
			this->TP_L->TabIndex = 40;
			this->TP_L->Text = L"Ticket price";
			this->TP_L->Visible = false;
			// 
			// backb
			// 
			this->backb->BackColor = System::Drawing::Color::DarkSlateGray;
			this->backb->FlatAppearance->BorderColor = System::Drawing::Color::LightGray;
			this->backb->Font = (gcnew System::Drawing::Font(L"Comic Sans MS", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->backb->ForeColor = System::Drawing::Color::LightGray;
			this->backb->Location = System::Drawing::Point(531, 11);
			this->backb->Name = L"backb";
			this->backb->Size = System::Drawing::Size(57, 39);
			this->backb->TabIndex = 41;
			this->backb->Text = L"<=";
			this->backb->UseVisualStyleBackColor = false;
			this->backb->Click += gcnew System::EventHandler(this, &MyForm::backb_Click);
			// 
			// Contr_L
			// 
			this->Contr_L->AutoSize = true;
			this->Contr_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->Contr_L->Location = System::Drawing::Point(104, 5);
			this->Contr_L->Name = L"Contr_L";
			this->Contr_L->Size = System::Drawing::Size(97, 27);
			this->Contr_L->TabIndex = 42;
			this->Contr_L->Text = L"Contract";
			this->Contr_L->Visible = false;
			// 
			// AccC_B
			// 
			this->AccC_B->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->AccC_B->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->AccC_B->ForeColor = System::Drawing::Color::DarkBlue;
			this->AccC_B->Location = System::Drawing::Point(511, 325);
			this->AccC_B->Name = L"AccC_B";
			this->AccC_B->Size = System::Drawing::Size(124, 71);
			this->AccC_B->TabIndex = 43;
			this->AccC_B->Text = L"Accept";
			this->AccC_B->UseVisualStyleBackColor = false;
			this->AccC_B->Visible = false;
			this->AccC_B->Click += gcnew System::EventHandler(this, &MyForm::AccC_B_Click);
			// 
			// Title_L
			// 
			this->Title_L->AutoSize = true;
			this->Title_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->Title_L->Location = System::Drawing::Point(192, 38);
			this->Title_L->Name = L"Title_L";
			this->Title_L->Size = System::Drawing::Size(122, 27);
			this->Title_L->TabIndex = 44;
			this->Title_L->Text = L"Movie title";
			this->Title_L->Visible = false;
			// 
			// Genre_L
			// 
			this->Genre_L->AutoSize = true;
			this->Genre_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->Genre_L->Location = System::Drawing::Point(197, 89);
			this->Genre_L->Name = L"Genre_L";
			this->Genre_L->Size = System::Drawing::Size(72, 27);
			this->Genre_L->TabIndex = 45;
			this->Genre_L->Text = L"Genre";
			this->Genre_L->Visible = false;
			// 
			// AgeR_L
			// 
			this->AgeR_L->AutoSize = true;
			this->AgeR_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->AgeR_L->Location = System::Drawing::Point(185, 122);
			this->AgeR_L->Name = L"AgeR_L";
			this->AgeR_L->Size = System::Drawing::Size(124, 27);
			this->AgeR_L->TabIndex = 46;
			this->AgeR_L->Text = L"Age Rating";
			this->AgeR_L->Visible = false;
			// 
			// DM_L
			// 
			this->DM_L->AutoSize = true;
			this->DM_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->DM_L->Location = System::Drawing::Point(192, 159);
			this->DM_L->Name = L"DM_L";
			this->DM_L->Size = System::Drawing::Size(189, 27);
			this->DM_L->TabIndex = 47;
			this->DM_L->Text = L"Duration minutes";
			this->DM_L->Visible = false;
			// 
			// StD_L
			// 
			this->StD_L->AutoSize = true;
			this->StD_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->StD_L->Location = System::Drawing::Point(185, 200);
			this->StD_L->Name = L"StD_L";
			this->StD_L->Size = System::Drawing::Size(111, 27);
			this->StD_L->TabIndex = 48;
			this->StD_L->Text = L"Start date";
			this->StD_L->Visible = false;
			// 
			// EndD_L
			// 
			this->EndD_L->AutoSize = true;
			this->EndD_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->EndD_L->Location = System::Drawing::Point(192, 243);
			this->EndD_L->Name = L"EndD_L";
			this->EndD_L->Size = System::Drawing::Size(100, 27);
			this->EndD_L->TabIndex = 49;
			this->EndD_L->Text = L"End date";
			this->EndD_L->Visible = false;
			// 
			// Dem_L
			// 
			this->Dem_L->AutoSize = true;
			this->Dem_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->Dem_L->Location = System::Drawing::Point(190, 281);
			this->Dem_L->Name = L"Dem_L";
			this->Dem_L->Size = System::Drawing::Size(98, 27);
			this->Dem_L->TabIndex = 50;
			this->Dem_L->Text = L"Demand";
			this->Dem_L->Visible = false;
			// 
			// Mt_text
			// 
			this->Mt_text->Location = System::Drawing::Point(19, 38);
			this->Mt_text->Name = L"Mt_text";
			this->Mt_text->Size = System::Drawing::Size(129, 33);
			this->Mt_text->TabIndex = 51;
			this->Mt_text->Visible = false;
			// 
			// Genre_text
			// 
			this->Genre_text->Location = System::Drawing::Point(19, 86);
			this->Genre_text->Name = L"Genre_text";
			this->Genre_text->Size = System::Drawing::Size(129, 33);
			this->Genre_text->TabIndex = 52;
			this->Genre_text->Visible = false;
			// 
			// AgeR_text
			// 
			this->AgeR_text->Location = System::Drawing::Point(19, 116);
			this->AgeR_text->Name = L"AgeR_text";
			this->AgeR_text->Size = System::Drawing::Size(129, 33);
			this->AgeR_text->TabIndex = 53;
			this->AgeR_text->Visible = false;
			// 
			// Durm_text
			// 
			this->Durm_text->Location = System::Drawing::Point(20, 156);
			this->Durm_text->Name = L"Durm_text";
			this->Durm_text->Size = System::Drawing::Size(129, 33);
			this->Durm_text->TabIndex = 54;
			this->Durm_text->Visible = false;
			// 
			// Startd_picker
			// 
			this->Startd_picker->CustomFormat = L"yyyy-MM-dd";
			this->Startd_picker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->Startd_picker->Location = System::Drawing::Point(3, 194);
			this->Startd_picker->Name = L"Startd_picker";
			this->Startd_picker->Size = System::Drawing::Size(165, 33);
			this->Startd_picker->TabIndex = 74;
			this->Startd_picker->Visible = false;
			// 
			// Endd_picker
			// 
			this->Endd_picker->CustomFormat = L"yyyy-MM-dd";
			this->Endd_picker->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->Endd_picker->Location = System::Drawing::Point(3, 243);
			this->Endd_picker->Name = L"Endd_picker";
			this->Endd_picker->Size = System::Drawing::Size(165, 33);
			this->Endd_picker->TabIndex = 73;
			this->Endd_picker->Visible = false;
			// 
			// Demand_text
			// 
			this->Demand_text->Location = System::Drawing::Point(20, 278);
			this->Demand_text->Name = L"Demand_text";
			this->Demand_text->Size = System::Drawing::Size(129, 33);
			this->Demand_text->TabIndex = 57;
			this->Demand_text->Visible = false;
			// 
			// Price_text
			// 
			this->Price_text->Location = System::Drawing::Point(14, 327);
			this->Price_text->Name = L"Price_text";
			this->Price_text->Size = System::Drawing::Size(129, 33);
			this->Price_text->TabIndex = 58;
			this->Price_text->Visible = false;
			// 
			// Pr_L
			// 
			this->Pr_L->AutoSize = true;
			this->Pr_L->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->Pr_L->Location = System::Drawing::Point(205, 322);
			this->Pr_L->Name = L"Pr_L";
			this->Pr_L->Size = System::Drawing::Size(61, 27);
			this->Pr_L->TabIndex = 59;
			this->Pr_L->Text = L"Price";
			this->Pr_L->Visible = false;
			// 
			// Welcome_L
			// 
			this->Welcome_L->AutoSize = true;
			this->Welcome_L->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Welcome_L->ForeColor = System::Drawing::Color::SaddleBrown;
			this->Welcome_L->Location = System::Drawing::Point(579, 13);
			this->Welcome_L->Name = L"Welcome_L";
			this->Welcome_L->Size = System::Drawing::Size(0, 31);
			this->Welcome_L->TabIndex = 60;
			this->Welcome_L->Visible = false;
			// 
			// Entry_b
			// 
			this->Entry_b->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Entry_b->ForeColor = System::Drawing::Color::Purple;
			this->Entry_b->Location = System::Drawing::Point(31, 451);
			this->Entry_b->Name = L"Entry_b";
			this->Entry_b->Size = System::Drawing::Size(132, 82);
			this->Entry_b->TabIndex = 61;
			this->Entry_b->Text = L"Accept";
			this->Entry_b->UseVisualStyleBackColor = true;
			this->Entry_b->Visible = false;
			this->Entry_b->Click += gcnew System::EventHandler(this, &MyForm::Entry_b_Click);
			// 
			// Seans_cb
			// 
			this->Seans_cb->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 16.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Seans_cb->ForeColor = System::Drawing::Color::Purple;
			this->Seans_cb->FormattingEnabled = true;
			this->Seans_cb->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"10-13", L"14-17", L"18-22" });
			this->Seans_cb->Location = System::Drawing::Point(49, 127);
			this->Seans_cb->Name = L"Seans_cb";
			this->Seans_cb->Size = System::Drawing::Size(121, 37);
			this->Seans_cb->TabIndex = 62;
			this->Seans_cb->Visible = false;
			this->Seans_cb->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::Seans_cb_SelectedIndexChanged);
			// 
			// Hall_cb
			// 
			this->Hall_cb->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 16.2F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Hall_cb->ForeColor = System::Drawing::Color::Purple;
			this->Hall_cb->FormattingEnabled = true;
			this->Hall_cb->Items->AddRange(gcnew cli::array< System::Object^  >(10) {
				L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9",
					L"10"
			});
			this->Hall_cb->Location = System::Drawing::Point(55, 227);
			this->Hall_cb->Name = L"Hall_cb";
			this->Hall_cb->Size = System::Drawing::Size(121, 37);
			this->Hall_cb->TabIndex = 63;
			this->Hall_cb->Visible = false;
			this->Hall_cb->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::Hall_cb_SelectedIndexChanged);
			// 
			// Seans_l
			// 
			this->Seans_l->AutoSize = true;
			this->Seans_l->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Seans_l->ForeColor = System::Drawing::Color::Purple;
			this->Seans_l->Location = System::Drawing::Point(33, 97);
			this->Seans_l->Name = L"Seans_l";
			this->Seans_l->Size = System::Drawing::Size(147, 25);
			this->Seans_l->TabIndex = 64;
			this->Seans_l->Text = L"Choose seans:";
			this->Seans_l->Visible = false;
			// 
			// Hall_l
			// 
			this->Hall_l->AutoSize = true;
			this->Hall_l->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Hall_l->ForeColor = System::Drawing::Color::Purple;
			this->Hall_l->Location = System::Drawing::Point(28, 184);
			this->Hall_l->Name = L"Hall_l";
			this->Hall_l->Size = System::Drawing::Size(142, 25);
			this->Hall_l->TabIndex = 65;
			this->Hall_l->Text = L"Choose Hall:";
			this->Hall_l->Visible = false;
			// 
			// reqm_b
			// 
			this->reqm_b->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->reqm_b->ForeColor = System::Drawing::Color::MidnightBlue;
			this->reqm_b->Location = System::Drawing::Point(505, 65);
			this->reqm_b->Name = L"reqm_b";
			this->reqm_b->Size = System::Drawing::Size(130, 39);
			this->reqm_b->TabIndex = 70;
			this->reqm_b->Text = L"Request Money";
			this->reqm_b->UseVisualStyleBackColor = true;
			this->reqm_b->Visible = false;
			this->reqm_b->Click += gcnew System::EventHandler(this, &MyForm::reqm_b_Click);
			// 
			// ac_cb
			// 
			this->ac_cb->AutoSize = true;
			this->ac_cb->ForeColor = System::Drawing::SystemColors::InactiveBorder;
			this->ac_cb->Location = System::Drawing::Point(26, 362);
			this->ac_cb->Name = L"ac_cb";
			this->ac_cb->Size = System::Drawing::Size(356, 31);
			this->ac_cb->TabIndex = 71;
			this->ac_cb->Text = L"agreement (cinemacoordinator)";
			this->ac_cb->UseVisualStyleBackColor = true;
			this->ac_cb->Visible = false;
			// 
			// ad_cb
			// 
			this->ad_cb->AutoSize = true;
			this->ad_cb->ForeColor = System::Drawing::SystemColors::HighlightText;
			this->ad_cb->Location = System::Drawing::Point(27, 388);
			this->ad_cb->Name = L"ad_cb";
			this->ad_cb->Size = System::Drawing::Size(272, 31);
			this->ad_cb->TabIndex = 72;
			this->ad_cb->Text = L"agreement (distributor)";
			this->ad_cb->UseVisualStyleBackColor = true;
			this->ad_cb->Visible = false;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->RelD_L);
			this->panel1->Controls->Add(this->Author_L);
			this->panel1->Controls->Add(this->RelDate_l);
			this->panel1->Controls->Add(this->Auth_LL);
			this->panel1->Controls->Add(this->Prich_l);
			this->panel1->Controls->Add(this->MGch_l);
			this->panel1->Controls->Add(this->DurCh_l);
			this->panel1->Controls->Add(this->Agr_L);
			this->panel1->Controls->Add(this->TCh_l);
			this->panel1->Controls->Add(this->Dch_l);
			this->panel1->Controls->Add(this->AcceprM_B);
			this->panel1->Controls->Add(this->PriceCh_l);
			this->panel1->Controls->Add(this->GenreCh_L);
			this->panel1->Controls->Add(this->DurationCh_l);
			this->panel1->Controls->Add(this->DemCh_l);
			this->panel1->Controls->Add(this->Ageres_L);
			this->panel1->Controls->Add(this->TitleCh_L);
			this->panel1->Controls->Add(this->MovieCh_L);
			this->panel1->Location = System::Drawing::Point(950, 593);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(185, 406);
			this->panel1->TabIndex = 75;

			this->panel1->Visible = false;
			// 
			// Prich_l
			// 
			this->Prich_l->AutoSize = true;
			this->Prich_l->Location = System::Drawing::Point(115, 222);
			this->Prich_l->Name = L"Prich_l";
			this->Prich_l->Size = System::Drawing::Size(51, 16);
			this->Prich_l->TabIndex = 16;
			this->Prich_l->Text = L"label26";
			this->Prich_l->Visible = false;
			// 
			// MGch_l
			// 
			this->MGch_l->AutoSize = true;
			this->MGch_l->Location = System::Drawing::Point(115, 73);
			this->MGch_l->Name = L"MGch_l";
			this->MGch_l->Size = System::Drawing::Size(51, 16);
			this->MGch_l->TabIndex = 15;
			this->MGch_l->Text = L"label25";
			this->MGch_l->Visible = false;
			// 
			// DurCh_l
			// 
			this->DurCh_l->AutoSize = true;
			this->DurCh_l->Location = System::Drawing::Point(115, 183);
			this->DurCh_l->Name = L"DurCh_l";
			this->DurCh_l->Size = System::Drawing::Size(51, 16);
			this->DurCh_l->TabIndex = 12;
			this->DurCh_l->Text = L"label22";
			this->DurCh_l->Visible = false;
			// 
			// Agr_L
			// 
			this->Agr_L->AutoSize = true;
			this->Agr_L->Location = System::Drawing::Point(113, 113);
			this->Agr_L->Name = L"Agr_L";
			this->Agr_L->Size = System::Drawing::Size(51, 16);
			this->Agr_L->TabIndex = 11;
			this->Agr_L->Text = L"label19";
			this->Agr_L->Visible = false;
			// 
			// TCh_l
			// 
			this->TCh_l->AutoSize = true;
			this->TCh_l->Location = System::Drawing::Point(115, 34);
			this->TCh_l->Name = L"TCh_l";
			this->TCh_l->Size = System::Drawing::Size(51, 16);
			this->TCh_l->TabIndex = 10;
			this->TCh_l->Text = L"label19";
			this->TCh_l->Visible = false;
			// 
			// Dch_l
			// 
			this->Dch_l->AutoSize = true;
			this->Dch_l->Location = System::Drawing::Point(115, 143);
			this->Dch_l->Name = L"Dch_l";
			this->Dch_l->Size = System::Drawing::Size(51, 16);
			this->Dch_l->TabIndex = 10;
			this->Dch_l->Text = L"label19";
			this->Dch_l->Visible = false;
			// 
			// AcceprM_B
			// 
			this->AcceprM_B->Location = System::Drawing::Point(37, 347);
			this->AcceprM_B->Name = L"AcceprM_B";
			this->AcceprM_B->Size = System::Drawing::Size(110, 43);
			this->AcceprM_B->TabIndex = 9;
			this->AcceprM_B->Text = L"Accept";
			this->AcceprM_B->UseVisualStyleBackColor = true;
			this->AcceprM_B->Visible = false;
			// 
			// PriceCh_l
			// 
			this->PriceCh_l->AutoSize = true;
			this->PriceCh_l->Location = System::Drawing::Point(17, 222);
			this->PriceCh_l->Name = L"PriceCh_l";
			this->PriceCh_l->Size = System::Drawing::Size(41, 16);
			this->PriceCh_l->TabIndex = 6;
			this->PriceCh_l->Text = L"Price:";
			this->PriceCh_l->Visible = false;
			// 
			// GenreCh_L
			// 
			this->GenreCh_L->AutoSize = true;
			this->GenreCh_L->Location = System::Drawing::Point(14, 73);
			this->GenreCh_L->Name = L"GenreCh_L";
			this->GenreCh_L->Size = System::Drawing::Size(47, 16);
			this->GenreCh_L->TabIndex = 5;
			this->GenreCh_L->Text = L"Genre:";
			this->GenreCh_L->Visible = false;
			// 
			// DurationCh_l
			// 
			this->DurationCh_l->AutoSize = true;
			this->DurationCh_l->Location = System::Drawing::Point(14, 183);
			this->DurationCh_l->Name = L"DurationCh_l";
			this->DurationCh_l->Size = System::Drawing::Size(60, 16);
			this->DurationCh_l->TabIndex = 4;
			this->DurationCh_l->Text = L"Duration:";
			this->DurationCh_l->Visible = false;
			// 
			// DemCh_l
			// 
			this->DemCh_l->AutoSize = true;
			this->DemCh_l->Location = System::Drawing::Point(12, 143);
			this->DemCh_l->Name = L"DemCh_l";
			this->DemCh_l->Size = System::Drawing::Size(62, 16);
			this->DemCh_l->TabIndex = 3;
			this->DemCh_l->Text = L"Demand:";
			this->DemCh_l->Visible = false;
			// 
			// Ageres_L
			// 
			this->Ageres_L->AutoSize = true;
			this->Ageres_L->Location = System::Drawing::Point(12, 113);
			this->Ageres_L->Name = L"Ageres_L";
			this->Ageres_L->Size = System::Drawing::Size(95, 16);
			this->Ageres_L->TabIndex = 2;
			this->Ageres_L->Text = L"Age restriction:";
			this->Ageres_L->Visible = false;
			// 
			// TitleCh_L
			// 
			this->TitleCh_L->AutoSize = true;
			this->TitleCh_L->Location = System::Drawing::Point(17, 34);
			this->TitleCh_L->Name = L"TitleCh_L";
			this->TitleCh_L->Size = System::Drawing::Size(36, 16);
			this->TitleCh_L->TabIndex = 1;
			this->TitleCh_L->Text = L"Title:";
			this->TitleCh_L->Visible = false;
			// 
			// MovieCh_L
			// 
			this->MovieCh_L->AutoSize = true;
			this->MovieCh_L->Location = System::Drawing::Point(54, 9);
			this->MovieCh_L->Name = L"MovieCh_L";
			this->MovieCh_L->Size = System::Drawing::Size(44, 16);
			this->MovieCh_L->TabIndex = 0;
			this->MovieCh_L->Text = L"Movie";
			this->MovieCh_L->Visible = false;
			// 
			// Buy
			// 
			this->Buy->BackColor = System::Drawing::Color::IndianRed;
			this->Buy->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Buy->ForeColor = System::Drawing::Color::SeaShell;
			this->Buy->Location = System::Drawing::Point(221, 328);
			this->Buy->Name = L"Buy";
			this->Buy->Size = System::Drawing::Size(152, 91);
			this->Buy->TabIndex = 76;
			this->Buy->Text = L"Buy";
			this->Buy->UseVisualStyleBackColor = false;
			this->Buy->Visible = false;
			// 
			// ch_cb
			// 
			this->ch_cb->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ch_cb->FormattingEnabled = true;
			this->ch_cb->Location = System::Drawing::Point(55, 334);
			this->ch_cb->Name = L"ch_cb";
			this->ch_cb->Size = System::Drawing::Size(121, 45);
			this->ch_cb->TabIndex = 77;
			this->ch_cb->Visible = false;
			// 
			// cs_cb
			// 
			this->cs_cb->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->cs_cb->FormattingEnabled = true;
			this->cs_cb->Location = System::Drawing::Point(52, 212);
			this->cs_cb->Name = L"cs_cb";
			this->cs_cb->Size = System::Drawing::Size(163, 45);
			this->cs_cb->TabIndex = 78;
			this->cs_cb->Visible = false;
			// 
			// cpl_cb
			// 
			this->cpl_cb->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->cpl_cb->FormattingEnabled = true;
			this->cpl_cb->Items->AddRange(gcnew cli::array< System::Object^  >(30) {
				L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9",
					L"10", L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18", L"19", L"20", L"21", L"22", L"23", L"24", L"25", L"26", L"27",
					L"28", L"29", L"30"
			});
			this->cpl_cb->Location = System::Drawing::Point(33, 430);
			this->cpl_cb->Name = L"cpl_cb";
			this->cpl_cb->Size = System::Drawing::Size(120, 45);
			this->cpl_cb->TabIndex = 79;
			this->cpl_cb->Visible = false;
			// 
			// Ct_l
			// 
			this->Ct_l->AutoSize = true;
			this->Ct_l->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Ct_l->ForeColor = System::Drawing::Color::SaddleBrown;
			this->Ct_l->Location = System::Drawing::Point(31, 15);
			this->Ct_l->Name = L"Ct_l";
			this->Ct_l->Size = System::Drawing::Size(174, 37);
			this->Ct_l->TabIndex = 80;
			this->Ct_l->Text = L"Choose ticket";
			this->Ct_l->Visible = false;
			// 
			// Mt_l
			// 
			this->Mt_l->AutoSize = true;
			this->Mt_l->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Mt_l->ForeColor = System::Drawing::Color::SaddleBrown;
			this->Mt_l->Location = System::Drawing::Point(229, 14);
			this->Mt_l->Name = L"Mt_l";
			this->Mt_l->Size = System::Drawing::Size(96, 37);
			this->Mt_l->TabIndex = 81;
			this->Mt_l->Text = L"label28";
			this->Mt_l->Visible = false;
			// 
			// Hallc_l
			// 
			this->Hallc_l->AutoSize = true;
			this->Hallc_l->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Hallc_l->ForeColor = System::Drawing::Color::SaddleBrown;
			this->Hallc_l->Location = System::Drawing::Point(8, 276);
			this->Hallc_l->Name = L"Hallc_l";
			this->Hallc_l->Size = System::Drawing::Size(168, 37);
			this->Hallc_l->TabIndex = 83;
			this->Hallc_l->Text = L"Choose Hall:";
			this->Hallc_l->Visible = false;
			// 
			// C_seans
			// 
			this->C_seans->AutoSize = true;
			this->C_seans->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->C_seans->ForeColor = System::Drawing::Color::SaddleBrown;
			this->C_seans->Location = System::Drawing::Point(4, 171);
			this->C_seans->Name = L"C_seans";
			this->C_seans->Size = System::Drawing::Size(182, 37);
			this->C_seans->TabIndex = 84;
			this->C_seans->Text = L"Choose seans:";
			this->C_seans->Visible = false;
			// 
			// Cpl_l
			// 
			this->Cpl_l->AutoSize = true;
			this->Cpl_l->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Cpl_l->ForeColor = System::Drawing::Color::SaddleBrown;
			this->Cpl_l->Location = System::Drawing::Point(8, 391);
			this->Cpl_l->Name = L"Cpl_l";
			this->Cpl_l->Size = System::Drawing::Size(177, 37);
			this->Cpl_l->TabIndex = 85;
			this->Cpl_l->Text = L"Choose place:";
			this->Cpl_l->Visible = false;
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::Transparent;
			this->panel2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->panel2->Controls->Add(this->ad_cb);
			this->panel2->Controls->Add(this->ac_cb);
			this->panel2->Controls->Add(this->Pr_L);
			this->panel2->Controls->Add(this->Price_text);
			this->panel2->Controls->Add(this->Demand_text);
			this->panel2->Controls->Add(this->Endd_picker);
			this->panel2->Controls->Add(this->Startd_picker);
			this->panel2->Controls->Add(this->AgeR_text);
			this->panel2->Controls->Add(this->Genre_text);
			this->panel2->Controls->Add(this->Mt_text);
			this->panel2->Controls->Add(this->Dem_L);
			this->panel2->Controls->Add(this->EndD_L);
			this->panel2->Controls->Add(this->StD_L);
			this->panel2->Controls->Add(this->DM_L);
			this->panel2->Controls->Add(this->AgeR_L);
			this->panel2->Controls->Add(this->Genre_L);
			this->panel2->Controls->Add(this->Title_L);
			this->panel2->Controls->Add(this->Contr_L);
			this->panel2->Controls->Add(this->Durm_text);
			this->panel2->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->panel2->ForeColor = System::Drawing::SystemColors::ControlText;
			this->panel2->Location = System::Drawing::Point(663, 68);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(444, 500);
			this->panel2->TabIndex = 86;
			// 
			// T_text
			// 
			this->T_text->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->T_text->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->T_text->Location = System::Drawing::Point(13, 286);
			this->T_text->Name = L"T_text";
			this->T_text->Size = System::Drawing::Size(142, 27);
			this->T_text->TabIndex = 31;
			this->T_text->Visible = false;
			// 
			// Sal_text
			// 
			this->Sal_text->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->Sal_text->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Sal_text->Location = System::Drawing::Point(12, 162);
			this->Sal_text->Name = L"Sal_text";
			this->Sal_text->Size = System::Drawing::Size(145, 27);
			this->Sal_text->TabIndex = 29;
			this->Sal_text->Visible = false;
			// 
			// MB_text
			// 
			this->MB_text->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->MB_text->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)),
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->MB_text->ForeColor = System::Drawing::Color::Black;
			this->MB_text->Location = System::Drawing::Point(9, 77);
			this->MB_text->Name = L"MB_text";
			this->MB_text->Size = System::Drawing::Size(148, 27);
			this->MB_text->TabIndex = 30;
			this->MB_text->Visible = false;
			// 
			// BM_l
			// 
			this->BM_l->AutoSize = true;
			this->BM_l->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->BM_l->ForeColor = System::Drawing::Color::MidnightBlue;
			this->BM_l->Location = System::Drawing::Point(525, 134);
			this->BM_l->Name = L"BM_l";
			this->BM_l->Size = System::Drawing::Size(83, 31);
			this->BM_l->TabIndex = 87;
			this->BM_l->Text = L"label2";
			this->BM_l->Visible = false;
			// 
			// Contr_LM
			// 
			this->Contr_LM->AutoSize = true;
			this->Contr_LM->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Contr_LM->ForeColor = System::Drawing::Color::SaddleBrown;
			this->Contr_LM->Location = System::Drawing::Point(45, 21);
			this->Contr_LM->Name = L"Contr_LM";
			this->Contr_LM->Size = System::Drawing::Size(0, 23);
			this->Contr_LM->TabIndex = 88;
			this->Contr_LM->Visible = false;
			// 
			// PrefS_L
			// 
			this->PrefS_L->AutoSize = true;
			this->PrefS_L->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->PrefS_L->ForeColor = System::Drawing::Color::Purple;
			this->PrefS_L->Location = System::Drawing::Point(207, 133);
			this->PrefS_L->Name = L"PrefS_L";
			this->PrefS_L->Size = System::Drawing::Size(73, 25);
			this->PrefS_L->TabIndex = 89;
			this->PrefS_L->Text = L"label3";
			this->PrefS_L->Visible = false;
			// 
			// PrefHall_L
			// 
			this->PrefHall_L->AutoSize = true;
			this->PrefHall_L->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->PrefHall_L->ForeColor = System::Drawing::Color::Purple;
			this->PrefHall_L->Location = System::Drawing::Point(207, 227);
			this->PrefHall_L->Name = L"PrefHall_L";
			this->PrefHall_L->Size = System::Drawing::Size(73, 25);
			this->PrefHall_L->TabIndex = 90;
			this->PrefHall_L->Text = L"label4";
			this->PrefHall_L->Visible = false;
			// 
			// contracts_cb
			// 
			this->contracts_cb->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->contracts_cb->FormattingEnabled = true;
			this->contracts_cb->Location = System::Drawing::Point(327, 438);
			this->contracts_cb->Name = L"contracts_cb";
			this->contracts_cb->Size = System::Drawing::Size(479, 39);
			this->contracts_cb->TabIndex = 91;
			this->contracts_cb->Visible = false;
			// 
			// applyContract_b
			// 
			this->applyContract_b->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->applyContract_b->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei UI", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->applyContract_b->ForeColor = System::Drawing::Color::MidnightBlue;
			this->applyContract_b->Location = System::Drawing::Point(467, 480);
			this->applyContract_b->Name = L"applyContract_b";
			this->applyContract_b->Size = System::Drawing::Size(195, 87);
			this->applyContract_b->TabIndex = 92;
			this->applyContract_b->Text = L"Apply selected contract";
			this->applyContract_b->UseVisualStyleBackColor = false;
			this->applyContract_b->Visible = false;
			// 
			// DateT
			// 
			this->DateT->CalendarTitleBackColor = System::Drawing::Color::IndianRed;
			this->DateT->CalendarTrailingForeColor = System::Drawing::Color::Brown;
			this->DateT->CustomFormat = L"yyyy-MM-dd";
			this->DateT->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->DateT->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->DateT->Location = System::Drawing::Point(31, 114);
			this->DateT->Name = L"DateT";
			this->DateT->Size = System::Drawing::Size(252, 44);
			this->DateT->TabIndex = 75;
			this->DateT->Visible = false;
			// 
			// DateLab
			// 
			this->DateLab->AutoSize = true;
			this->DateLab->Font = (gcnew System::Drawing::Font(L"Papyrus", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->DateLab->ForeColor = System::Drawing::Color::SaddleBrown;
			this->DateLab->Location = System::Drawing::Point(11, 60);
			this->DateLab->Name = L"DateLab";
			this->DateLab->Size = System::Drawing::Size(82, 37);
			this->DateLab->TabIndex = 93;
			this->DateLab->Text = L"Date:";
			this->DateLab->Visible = false;
			// 
			// EmployMinMax
			// 
			this->EmployMinMax->AutoSize = true;
			this->EmployMinMax->BackColor = System::Drawing::Color::Transparent;
			this->EmployMinMax->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EmployMinMax->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->EmployMinMax->Location = System::Drawing::Point(9, 196);
			this->EmployMinMax->Name = L"EmployMinMax";
			this->EmployMinMax->Size = System::Drawing::Size(72, 23);
			this->EmployMinMax->TabIndex = 94;
			this->EmployMinMax->Text = L"label2";
			this->EmployMinMax->Visible = false;
			// 
			// Auth_LL
			// 
			this->Auth_LL->AutoSize = true;
			this->Auth_LL->Location = System::Drawing::Point(17, 265);
			this->Auth_LL->Name = L"Auth_LL";
			this->Auth_LL->Size = System::Drawing::Size(48, 16);
			this->Auth_LL->TabIndex = 17;
			this->Auth_LL->Text = L"Author:";


			this->Auth_LL->Visible = false;
			// 
			// RelDate_l
			// 
			this->RelDate_l->AutoSize = true;
			this->RelDate_l->Location = System::Drawing::Point(3, 312);
			this->RelDate_l->Name = L"RelDate_l";
			this->RelDate_l->Size = System::Drawing::Size(92, 16);
			this->RelDate_l->TabIndex = 18;
			this->RelDate_l->Text = L"Realese date:";


			this->RelDate_l->Visible = false;
			// 
			// Author_L
			// 
			this->Author_L->AutoSize = true;
			this->Author_L->Location = System::Drawing::Point(115, 265);
			this->Author_L->Name = L"Author_L";
			this->Author_L->Size = System::Drawing::Size(44, 16);
			this->Author_L->TabIndex = 19;
			this->Author_L->Text = L"label4";

			this->Author_L->Visible = false;
			// 
			// RelD_L
			// 
			this->RelD_L->AutoSize = true;
			this->RelD_L->Location = System::Drawing::Point(115, 312);
			this->RelD_L->Name = L"RelD_L";
			this->RelD_L->Size = System::Drawing::Size(44, 16);
			this->RelD_L->TabIndex = 20;
			this->RelD_L->Text = L"label5";

			this->RelD_L->Visible = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(1187, 581);
			this->Controls->Add(this->AccC_B);
			this->Controls->Add(this->S_label);
			this->Controls->Add(this->EmployMinMax);
			this->Controls->Add(this->DateLab);
			this->Controls->Add(this->DateT);
			this->Controls->Add(this->applyContract_b);
			this->Controls->Add(this->contracts_cb);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->PrefHall_L);
			this->Controls->Add(this->PrefS_L);
			this->Controls->Add(this->Contr_LM);
			this->Controls->Add(this->BM_l);
			this->Controls->Add(this->moviesFlow);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->Cpl_l);
			this->Controls->Add(this->C_seans);
			this->Controls->Add(this->Hallc_l);
			this->Controls->Add(this->Mt_l);
			this->Controls->Add(this->Ct_l);
			this->Controls->Add(this->cpl_cb);
			this->Controls->Add(this->cs_cb);
			this->Controls->Add(this->ch_cb);
			this->Controls->Add(this->Buy);
			this->Controls->Add(this->reqm_b);
			this->Controls->Add(this->Hall_l);
			this->Controls->Add(this->Seans_l);
			this->Controls->Add(this->Hall_cb);
			this->Controls->Add(this->Seans_cb);
			this->Controls->Add(this->Employee_B);
			this->Controls->Add(this->Entry_b);
			this->Controls->Add(this->Welcome_L);
			this->Controls->Add(this->backb);
			this->Controls->Add(this->TP_L);
			this->Controls->Add(this->M_label);
			this->Controls->Add(this->Emp_CB);
			this->Controls->Add(this->T_text);
			this->Controls->Add(this->MB_text);
			this->Controls->Add(this->Sal_text);
			this->Controls->Add(this->Acc_B);
			this->Controls->Add(this->Bud_label);
			this->Controls->Add(this->Employ_L);
			this->Controls->Add(this->Sal_L);
			this->Controls->Add(this->MB_L);
			this->Controls->Add(this->T_label);
			this->Controls->Add(this->Position_L);
			this->Controls->Add(this->Code_L);
			this->Controls->Add(this->PIB_L);
			this->Controls->Add(this->b_Emply);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->Elabel1);
			this->Controls->Add(this->View_B);
			this->Controls->Add(this->label1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

     


	private:
		bool ValidateViewerControls();
		bool ResolveSlotIndex(int& slotIdx);
		bool ParseHallAndSeat(int& hallNo, int& seatNo);
		void ReadTimeAndDate(std::string& block, std::string& dateStr);
		bool CheckAndLockSeat(int slotIdx, int hallNo, int seatNo, const std::string& block);
		double ComputeTicketPriceOrDefault();
		void ApplyBudgetAfterSale(double price);
		void RemoveSelectedSeatFromCombo();
		void ShowTicketSuccess(const std::string& dateStr, const std::string& block, int hallNo, int seatNo, double price);

	private:
		value struct SalaryParse { bool ok; double value; };

		SalaryParse ParseSalaryOnce();
		void ApproveMovieBudget(System::Text::StringBuilder^ report, bool& anyApproved);
		void ApproveTicketPrice(System::Text::StringBuilder^ report, bool& anyApproved);
		void ApproveSalary(const SalaryParse& sp, System::Text::StringBuilder^ report, bool& anyApproved);
		void RefreshBudgetLabel();
		void ShowApprovalSummary(bool anyApproved, System::Text::StringBuilder^ report);

#pragma endregion
	private: void ResetContractUI();


	private: void HideAllControls() {
		// Приховати всі елементи
		label1->Visible = false;
		Employee_B->Visible = false;
		View_B->Visible = false;
		Elabel1->Visible = false;
		textBox1->Visible = false;
		textBox2->Visible = false;
		Position_L->Visible = false;
		Code_L->Visible = false;
		PIB_L->Visible = false;
		comboBox1->Visible = false;
		b_Emply->Visible = false;
		Bud_label->Visible = false;
		S_label->Visible = false;
		M_label->Visible = false;
		Employ_L->Visible = false;
		Emp_CB->Visible = false;
		T_label->Visible = false;
		Acc_B->Visible = false;
		MB_L->Visible = false;
		MB_text->Visible = false;
		Sal_L->Visible = false;
		Sal_text->Visible = false;
		TP_L->Visible = false;

		Contr_L->Visible = false;
		Mt_text->Visible = false;
		AgeR_text->Visible = false;
		Genre_text->Visible = false;
		Durm_text->Visible = false;
		Startd_picker->Visible = false;
		Endd_picker->Visible = false;
		Demand_text->Visible = false;
		Price_text->Visible = false;
		AccC_B->Visible = false;
		Title_L->Visible = false;
		Genre_L->Visible = false;
		AgeR_L->Visible = false;
		DM_L->Visible = false;
		StD_L->Visible = false;
		Dem_L->Visible = false;
		Pr_L->Visible = false;
		EndD_L->Visible = false;
		T_text->Visible = false;

		Welcome_L->Visible = false;
		Entry_b->Visible = false;
		Seans_cb->Visible = false;
		Hall_cb->Visible = false;
		Seans_l->Visible = false;
		Hall_l->Visible = false;
		reqm_b->Visible = false;
		ac_cb->Visible = false;
		ad_cb->Visible = false;
		Prich_l->Visible = false;
		MGch_l->Visible = false;
		DurCh_l->Visible = false;
		Agr_L->Visible = false;
		TCh_l->Visible = false;
		Dch_l->Visible = false;
		AcceprM_B->Visible = false;
		PriceCh_l->Visible = false;
		GenreCh_L->Visible = false;
		DurationCh_l->Visible = false;
		DemCh_l->Visible = false;
		Ageres_L->Visible = false;
		TitleCh_L->Visible = false;
		MovieCh_L->Visible = false;
		Buy->Visible = false;
		ch_cb->Visible = false;
		cs_cb->Visible = false;
		cpl_cb->Visible = false;
		Ct_l->Visible = false;
		Mt_l->Visible = false;
		Hallc_l->Visible = false;
		C_seans->Visible = false;
		Cpl_l->Visible = false;
		T_text->Visible = false;
		Sal_text->Visible = false;

		Endd_picker->Visible = false;
		Startd_picker->Visible = false;
		Price_text->Visible = false;
		Demand_text->Visible = false;
		Genre_text->Visible = false;
		Dem_L->Visible = false;
		EndD_L->Visible = false;
		StD_L->Visible = false;
		DM_L->Visible = false;
		AgeR_L->Visible = false;
		Genre_L->Visible = false;
		Title_L->Visible = false;
		AccC_B->Visible = false;
		moviesFlow->Visible = false;
		reqm_b->Visible = false;
		BM_l->Visible = false;

		PrefHall_L->Visible = false;
		PrefS_L->Visible = false;
		Contr_LM->Visible = false;

		applyContract_b->Visible = false;
		contracts_cb->Visible = false;


		DateT->Visible = false;
		DateLab->Visible = false;

		EmployMinMax->Visible = false;

		RelD_L->Visible = false;
		Author_L->Visible = false;
		RelDate_l->Visible = false;

		Auth_LL->Visible = false;

	}



	private:
		bool ValidateContractUI();
		ContractDraft BuildContractDraft();
		void EnsureDistribTimerCleared();
		void StartDistributorRequest(const ContractDraft& d);
		bool SaveCoordinatorContract(const ContractDraft& d, Contract*& created);
		void AfterContractSaved(Contract* created, const ContractDraft& d);
		void UpdateBudgetHints(const ContractDraft& d);
		void UpdateSummaryLabel();

	private: void ShowPage(int pageNumber) {
		HideAllControls();

		switch (pageNumber) {
		case 1: // Головна сторінка
			label1->Visible = true;
			Employee_B->Visible = true;
			View_B->Visible = true;
			break;

		case 2: // Сторінка реєстрації працівника
			// NEW: очистити інпут-и щоразу
			if (textBox1) textBox1->Clear();
			if (textBox2) textBox2->Clear();
			if (comboBox1) {
				comboBox1->SelectedIndex = -1;
				comboBox1->SelectedItem = nullptr;
				comboBox1->Text = "";
			}

			Elabel1->Visible = true;
			textBox1->Visible = true;
			textBox2->Visible = true;
			Position_L->Visible = true;
			Code_L->Visible = true;
			PIB_L->Visible = true;
			comboBox1->Visible = true;
			b_Emply->Visible = true;
			break;

		case 3: // Сторінка бухгалтера
			Bud_label->Visible = true;
			S_label->Visible = true;
			M_label->Visible = true;
			Employ_L->Visible = true;
			Emp_CB->Visible = true;
			T_label->Visible = true;
			Acc_B->Visible = true;
			MB_L->Visible = true;
			MB_text->Visible = true;
			Sal_L->Visible = true;
			Sal_text->Visible = true;
			TP_L->Visible = true;
			T_text->Visible = true;
			contracts_cb->Visible = true;
			applyContract_b->Visible = true;
			EmployMinMax->Visible = true;
			UpdateBudgetLabelsFromPercents();
			break;

		case 4: // Сторінка контракту
			Contr_L->Visible = true;
			Mt_text->Visible = true;
			AgeR_text->Visible = true;
			Genre_text->Visible = true;
			Durm_text->Visible = true;
			Startd_picker->Visible = true;
			Endd_picker->Visible = true;
			Demand_text->Visible = true;
			Price_text->Visible = true;
			AccC_B->Visible = true;
			Title_L->Visible = true;
			Genre_L->Visible = true;
			AgeR_L->Visible = true;
			DM_L->Visible = true;
			StD_L->Visible = true;
			Dem_L->Visible = true;
			Pr_L->Visible = true;
			EndD_L->Visible = true;
			ac_cb->Visible = true;
			ad_cb->Visible = true;
			break;

		case 5: // Сторінка менеджера
			Entry_b->Visible = true;
			Seans_cb->Visible = true;
			Hall_cb->Visible = true;
			S_label->Visible = true;
			Seans_l->Visible = true;
			Hall_l->Visible = true;
			PrefHall_L->Visible = true;
			PrefS_L->Visible = true;
			UpdatePreferredSeansAndHall();
			Contr_LM->Visible = true;
			Seans_cb->SelectedIndex = 2; // наприклад одразу Evening
			Seans_cb_SelectedIndexChanged(nullptr, nullptr);
			contracts_cb->Visible = true;
			applyContract_b->Visible = true;
			if (Seans_cb)   Seans_cb->Enabled = false;
			if (Hall_cb)    Hall_cb->Enabled = false;
			if (PrefHall_L) PrefHall_L->Enabled = false; // (не обов’язково, це Label)
			if (PrefS_L)    PrefS_L->Enabled = false; 
			break;


		case 6: //сторінка кінокоординатора
			BM_l->Visible = true;
			reqm_b->Visible = true;
			break;


		case 7: //покупка білету
			Buy->Visible = true;
			ch_cb->Visible = true;
			cs_cb->Visible = true;
			cpl_cb->Visible = true;
			Ct_l->Visible = true;
			Mt_l->Visible = true;
			Hallc_l->Visible = true;
			C_seans->Visible = true;
			Cpl_l->Visible = true;

			DateLab->Visible = false;
			DateT->Visible = true;
			// before hiding: leaving old page
			if (_currentPage == 7) {
				ResetViewerPageUI();
			}

			UpdateDateTBounds();
			UpdateViewerTicketPriceLabel();

			PopulateViewerChoicesFromManager();     // <— важливо
			RebuildSeatComboForCurrentSelection();  // <— після того, як є вибір

			break;

		case 8: //сторінка фільма
			moviesFlow->Visible = true;
			break;
			Prich_l->Visible = true;
			MGch_l->Visible = true;
			DurCh_l->Visible = true;
			Agr_L->Visible = true;
			TCh_l->Visible = true;
			Dch_l->Visible = true;
			AcceprM_B->Visible = true;
			PriceCh_l->Visible = true;
			GenreCh_L->Visible = true;
			DurationCh_l->Visible = true;
			DemCh_l->Visible = true;
			Ageres_L->Visible = true;
			TitleCh_L->Visible = true;
			MovieCh_L->Visible = true;
			RelD_L->Visible = true;
			Author_L->Visible = true;
			RelDate_l->Visible = true;

			Auth_LL->Visible = true;

			break;

		}

		_currentPage = pageNumber;
	}


		   void PrefillContractFromMovie(const Movie& m);

		   // додамо декларацію ще одного хендлера, щоб «вибирати» фільм кліком по картці
     private: System::Void OnMovieCardClick(System::Object^ sender, System::EventArgs^ e);
	   int _activeEmployeeCode = -1;
	   void UpdateEmployMinMaxLabel();
     private: System::Void Emp_CB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);


	private: System::Void View_B_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Employee_B_Click(System::Object^ sender, System::EventArgs^ e) {
		ShowPage(2);

	}

     
	 private: System::Void AcceptM_B_Click(System::Object^ sender, System::EventArgs^ e) {
		 ShowPage(7);
	 }

     private: System::Void Seans_cb_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
     private: System::Void Hall_cb_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);

	private: void UpdateEmployeeComboBox();


	private: System::Void b_Emply_Click(System::Object^ sender, System::EventArgs^ e);

	private: void PopulateViewerChoicesFromManager();

    private: System::Void backb_Click(System::Object^ sender, System::EventArgs^ e) {
		ShowPage(1);

		cs_cb->SelectedIndex = -1;
		ch_cb->SelectedIndex = -1;
		cpl_cb->SelectedIndex = -1;

		cs_cb->Text = "";
		ch_cb->Text = "";
		cpl_cb->Text = "";
    }
	private: System::Void reqm_b_Click(System::Object^ sender, System::EventArgs^ e);

	private:
		System::Void OnViewerMovieBlocked(System::Object^ sender, System::EventArgs^ e);

	private: System::Void AccC_B_Click(System::Object^ sender, System::EventArgs^ e);

	private: System::Windows::Forms::Panel^ CreateViewerMovieCard(const Movie& m);
    private: void PopulateViewerCatalog();
    private: System::Void OnAcceptViewerMovie(System::Object^ sender, System::EventArgs^ e);

	private: System::Void StartOrEndChanged_UpdateDateT(System::Object^ sender, System::EventArgs^ e);
	private: void UpdateDateTBounds();

	private: System::Void Entry_b_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Acc_B_Click(System::Object^ sender, System::EventArgs^ e);

	private: void RebuildSeatComboForCurrentSelection();
    private: System::Void cs_cb_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
    private: System::Void Buy_Click(System::Object^ sender, System::EventArgs^ e);

	private: void PopulateContractsCombo(int selectCode);

	private:
		int  lockedHallNumber = -1;
		bool hallSelectionLocked = false;

	private:
		// Track current page and force rebuild of viewer UI on leave
		int _currentPage = 1;

		void ResetViewerPageUI() {
			if (!cs_cb || !ch_cb || !cpl_cb) return;
			cs_cb->Items->Clear();  cs_cb->Text = "";  cs_cb->SelectedIndex = -1;
			ch_cb->Items->Clear();  ch_cb->Text = "";  ch_cb->SelectedIndex = -1;
			cpl_cb->Items->Clear(); cpl_cb->Text = ""; cpl_cb->SelectedIndex = -1;
		}

	private:
		void UpdateBudgetLabelsFromPercents();                       // обгортка 80/20
		void UpdateBudgetLabelsFromPercents(double movieShare, double salaryShare);

};
}