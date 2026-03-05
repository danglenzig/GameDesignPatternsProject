#pragma once

// example code, exclude from project

class MinimalSingleton
{
private:
	// private constructor -- can't create from outside
	MinimalSingleton() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	MinimalSingleton(const MinimalSingleton&) = delete;
	MinimalSingleton& operator=(const MinimalSingleton&) = delete;

public:

	// the ONLY way to get the instance
	static MinimalSingleton& Instance() {
		static MinimalSingleton instance; // created once, lives forever
		return instance;
	}
};