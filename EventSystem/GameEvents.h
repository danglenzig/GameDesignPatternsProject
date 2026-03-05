#pragma once
#include "raylib.h"
#include "Event.h"


struct DamageSectorData
{
	Vector2 position;
	float radius;
	float angleStart;
	float angleEnd;
};

class GameEvents
{
private:
	GameEvents() = default;
public:
	static GameEvents& Instance()
	{
		static GameEvents instance;
		return instance;
	}

	Event<float> OnFrameUpdate;
	Event<DamageSectorData> OnPlayerSlap;
	SimpleEvent OnAttackInput;
};
