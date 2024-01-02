#pragma once

#include "logging.h"
#include "windivert.h"

class Networking {
	public: 
		Networking();
		Networking(const Networking&) = delete;
		Networking(Networking&&) = delete;
		Networking& operator=(const Networking&) = delete;
		Networking& operator=(Networking&&) = delete;
		~Networking();
		
		// tells the driver what to do with matching packets
		enum class DivertType {
			DIVERT_TYPE_LATENCY,    // add latency to matching packets
			DIVERT_TYPE_DROP,       // drop matching packets
			DIVERT_TYPE_PERCENTDROP // drop a percentage of matching packets
		};

		// utility enum for Application
		enum class TrafficDirection {
			DIRECTION_INBOUND,
			DIRECTION_OUTBOUND
		};

		// must be called each time the calling application
		// requests to start a networking operation
		bool CheckFunctionality();
		bool CheckFunctionality(const char* filter);

		// starts the networking operation with the given filter
		// and opmode.
		void Start(
			DivertType divType,
			const char* filter,
			uint16_t latency = 0,
			uint8_t percentDrop = 0
		);

		std::string CompileFilter(
			const char* filter,
			TrafficDirection direction
		);
	
	// private members
	private:
		Logger& _logger;
		HANDLE _ifHandle;
		WINDIVERT_ADDRESS _addr[10];
		uint8_t* _packets;
		uint32_t _packetLen;
		uint32_t _addrLen;
		
	// private methods
	private:
		bool Setup(const char* filter);
		void LatencyMode(uint16_t latency);
		void DropMode();
		void PercentDropMode(uint8_t percentDrop);
};