#pragma once

#include "overrides.h"
#include "networking.h"

#define _SEND_ALERT(message) fl_beep(); fl_alert(message);

class Application {
	public:
		static Application& GetInstance();
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;
		~Application();

	// public member methods
	public:
		bool InitializeWindow();
		int eventloop();
		
	// private members
	private:
		// window components
		std::unique_ptr<Fl_Window>  m_MainWindow  = nullptr;
		std::unique_ptr<DM_MenuBar> m_MenuBar     = nullptr;
		std::unique_ptr<DM_Button>  m_StartButton = nullptr,
			m_StopButton = nullptr;
		std::unique_ptr<DM_Check_Button> m_DropModeCheckbox = nullptr,
			m_LatencyModeCheckbox = nullptr, m_PercentDropModeCheckbox = nullptr,
			m_UsePrecompiledFiltersCheckbox = nullptr;
		std::unique_ptr<DM_Int_Input> m_LatencyInput = nullptr,
			m_PercentDropInput = nullptr;
		std::unique_ptr<DM_Choice> m_TrafficDirectionChoice = nullptr,
			m_PrecompiledFiltersChoice = nullptr;
		std::unique_ptr<DM_Input> m_CustomFilterInput = nullptr;

		// pre-compiled filters
		std::vector<std::string> m_PrecompiledFilters;

		// logger
		Logger& m_Logger;

		// network thread related
		Networking                             m_Networking;
		std::thread                         m_NetworkThread;
		uint16_t                              m_Latency = 0;
		uint8_t                           m_PercentDrop = 0;
		Networking::TrafficDirection     m_TrafficDirection;
		std::string        m_CompiledFilter, m_CustomFilter;
		
	// private methods
	private:
		Application();

		// component-related
		bool DrawMenuBar();                                               // draws menu bar
		bool DrawChecks();                                                // draws checkboxes
		bool DrawButtons();                                               // draws buttons
		bool DrawInputs();                                                // draws inputs
		bool DrawChoices();                                               // draws dropdowns
		Networking::DivertType GetDivertType();                           // returns the type of divert selected from checkboxes
		Networking::TrafficDirection GetTrafficDirection();               // returns the traffic direction selected from dropdown

		// utility
		template <class... _Filters>
		void AddPrecompiledFilters(_Filters&&... filters) {
			(m_PrecompiledFilters.push_back(std::forward<_Filters>(filters)), ...);
		}
		void Start_ModifyControls(); // deactivates/activates necessary controls on start
		void Stop_ModifyControls();
		
		// callbacks
		static void OnOpenLog(Fl_Widget* w, void* p);
		static void OnClearLog(Fl_Widget* w, void* p);
		static void OnExit(Fl_Widget* w, void* p);
		static void OnStart(Fl_Widget* w, void* p);
		static void OnStop(Fl_Widget* w, void* p);
		static void OnCheck_DropMode(Fl_Widget* w, void* p);
		static void OnCheck_LatencyMode(Fl_Widget* w, void* p);
		static void OnCheck_PercentDropMode(Fl_Widget* w, void* p);
		static void OnInput_PercentDrop(Fl_Widget* w, void* p);
		static void OnInput_Latency(Fl_Widget* w, void* p);
		static void OnInput_CustomFilter(Fl_Widget* w, void* p);
		static void OnChoice_TrafficDirection(Fl_Widget* w, void* p);
		static void OnChoice_PrecompiledFilters(Fl_Widget* w, void* p);
		static void OnCheck_UsePrecompiledFilters(Fl_Widget* w, void* p);
};