#pragma once

// Depricated
// Using GameEvents::Instance().OnFrameUpdate instead

class UpdateRelay
{
private:
	// private constructor -- can't create from outside
	UpdateRelay() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	UpdateRelay(const UpdateRelay&) = delete;
	UpdateRelay& operator=(const UpdateRelay&) = delete;

public:

	// the ONLY way to get the instance
	static UpdateRelay& Instance() {
		static UpdateRelay instance; // created once, lives forever
		return instance;
	}

	void DispatchUpdate(const float& dT)
	{

	}

};