#pragma once
#include "../GameObjects/Player.h"


// Deprecated

class EnemySpawner
{
private:
	// private constructor -- can't create from outside
	EnemySpawner() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	EnemySpawner(const EnemySpawner&) = delete;
	EnemySpawner& operator=(const EnemySpawner&) = delete;

public:

	// the ONLY way to get the instance
	static EnemySpawner& Instance() {
		static EnemySpawner instance; // created once, lives forever
		return instance;
	}

	void SetPlayerReference(const Player& _player)
	{
		//player = _player;
	}

};