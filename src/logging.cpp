#include "logging.h"

Logger::Logger() : m_LogPath(std::filesystem::current_path() / "log.txt") {
	if (!std::filesystem::exists(m_LogPath)) {
		this->m_LogFile = std::ofstream(m_LogPath, std::ios::out | std::ios::app);
		this->LogToFile("Logger::Logger(): Log file created and opened at {}\n", m_LogPath.string());
	}
	else {
		// open the file in append mode
		this->m_LogFile = std::ofstream(this->m_LogPath, std::ios::out | std::ios::app);
		this->LogToFile("Logger::Logger(): Log file opened at {}\n", m_LogPath.string());
	}
}

Logger& Logger::GetInstance() {
	static Logger instance;
	return instance;
}

std::string Logger::GetTimeAndDate() {
	char tmBuff[30];

	// get current time and date
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	ctime_s(tmBuff, sizeof(tmBuff), &time);
	std::string timeAndDate(tmBuff);
	
	// remove the newline character from the string
	timeAndDate.erase(std::remove(timeAndDate.begin(), timeAndDate.end(), '\n'), timeAndDate.end());
	
	return fmt::format("[{}]", timeAndDate);
}

void Logger::ClearLog() {
	this->m_LogFile.close();
	this->m_LogFile.open(this->m_LogPath, std::ios::out | std::ios::trunc);
	this->LogToFile("Logger::ClearLog(): Log file cleared at {}\n", m_LogPath.string());
}

std::string Logger::GetLogFilePath() {
	return this->m_LogPath.string();
}