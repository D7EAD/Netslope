#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <windows.h>
#include <regex>
#include "fmt/format.h"
#include "sync.h"

class Logger {
	public:
		static Logger& GetInstance();
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger& operator=(Logger&&) = delete;
		~Logger() = default;
		
	// public member methods
	public: 
		// function to get current time and date return as string
		std::string GetTimeAndDate();
		template<typename... Args> void LogToFile(const std::string& format, Args&&... args) {
			std::lock_guard<std::mutex> lock(this->m_Mutex);
			this->m_LogFile << fmt::format(this->GetTimeAndDate() + " " + format, std::forward<Args>(args)...);
		}
		void ClearLog();
		std::string GetLogFilePath();

	// private members
	private:
		std::mutex m_Mutex;
		std::filesystem::path m_LogPath;
		std::ofstream m_LogFile;

	// private methods
	private:
		Logger();
};