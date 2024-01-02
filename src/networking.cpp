#include "networking.h"

Networking::Networking() : _logger(Logger::GetInstance()), _ifHandle(nullptr), _addr(), _packets(nullptr), _packetLen(0) {
	this->_packets = new uint8_t[10 * WINDIVERT_MTU_MAX];
	this->_addrLen = sizeof(this->_addr);
}

Networking::~Networking() {
	delete[] this->_packets;
	this->_logger.LogToFile("Networking::~Networking(): Resources freed.\n");
}

bool Networking::CheckFunctionality() {
	// check if WinDivert loads and is functional
	this->_ifHandle = WinDivertOpen("true", WINDIVERT_LAYER_NETWORK, 0, 0);
	if (this->_ifHandle == INVALID_HANDLE_VALUE) {
		this->_logger.LogToFile("Networking::CheckFunctionality(): Failed to open WinDivert with error {}.\n", GetLastError());
		return false;
	}
	WinDivertClose(this->_ifHandle);
	this->_logger.LogToFile("Networking::CheckFunctionality(): WinDivert loaded and functional.\n");

	return true;
}

bool Networking::CheckFunctionality(const char* filter) {
	// check if WinDivert loads and is functional
	this->_ifHandle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
	if (this->_ifHandle == INVALID_HANDLE_VALUE) {
		this->_logger.LogToFile("Networking::CheckFunctionality(const char*): Failed to open WinDivert with error {}. \"{}\" is likely an invalid filter.\n", GetLastError(), filter);
		return false;
	}
	WinDivertClose(this->_ifHandle);
	this->_logger.LogToFile("Networking::CheckFunctionality(const char*): WinDivert loaded and functional.\n");

	return true;
}

void Networking::Start(DivertType divType, const char* filter, uint16_t latency, uint8_t percentDrop) {
	this->_logger.LogToFile("Networking::Start(): Setting up WinDivert.\n");
	if (this->Setup(filter)) {
		this->_logger.LogToFile("Networking::Start(): Set up WinDivert with filter \"{}.\"\n", filter);
		switch (divType) {
			case Networking::DivertType::DIVERT_TYPE_DROP: {
				this->_logger.LogToFile("Networking::Start(): Starting drop mode.\n");
				this->DropMode();
				break;
			}
			case Networking::DivertType::DIVERT_TYPE_LATENCY: {
				this->_logger.LogToFile("Networking::Start(): Starting latency mode.\n");
				this->LatencyMode(latency);
				break;
			}
			case Networking::DivertType::DIVERT_TYPE_PERCENTDROP: {
				this->_logger.LogToFile("Networking::Start(): Starting percent drop mode.\n");
				this->PercentDropMode(percentDrop);
				break;
			}
		}

		if (this->_ifHandle) {
			WinDivertClose(this->_ifHandle);
		}
		
		this->_logger.LogToFile("Networking::Start(): Network thread exiting.\n");
		
		return;
	}
	else {
		this->_logger.LogToFile("Networking::Start(): Failed to setup WinDivert with error {}.\n", GetLastError());
		return;
	}
}

std::string Networking::CompileFilter(const char* filter, TrafficDirection direction) {
	std::string _filter;

	// compile the filter
	if (direction == TrafficDirection::DIRECTION_INBOUND) {
		_filter = "inbound and ";
	}
	else {
		_filter = "outbound and ";
	}
	_filter += filter;

	return _filter;
}

bool Networking::Setup(const char* filter) {
	this->_ifHandle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
	if (this->_ifHandle == INVALID_HANDLE_VALUE) {
		this->_logger.LogToFile("Networking::Setup(): Failed to open WinDivert with error {}.\n", GetLastError());
		return false;
	}

	if (!WinDivertSetParam(this->_ifHandle, WINDIVERT_PARAM_QUEUE_LENGTH, WINDIVERT_PARAM_QUEUE_LENGTH_MAX)) {
		this->_logger.LogToFile("Networking::Setup(): Failed to set WinDivert queue length with error {}.\n", GetLastError());
		return false;
	}
	this->_logger.LogToFile("Networking::Setup(): WinDivert queue length set to maximum.\n");
	
	if (!WinDivertSetParam(this->_ifHandle, WINDIVERT_PARAM_QUEUE_TIME, WINDIVERT_PARAM_QUEUE_TIME_MAX)) {
		this->_logger.LogToFile("Networking::Setup(): Failed to set WinDivert queue time with error {}.\n", GetLastError());
		return false;
	}
	this->_logger.LogToFile("Networking::Setup(): WinDivert queue time set to maximum.\n");

	if (!WinDivertSetParam(this->_ifHandle, WINDIVERT_PARAM_QUEUE_SIZE, WINDIVERT_PARAM_QUEUE_SIZE_MAX)) {
		this->_logger.LogToFile("Networking::Setup(): Failed to set WinDivert queue size with error {}.\n", GetLastError());
		return false;
	}
	this->_logger.LogToFile("Networking::Setup(): WinDivert queue size set to maximum.\n");

	return true;
}

void Networking::LatencyMode(uint16_t latency) {
	while (Syncronization::Syncronizer().s_NetworkThreadRunning) {
		if (!WinDivertRecvEx(this->_ifHandle, this->_packets, 10 * WINDIVERT_MTU_MAX, &this->_packetLen, NULL, this->_addr, &this->_addrLen, NULL)) {
			this->_logger.LogToFile("Networking::LatencyMode(): Failed to receive packet with error {}.\n", GetLastError());
			continue;
		}
		this->_logger.LogToFile("Networking::LatencyMode(): Received packet, delaying for {}ms.\n", latency);
		std::this_thread::sleep_for(std::chrono::milliseconds(latency));		
		
		if (!WinDivertRecvEx(this->_ifHandle, this->_packets, 10 * WINDIVERT_MTU_MAX, &this->_packetLen, NULL, this->_addr, &this->_addrLen, NULL)) {
			this->_logger.LogToFile("Networking::LatencyMode(): Failed to reinject packet with error {}.\n", GetLastError());
			continue;
		}
		this->_logger.LogToFile("Networking::LatencyMode(): Re-injected packet after {}ms.\n", latency);
	}
	
	this->_logger.LogToFile("Networking::LatencyMode(): Exiting latency mode.\n");
}

void Networking::DropMode() {
	while (Syncronization::Syncronizer().s_NetworkThreadRunning) {
		this->_logger.LogToFile("Networking::DropMode(): Waiting for packet.\n");
		if (!WinDivertRecvEx(this->_ifHandle, this->_packets, 10 * WINDIVERT_MTU_MAX, &this->_packetLen, NULL, this->_addr, &this->_addrLen, NULL)) {
			this->_logger.LogToFile("Networking::DropMode(): Failed to receive packet with error {}.\n", GetLastError());
			continue;
		}
		this->_logger.LogToFile("Networking::DropMode(): Packet received and dropped.\n");
	}

	this->_logger.LogToFile("Networking::DropMode(): Exiting drop mode.\n");
}

void Networking::PercentDropMode(uint8_t percentDrop) {
	while (Syncronization::Syncronizer().s_NetworkThreadRunning) {
		this->_logger.LogToFile("Networking::PercentDropMode(): Waiting for packet.\n");
		if (!WinDivertRecvEx(this->_ifHandle, this->_packets, 10 * WINDIVERT_MTU_MAX, &this->_packetLen, NULL, this->_addr, &this->_addrLen, NULL)) {
			this->_logger.LogToFile("Networking::PercentDropMode(): Failed to receive packet with error {}.\n", GetLastError());
			continue;
		}
		else {
			if (rand() % 100 < percentDrop) {
				this->_logger.LogToFile("Networking::PercentDropMode(): Packet received and dropped at {}% chance.\n", percentDrop);
				continue;
			}
			else {
				this->_logger.LogToFile("Networking::PercentDropMode(): Packet received and reinjected.\n");
			}
		}
		if (!WinDivertSendEx(this->_ifHandle, this->_packets, 10 * WINDIVERT_MTU_MAX, &this->_packetLen, NULL, this->_addr, this->_addrLen, NULL)) {
			this->_logger.LogToFile("Networking::PercentDropMode(): Failed to reinject packet with error {}.\n", GetLastError());
			continue;
		}
	}

	this->_logger.LogToFile("Networking::PercentDropMode(): Exiting percent drop mode.\n");
}