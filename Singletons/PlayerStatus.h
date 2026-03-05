#pragma once
#include "raylib.h"
#include <iostream>


class PlayerStatus
{
private:
	// private constructor -- can't create from outside
	PlayerStatus() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	PlayerStatus(const PlayerStatus&) = delete;
	PlayerStatus& operator=(const PlayerStatus&) = delete;

	Vector2 playerPosition = { 0,0 };
	std::string playerState = "NONE";


public:

	// the ONLY way to get the instance
	static PlayerStatus& Instance() {
		static PlayerStatus instance; // created once, lives forever
		return instance;
	}

	void SetPlayerPosition(const Vector2& pos) {
		playerPosition = pos;
	}
	Vector2 GetPlayerPosition() const {
		return playerPosition;
	}

	void SetPlayerState(const std::string& stateName) {
		playerState = stateName;
	}
	std::string GetPlayerState() const {
		return playerState;
	}


};