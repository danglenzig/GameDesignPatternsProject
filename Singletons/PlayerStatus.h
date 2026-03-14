#pragma once
#include "raylib.h"
#include <iostream>


class PlayerStatus
{
private:
	PlayerStatus() {}
	PlayerStatus(const PlayerStatus&) = delete;
	PlayerStatus& operator=(const PlayerStatus&) = delete;

	Vector2 playerPosition = { 0,0 };
	std::string playerState = "NONE";

	const float hitRadius = 40.0f;

	int playerHealth = -1;
	size_t bugsSlapped = 0;
	size_t seconds_played = 0;


public:
	static PlayerStatus& Instance() {
		static PlayerStatus instance;
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

	int GetPlayerHealth() const { return playerHealth; }
	void DecreasePlayerHealth(const int& decreaseAmount)
	{
		if (playerHealth - decreaseAmount < 0) {
			playerHealth = 0;
		}
		else {
			playerHealth -= decreaseAmount;
		}
	}

	void IncrementBugsSlapped() {
		bugsSlapped++;
	}
	size_t GetBugsSlapped() {
		return bugsSlapped;
	}

	void IncreasePlayerHealth(const int& increaseAmount) { playerHealth += increaseAmount; }

	float GetHitRadius() const { return hitRadius; }

	void SetSecondsPlayed(size_t value) { seconds_played = value; }
	size_t GetSecondsPlayed() { return seconds_played; }


};