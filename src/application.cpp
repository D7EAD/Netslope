#include "application.h"

Application::Application() : m_Logger(Logger::GetInstance()), m_TrafficDirection(Networking::TrafficDirection::DIRECTION_INBOUND) {
	Fl::scheme("gtk+");
	Fl::background(45, 45, 48);
	Fl::set_color(FL_FOREGROUND_COLOR, fl_rgb_color(200, 200, 200));
	
	// load pre-compiled filters
	this->AddPrecompiledFilters(
		"all",
		"loopback",
		"tcp",
		"udp",
		"icmp",
		"icmpv6",
		"ip",
		"ipv6",
		"tcp and (tcp.DstPort == 80)",
		"tcp and (tcp.SrcPort == 80)",
		"tcp and (tcp.DstPort == 443)",
		"tcp and (tcp.SrcPort == 443)",
		"udp and (udp.DstPort == 53)",
		"udp and (udp.SrcPort == 53)",
		"tcp and (tcp.DstPort == 53)",
		"tcp and (tcp.SrcPort == 53)",
		"udp and (udp.DstPort == 67)",
		"udp and (udp.SrcPort == 67)",
		"udp and (udp.DstPort == 68)",
		"udp and (udp.SrcPort == 68)",
		"tcp and (tcp.DstPort == 25)",
		"tcp and (tcp.SrcPort == 25)",
		"tcp and (tcp.DstPort == 110)",
		"tcp and (tcp.SrcPort == 110)"
	);

	this->m_Logger.LogToFile("Application::Application(): Application initializing.\n");
}

bool Application::DrawMenuBar() {
	this->m_Logger.LogToFile("Application::DrawMenuBar(): Drawing menu bar and members.\n");

	this->m_MenuBar = std::make_unique<DM_MenuBar>(0, 0, WINDOW_WIDTH, 25);
	if (!this->m_MenuBar) {
		this->m_Logger.LogToFile("Application::DrawMenuBar(): Failed to allocate memory for menu bar.\n");
		return false;
	}
	this->m_MenuBar->add("File/Open Log", FL_CTRL + 'o', Application::OnOpenLog);
	this->m_MenuBar->add("File/Clear Log", FL_CTRL + 'x', Application::OnClearLog);
	this->m_MenuBar->add("Exit", FL_CTRL + 'q', Application::OnExit);
	this->m_MainWindow->add(this->m_MenuBar.get());

	this->m_Logger.LogToFile("Application::DrawMenuBar(): Menu bar and members drawn.\n");
	return true;
}

// add check callbacks
bool Application::DrawChecks() {
	this->m_Logger.LogToFile("Application::DrawChecks(): Drawing checkboxes.\n");
	
	this->m_DropModeCheckbox = std::make_unique<DM_Check_Button>(270, 225, 18, 18, "Drop Mode");
	if (!this->m_DropModeCheckbox) {
		this->m_Logger.LogToFile("Application::DrawChecks(): Failed to allocate memory for drop mode checkbox.\n");
		return false;
	}
	this->m_DropModeCheckbox->value(1);
	this->m_DropModeCheckbox->callback(Application::OnCheck_DropMode);
	this->m_MainWindow->add(this->m_DropModeCheckbox.get());

	this->m_LatencyModeCheckbox = std::make_unique<DM_Check_Button>(157, 225, 18, 18, "Latency Mode");
	if (!this->m_LatencyModeCheckbox) {
		this->m_Logger.LogToFile("Application::DrawChecks(): Failed to allocate memory for latency mode checkbox.\n");
		return false;
	}
	this->m_LatencyModeCheckbox->callback(Application::OnCheck_LatencyMode);
	this->m_MainWindow->add(this->m_LatencyModeCheckbox.get());
	
	this->m_PercentDropModeCheckbox = std::make_unique<DM_Check_Button>(10, 225, 18, 18, "Percent Drop Mode");
	if (!this->m_PercentDropModeCheckbox) {
		this->m_Logger.LogToFile("Application::DrawChecks(): Failed to allocate memory for percent drop mode checkbox.\n");
		return false;
	}
	this->m_PercentDropModeCheckbox->callback(Application::OnCheck_PercentDropMode);
	this->m_MainWindow->add(this->m_PercentDropModeCheckbox.get());

	this->m_UsePrecompiledFiltersCheckbox = std::make_unique<DM_Check_Button>(10, 170, 18, 18, "Use Precompiled Filters");
	if (!this->m_UsePrecompiledFiltersCheckbox) {
		this->m_Logger.LogToFile("Application::DrawChecks(): Failed to allocate memory for use precompiled filters checkbox.\n");
		return false;
	}
	this->m_UsePrecompiledFiltersCheckbox->value(1);
	this->m_UsePrecompiledFiltersCheckbox->callback(Application::OnCheck_UsePrecompiledFilters);
	this->m_MainWindow->add(this->m_UsePrecompiledFiltersCheckbox.get());

	this->m_Logger.LogToFile("Application::DrawChecks(): Checkboxes drawn.\n");
	return true;
}

bool Application::DrawButtons() {
	this->m_Logger.LogToFile("Application::DrawButtons(): Drawing buttons.\n");
	
	this->m_StartButton = std::make_unique<DM_Button>(WINDOW_WIDTH - 80, WINDOW_HEIGHT - 30, 75, 25, "Start");
	if (!this->m_StartButton) {
		this->m_Logger.LogToFile("Application::DrawButtons(): Failed to allocate memory for start button.\n");
		return false;
	}
	this->m_StartButton->callback(Application::OnStart);
	this->m_MainWindow->add(this->m_StartButton.get());

	this->m_StopButton = std::make_unique<DM_Button>(WINDOW_WIDTH - 160, WINDOW_HEIGHT - 30, 75, 25, "Stop");
	if (!this->m_StopButton) {
		this->m_Logger.LogToFile("Application::DrawButtons(): Failed to allocate memory for stop button.\n");
		return false;
	}
	this->m_StopButton->deactivate();
	this->m_StopButton->callback(Application::OnStop);
	this->m_MainWindow->add(this->m_StopButton.get());
	
	this->m_Logger.LogToFile("Application::DrawButtons(): Buttons drawn.\n");
	return true;
}

bool Application::DrawInputs() {
	this->m_Logger.LogToFile("Application::DrawInputs(): Drawing inputs.\n");

	this->m_PercentDropInput = std::make_unique<DM_Int_Input>(147, 140, 30, 25, "Packets to Drop (%):");
	if (!this->m_PercentDropInput) {
		this->m_Logger.LogToFile("Application::DrawInputs(): Failed to allocate memory for percent drop input.\n");
		return false;
	}
	this->m_PercentDropInput->value("0");
	this->m_PercentDropInput->deactivate();
	this->m_PercentDropInput->callback(Application::OnInput_PercentDrop);
	this->m_MainWindow->add(this->m_PercentDropInput.get());

	this->m_LatencyInput = std::make_unique<DM_Int_Input>(280, 140, 30, 25, "Latency (ms):");
	if (!this->m_LatencyInput) {
		this->m_Logger.LogToFile("Application::DrawInputs(): Failed to allocate memory for latency input.\n");
		return false;
	}
	this->m_LatencyInput->value("0");
	this->m_LatencyInput->deactivate();
	this->m_LatencyInput->callback(Application::OnInput_Latency);
	this->m_MainWindow->add(this->m_LatencyInput.get());
	
	this->m_CustomFilterInput = std::make_unique<DM_Input>(10, 110, 300, 25, "User-Defined Filter");
	if (!this->m_CustomFilterInput) {
		this->m_Logger.LogToFile("Application::DrawInputs(): Failed to allocate memory for custom filter input.\n");
		return false;
	}
	this->m_CustomFilterInput->deactivate();
	this->m_CustomFilterInput->callback(Application::OnInput_CustomFilter);
	this->m_MainWindow->add(this->m_CustomFilterInput.get());

	this->m_Logger.LogToFile("Application::DrawInputs(): Inputs drawn.\n");
	return true;
}

bool Application::DrawChoices() {
	this->m_Logger.LogToFile("Application::DrawChoice(): Drawing dropdown lists.\n");

	this->m_TrafficDirectionChoice = std::make_unique<DM_Choice>(464, 180, 130, 25, "Traffic Direction:");
	if (!this->m_TrafficDirectionChoice) {
		this->m_Logger.LogToFile("Application::DrawChoice(): Failed to allocate memory for traffic direction choice.\n");
		return false;
	}
	this->m_TrafficDirectionChoice->add("Inbound");
	this->m_TrafficDirectionChoice->add("Outbound");
	this->m_TrafficDirectionChoice->value(0);
	this->m_TrafficDirectionChoice->callback(Application::OnChoice_TrafficDirection);
	this->m_MainWindow->add(this->m_TrafficDirectionChoice.get());
	
	this->m_PrecompiledFiltersChoice = std::make_unique<DM_Choice>(10, 60, 300, 25, "Precompiled Filters");
	if (!this->m_PrecompiledFiltersChoice) {
		this->m_Logger.LogToFile("Application::DrawChoice(): Failed to allocate memory for precompiled filters choice.\n");
		return false;
	}
	
	// add precompiled filters to dropdown list
	for (std::string& filter : this->m_PrecompiledFilters) {
		this->m_PrecompiledFiltersChoice->add(filter.c_str());
	}
	this->m_PrecompiledFiltersChoice->value(0);
	this->m_PrecompiledFiltersChoice->align(FL_ALIGN_TOP_LEFT);
	this->m_PrecompiledFiltersChoice->callback(Application::OnChoice_PrecompiledFilters);

	this->m_Logger.LogToFile("Application::DrawChoice(): Dropdown lists drawn.\n");
	return true;
}

Networking::DivertType Application::GetDivertType() {
	if (this->m_DropModeCheckbox->value()) {
		return Networking::DivertType::DIVERT_TYPE_DROP;
	}
	else if (this->m_LatencyModeCheckbox->value()) {
		return Networking::DivertType::DIVERT_TYPE_LATENCY;
	}
	else if (this->m_PercentDropModeCheckbox->value()) {
		return Networking::DivertType::DIVERT_TYPE_PERCENTDROP;
	}
}

void Application::OnOpenLog(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	std::string args = app.m_Logger.GetLogFilePath();
	ShellExecuteA(NULL, "open", "notepad.exe", args.c_str(), NULL, SW_SHOWDEFAULT);

	app.m_Logger.LogToFile("Application::OnOpenLog(): Opening log file.\n");
}

void Application::OnClearLog(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();
	app.m_Logger.ClearLog();
	_SEND_ALERT("Log cleared.");
}

void Application::OnExit(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();
	
	// ask the user if they want to exit
	fl_beep();
	if (fl_choice("Are you sure you want to exit?", "No", "Yes", NULL)) {
		app.m_Logger.LogToFile("Application::OnExit(): Exiting application.\n");
		app.m_MainWindow->hide();
	}
}

void Application::OnStart(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	// start the network thread
	if (Syncronization::Syncronizer().s_NetworkThreadRunning) {
		_SEND_ALERT("Network thread already running, unable to start.");
	}
	else {
		if (!app.m_Networking.CheckFunctionality()) { // check general functionality
			_SEND_ALERT("Unable to start network diversion, check logs.");
		}
		else {
			app.m_Logger.LogToFile("Application::OnStart(): Disabling GUI components while running.\n");
			app.Start_ModifyControls();
			app.m_Logger.LogToFile("Application::OnStart(): GUI components disabled.\n");

			// using pre-compiled filter
			if (app.m_UsePrecompiledFiltersCheckbox->value()) {
				app.m_Logger.LogToFile("Application::OnStart(): Using precompiled filters.\n");

				if (app.m_PrecompiledFilters[app.m_PrecompiledFiltersChoice->value()] == "all") {
					app.m_Logger.LogToFile("Application::OnStart(): Filter \"all\" selected, no filter will be used.\n");

					if (app.GetTrafficDirection() == Networking::TrafficDirection::DIRECTION_INBOUND) {
						app.m_CompiledFilter = "inbound";
					}
					else {
						app.m_CompiledFilter = "outbound";
					}
				}
				else {
					app.m_Logger.LogToFile("Application::OnStart(): Using precompiled filter: \"{}\"\n", app.m_PrecompiledFilters[app.m_PrecompiledFiltersChoice->value()]);

					app.m_CompiledFilter = app.m_Networking.CompileFilter(
						app.m_PrecompiledFilters[app.m_PrecompiledFiltersChoice->value()].c_str(),
						app.GetTrafficDirection()
					);
				}
				
				Syncronization::Syncronizer().s_NetworkThreadRunning = true;
				app.m_NetworkThread = std::thread(
					&Networking::Start,
					&app.m_Networking,
					app.GetDivertType(),            // divert type
					app.m_CompiledFilter.c_str(),   // compiled filter
					app.m_Latency,                  // latency(?)
					app.m_PercentDrop               // percentDrop(?)
				);
				app.m_NetworkThread.detach();
				app.m_Logger.LogToFile("Application::OnStart(): Starting network thread.\n");
				app.m_MainWindow->label("Netslope (Running)");
			}
			else { // custom filter
				app.m_CustomFilter = app.m_CustomFilterInput->value();

				// check filter validity
				if (!app.m_Networking.CheckFunctionality(app.m_CustomFilter.c_str())) {
					_SEND_ALERT("Invalid filter.");
					app.m_Logger.LogToFile("Application::OnStart(): Filter \"{}\" is invalid.\n", app.m_CustomFilter);
					app.Stop_ModifyControls(); // re-enable controls
					return;
				}
				else {
					app.m_Logger.LogToFile("Application::OnStart(): Custom filter: \"{}\" is valid.\n", app.m_CustomFilter);
				}

				app.m_Logger.LogToFile("Application::OnStart(): Using custom filter: \"{}\"\n", app.m_CustomFilter);

				Syncronization::Syncronizer().s_NetworkThreadRunning = true;
				app.m_NetworkThread = std::thread(
					&Networking::Start,
					&app.m_Networking,
					app.GetDivertType(),        // divert type
					app.m_CustomFilter.c_str(), // compiled filter
					app.m_Latency,              // latency(?)
					app.m_PercentDrop           // percentDrop(?)
				);
				app.m_NetworkThread.detach();
				app.m_Logger.LogToFile("Application::OnStart(): Starting network thread.\n");
				app.m_MainWindow->label("Netslope (Running)");
			}
		}
	}
}

void Application::OnStop(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	if (!Syncronization::Syncronizer().s_NetworkThreadRunning) {
		_SEND_ALERT("Network diversion not running, unable to stop.");
	}
	else {
		app.m_Logger.LogToFile("Application::OnStop(): Stopping network thread.\n");
		Syncronization::Syncronizer().s_NetworkThreadRunning = false;
		app.m_MainWindow->label("Netslope");

		app.m_Logger.LogToFile("Application::OnStop(): Enabling GUI components on stop.\n");
		app.Stop_ModifyControls();
		app.m_Logger.LogToFile("Application::OnStop(): GUI components enabled.\n");
	}
}

Networking::TrafficDirection Application::GetTrafficDirection() {
	if (this->m_TrafficDirectionChoice->value() == 0) {
		return Networking::TrafficDirection::DIRECTION_INBOUND;
	}
	else if (this->m_TrafficDirectionChoice->value() == 1) {
		return Networking::TrafficDirection::DIRECTION_OUTBOUND;
	}
}

void Application::OnCheck_DropMode(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();
	
	// disable other mode inputs
	app.m_PercentDropInput->deactivate();
	app.m_LatencyInput->deactivate();

	app.m_LatencyModeCheckbox->value(0);
	app.m_PercentDropModeCheckbox->value(0);
	app.m_DropModeCheckbox->value(1);
	app.m_Logger.LogToFile("Application::OnCheck_DropMode(): Drop mode selected.\n");
}

void Application::OnCheck_LatencyMode(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	// enable latency input
	app.m_LatencyInput->activate();

	// disable percent drop input
	app.m_PercentDropInput->deactivate();

	app.m_DropModeCheckbox->value(0);
	app.m_PercentDropModeCheckbox->value(0);
	app.m_LatencyModeCheckbox->value(1);
	app.m_Logger.LogToFile("Application::OnCheck_LatencyMode(): Latency mode selected.\n");
}

void Application::OnCheck_PercentDropMode(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	// enable percent drop input
	app.m_PercentDropInput->activate();

	// disable latency input
	app.m_LatencyInput->deactivate();

	app.m_DropModeCheckbox->value(0);
	app.m_LatencyModeCheckbox->value(0);
	app.m_PercentDropModeCheckbox->value(1);
	app.m_Logger.LogToFile("Application::OnCheck_PercentDropMode(): Percent drop mode selected.\n");
}

void Application::OnInput_PercentDrop(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	std::string input = app.m_PercentDropInput->value();

	if (!std::regex_match(input, std::regex("[0-9]+"))) {
		return;
	}

	int percentDrop = std::stoi(input);
	if (percentDrop < 0) {
		percentDrop = 0;
		app.m_PercentDropInput->value("0");
	}
	else if (percentDrop > 100) {
		percentDrop = 100;
		app.m_PercentDropInput->value("100");
	}

	app.m_PercentDrop = percentDrop;
}

void Application::OnInput_Latency(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	std::string input = app.m_LatencyInput->value();

	if (!std::regex_match(input, std::regex("[0-9]+"))) {
		return;
	}

	int latency = std::stoi(input);
	app.m_Latency = latency;
}

void Application::OnChoice_TrafficDirection(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	switch (app.m_TrafficDirectionChoice->value()){
		case 0: {
			app.m_Logger.LogToFile("Application::OnChoice_TrafficDirection(): Inbound traffic selected.\n");
			app.m_TrafficDirection = Networking::TrafficDirection::DIRECTION_INBOUND;
			break;
		}
		case 1: {
			app.m_Logger.LogToFile("Application::OnChoice_TrafficDirection(): Outbound traffic selected.\n");
			app.m_TrafficDirection = Networking::TrafficDirection::DIRECTION_OUTBOUND;
			break;
		}
	}
}

void Application::Start_ModifyControls() {
	Application& app = Application::GetInstance();

	// disable start button
	app.m_StartButton->deactivate();

	// enable stop button
	app.m_StopButton->activate();

	// disable checkboxes
	app.m_DropModeCheckbox->deactivate();
	app.m_LatencyModeCheckbox->deactivate();
	app.m_PercentDropModeCheckbox->deactivate();
	app.m_UsePrecompiledFiltersCheckbox->deactivate();

	// disable inputs
	app.m_PercentDropInput->deactivate();
	app.m_LatencyInput->deactivate();
	app.m_CustomFilterInput->deactivate();

	// disable choices
	app.m_TrafficDirectionChoice->deactivate();
	app.m_PrecompiledFiltersChoice->deactivate();
}

void Application::Stop_ModifyControls() {
	Application& app = Application::GetInstance();

	// enable start button
	app.m_StartButton->activate();

	// disable stop button
	app.m_StopButton->deactivate();

	// enable checkboxes
	app.m_DropModeCheckbox->activate();
	app.m_LatencyModeCheckbox->activate();
	app.m_PercentDropModeCheckbox->activate();
	app.m_UsePrecompiledFiltersCheckbox->activate();

	// enable inputs
	if (app.m_DropModeCheckbox->value() == 1) {
		app.m_PercentDropInput->deactivate();
		app.m_LatencyInput->deactivate();
	}
	else if (app.m_LatencyModeCheckbox->value() == 1) {
		app.m_LatencyInput->activate();
	}
	else if (app.m_PercentDropModeCheckbox->value() == 1) {
		app.m_PercentDropInput->activate();
	}

	// enable choices
	if (app.m_UsePrecompiledFiltersCheckbox->value() == 1) {
		app.m_PrecompiledFiltersChoice->activate();
		app.m_TrafficDirectionChoice->activate();
	}
	else {
		app.m_CustomFilterInput->activate();
	}
}

void Application::OnCheck_UsePrecompiledFilters(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	if (app.m_UsePrecompiledFiltersCheckbox->value()) {
		app.m_TrafficDirectionChoice->activate();
		app.m_PrecompiledFiltersChoice->activate();
		app.m_CustomFilterInput->deactivate();
		app.m_Logger.LogToFile("Application::OnCheck_UsePrecompiledFilters(): Using precompiled filters.\n");
	}
	else {
		app.m_TrafficDirectionChoice->deactivate();
		app.m_PrecompiledFiltersChoice->deactivate();
		app.m_CustomFilterInput->activate();
		app.m_Logger.LogToFile("Application::OnCheck_UsePrecompiledFilters(): Not using precompiled filters.\n");
	}
}

void Application::OnInput_CustomFilter(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();
	
	std::string input = app.m_CustomFilterInput->value();
	app.m_CustomFilter = input;

	app.m_Logger.LogToFile("Application::OnInput_CustomFilter(): Custom filter set to: " + input + "\n");
}

void Application::OnChoice_PrecompiledFilters(Fl_Widget* w, void* p) {
	Application& app = Application::GetInstance();

	app.m_CompiledFilter = app.m_PrecompiledFilters[app.m_PrecompiledFiltersChoice->value()];
}

Application& Application::GetInstance() {
	static Application instance;
	return instance;
}

Application::~Application() {
	this->m_Logger.LogToFile("Application::~Application(): Application closing.\n");
}

bool Application::InitializeWindow() {
	this->m_MainWindow = std::make_unique<Fl_Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "Netslope");
	if (!this->m_MainWindow) {
		this->m_Logger.LogToFile("Application::InitializeWindow(): Failed to initialize main window.\n");
		return false;
	}

	// draw menu bar
	if (!this->DrawMenuBar()) { return false; }

	// draw buttons
	if (!this->DrawButtons()) { return false; }

	// draw checkboxes
	if (!this->DrawChecks()) { return false; }
	
	// draw inputs
	if (!this->DrawInputs()) { return false; }

	// draw dropdown lists
	if (!this->DrawChoices()) { return false; }

	this->m_Logger.LogToFile("Application::InitializeWindow(): Initialized main window and components.\n");

	return true;
}

int Application::eventloop() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	this->m_MainWindow->callback(Application::OnExit, this);
	this->m_MainWindow->end();
	this->m_MainWindow->show();

	this->m_Logger.LogToFile("Application::eventloop(): Starting event loop.\n");

	return Fl::run();
}