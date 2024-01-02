#pragma once

#include <iostream>
#include <mutex>
#include <atomic>

class Syncronization {
	public:
		static Syncronization& Syncronizer();
		Syncronization(const Syncronization&) = delete;
		Syncronization(Syncronization&&) = delete;
		Syncronization& operator=(const Syncronization&) = delete;
		Syncronization& operator=(Syncronization&&) = delete;
		~Syncronization() = default;

	// public members
	public:
		std::atomic<bool> s_NetworkThreadRunning;

	private:
		Syncronization() = default;
};