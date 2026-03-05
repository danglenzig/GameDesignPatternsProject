#pragma once
#include "raylib.h"
//#include "../EventSystem/Event.h"
#include "../EventSystem/GameEvents.h"

struct DrawData
{
	Vector2 position;
	Vector2 size;
	float lookAngle;
	Texture2D* ptrToTexture;
};

// parent class for player and enemies
class GameObject
{
protected:
	size_t updateHandle = -1;
	Vector2 position = { 0,0 };
	Vector2 size = { 0,0 };
	float lookAngle = 0.0f;
	DrawData drawData;

	

private:
	//
public:
	GameObject() {
		drawData.position		= { 0,0 };
		drawData.size			= { 0,0 };
		drawData.lookAngle		= 0.0f;
		drawData.ptrToTexture	= nullptr;

		//Initialize();

		updateHandle = GameEvents::Instance().OnFrameUpdate.Subscribe(
			[this](const float& dT) {
				OnFrameUpdate(dT);
			}
		);
	}

	virtual ~GameObject() {
		GameEvents::Instance().OnFrameUpdate.Unsubscribe(updateHandle);
	}

	virtual void OnFrameUpdate(const float& dT) {}
	virtual void FixDrawData() {}

	// child-specific setup tasks
	virtual void Initialize() {}


	DrawData GetDrawData()
	{
		return drawData;
	}
};