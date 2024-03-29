#pragma once

//#define MinDataRequestInterval 1000

namespace WindowsForm {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms::DataVisualization::Charting;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^ comboBox1;
	protected:
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Button^ button2;


	private: System::Windows::Forms::Timer^ timer1;
	private: System::IO::Ports::SerialPort^ serialPort1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::TextBox^ textBox1;

	private: System::Windows::Forms::Timer^ timer2;
	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->serialPort1 = (gcnew System::IO::Ports::SerialPort(this->components));
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(24, 24);
			this->comboBox1->Margin = System::Windows::Forms::Padding(6);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(218, 32);
			this->comboBox1->TabIndex = 0;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(1291, 20);
			this->button1->Margin = System::Windows::Forms::Padding(6);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(198, 42);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->button3);
			this->groupBox1->Controls->Add(this->chart1);
			this->groupBox1->Controls->Add(this->button2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(24, 105);
			this->groupBox1->Margin = System::Windows::Forms::Padding(6);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(6);
			this->groupBox1->Size = System::Drawing::Size(1484, 510);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"groupBox1";
			// 
			// textBox1
			// 
			this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->textBox1->Location = System::Drawing::Point(929, 56);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 29);
			this->textBox1->TabIndex = 5;
			// 
			// button3
			// 
			this->button3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button3->Location = System::Drawing::Point(1267, 52);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(198, 39);
			this->button3->TabIndex = 4;
			this->button3->Text = L"button3";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// chart1
			// 
			this->chart1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(18, 100);
			this->chart1->Name = L"chart1";
			this->chart1->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Bright;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Point;
			series1->IsVisibleInLegend = false;
			series1->IsXValueIndexed = true;
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(1447, 401);
			this->chart1->TabIndex = 3;
			this->chart1->Text = L"chart1";
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button2->Location = System::Drawing::Point(1038, 52);
			this->button2->Margin = System::Windows::Forms::Padding(6);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(220, 39);
			this->button2->TabIndex = 2;
			this->button2->Text = L"button2";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(797, 59);
			this->label1->Margin = System::Windows::Forms::Padding(6, 0, 6, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(64, 25);
			this->label1->TabIndex = 0;
			this->label1->Text = L"label1";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// serialPort1
			// 
			this->serialPort1->DataReceived += gcnew System::IO::Ports::SerialDataReceivedEventHandler(this, &Form1::serialPort1_DataReceived);
			// 
			// timer2
			// 
			this->timer2->Tick += gcnew System::EventHandler(this, &Form1::timer2_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(11, 24);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1522, 637);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->comboBox1);
			this->Margin = System::Windows::Forms::Padding(6);
			this->Name = L"Form1";
			this->Text = L"Form1 - MOGI template";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->SizeChanged += gcnew System::EventHandler(this, &Form1::Form1_SizeChanged);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

		//Random^ RandInt = gcnew Random();

		//lista a megjelenített időpontoknak
		List<String^>^ idopontok = gcnew List<String^>();
		//lista a megjelenített hőmérsékletértékeknek
		List<double>^ Temp = gcnew List<double>();

		//új adat hozzáadása USB-ről
		void UpdateData()
		{
			//sor kiolvasása USB-ről
			String^ tmp = serialPort1->ReadLine();
			//a sor elejéről levágjuk a '\0' karaktereket
			while (tmp[0] == '\0')
				tmp = tmp->Remove(0,1);
			//string to double konverzió után hozzáasjuk az adatsorhoz
			Temp->Add(Convert::ToDouble(tmp));
			//aktuális időpont hozzáadása az adatsorhoz
			idopontok->Add(DateTime::Now.ToString("HH:mm:ss"));
		}

		//program indulása
		private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e)
		{
			//ablakokn lévő elemek szövegei, paraméterei
			Text = "uMogi";

			button1->Text = "Connect";
			button2->Text = "Start Measuring";
			button3->Text = "Stop Measuring";

			label1->Text = "Interval [s]:";

			textBox1->Text = "1";

			groupBox1->Text = "uMogi";
			groupBox1->Enabled = false;

			timer1->Interval = Convert::ToDouble(textBox1->Text) * 1000;

			//soros portok megjelenítése a ComboBoxban
			array<String^>^ portok = System::IO::Ports::SerialPort::GetPortNames();
			comboBox1->Items->AddRange(portok);

			//grafikon megjelenése
			chart1->ChartAreas[0]->AxisX->MajorGrid->LineColor = Color::LightGray;
			chart1->ChartAreas[0]->AxisY->MajorGrid->LineColor = Color::LightGray;
			chart1->ChartAreas[0]->AxisY->Title = "Temperature [°C]";
			chart1->ChartAreas[0]->AxisX->Maximum = chart1->Width / 50;
			chart1->Series["Series1"]->Color = Color::Green;
			chart1->ChartAreas[0]->AxisX->Interval = 1;

			//timer2 indítása: az listákból periodikusan kiolvassuk az adatokat és a grafikonhoz rendeljük
			timer2->Interval = 100;
			timer2->Start();
		}

		private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) //Connect GOMB
		{
			try
			{
				//soros kommunikáció megnyitása a ComboBoxban kiválasztott porton
				serialPort1->PortName = comboBox1->Text;
				serialPort1->BaudRate = 115200;
				serialPort1->NewLine = "\r\n";
				serialPort1->Open();
				groupBox1->Enabled = true;
			}
			//ha nem sikerült, hibaüzenet mutatása
			catch (Exception^ ex) { MessageBox::Show("Connection Error"); }
		}

		private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) //Start Measuring GOMB
		{
			//timer1 intervallumának beállítása a textBoxban megadott érték alapján
			try { timer1->Interval = Convert::ToDouble(textBox1->Text) * 1000; }
			catch (Exception^ ex) { MessageBox::Show("Incorrect Interval"); }
			//timer1 intervalluma ne legyen kisebb mint 1 s
			if (timer1->Interval < 1000)
			{
				timer1->Interval = 1000;
				textBox1->Text = "1";
			}
			
			//adatsor törlése
			Temp->Clear();
			idopontok->Clear();
			chart1->Series["Series1"]->Points->Clear();

			if (serialPort1->IsOpen)
			{
				button2->Text = "Reset Measurement";	//ha folyamatban van a mérés, a gomb újabb megnyomásával törölhetjük az adatokat
				serialPort1->WriteLine("t");			//aktuális hőmérésklet kérése USB-n
				timer1->Start();						//timer1 indítása, minden ticknél új hőmérséklet kérése
			}
		}
		
		//timer1 letelt
		private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e)
		{
			if (serialPort1->IsOpen)
				serialPort1->WriteLine("t\r\n");	//aktuális hőmérésklet kérése USB-n

		}

		//soros porton adat érkezett
		private: System::Void serialPort1_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e)
		{
			//ha az adatok száma túllépte a maxmumot, töröljük
			if (idopontok->Count >= chart1->ChartAreas[0]->AxisX->Maximum)
			{
				Temp->Clear();
				idopontok->Clear();
			}
			UpdateData();	//új adat hozzáadása USB-ről
		}

		private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) //Stop Measuring GOMB
		{
			timer1->Stop();	//timer1 leállítása, így nem jérünk több adatot USB-n
			button2->Text = "Start Measuring";
		}

		//ha változik az ablak mérete, frissítjük a grafikonon megjeleníthető adatok számát
		private: System::Void Form1_SizeChanged(System::Object^ sender, System::EventArgs^ e)
		{
			chart1->ChartAreas[0]->AxisX->Maximum = (int)chart1->Width / 50;
		}

		//timer2 letelt
		private: System::Void timer2_Tick(System::Object^ sender, System::EventArgs^ e)
		{
			int meret = Temp->Count;
			//ha érkezett adat, végigmegyünk a kiolvasott adatsor elemein
			if (meret > 0);
			{
				for (int i = 0; i < meret; i++)
				{
					//a legelső adatot a grafikonhoz rendeljük
					double temp=Temp[0];
					String^ time = idopontok[0];
					chart1->Series["Series1"]->Points->AddXY(time, temp);
					//töröljük a legelső adatot.
					Temp->RemoveAt(0);
					idopontok->RemoveAt(0);
				}
			}

			//ha  megetelt a grafikon, kitöröljük a legelső elemét
			if (chart1->Series["Series1"]->Points->Count == chart1->ChartAreas[0]->AxisX->Maximum)
			{
				chart1->Series["Series1"]->Points->RemoveAt(0);
			}
			chart1->ResetAutoValues();
		}
};
}
