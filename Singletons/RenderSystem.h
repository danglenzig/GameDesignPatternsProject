#pragma once
#include "../GameObjects/GameObject.h"
#include "raylib.h"
#include "PlayerStatus.h"
#include <string>



class RenderSystem
{
private:
	// private constructor -- can't create from outside
	RenderSystem() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

public:

	// the ONLY way to get the instance
	static RenderSystem& Instance() {
		static RenderSystem instance; // created once, lives forever
		return instance;
	}

	void UpdateHud(const size_t secondsPlayed)
	{
		DrawText("WASD to move...", 10, 10, 30, BLACK);
		int playerHealth = PlayerStatus::Instance().GetPlayerHealth();
		std::string phStr = "Player Health: " + std::to_string(playerHealth);
		DrawText(phStr.c_str(), 10, 50, 30, BLACK);

		std::string secondsPlayedStr = "Seconds Played: " + std::to_string(secondsPlayed);
		DrawText(secondsPlayedStr.c_str(), 10, 90, 30, BLACK);

	}

	void RenderGameObject(GameObject* ptrToGameObject)
	{
		if (!ptrToGameObject) return;
		DrawData drawData = ptrToGameObject->GetDrawData();

		// if no texture then draw red square
		if (!drawData.ptrToTexture) {
			DrawRectanglePro(
				Rectangle{ drawData.position.x - 16, drawData.position.y - 16, 32,32 },
				{ 16,16 },
				drawData.lookAngle * RAD2DEG,
				RED
			);
			return;
		}
		else {
			DrawTexturePro(
				*drawData.ptrToTexture,
				{ 0,0, (float)drawData.ptrToTexture->width, (float)drawData.ptrToTexture->height },
				{ drawData.position.x, drawData.position.y, drawData.size.x, drawData.size.y },
				{ drawData.size.x / 2, drawData.size.y / 2 },
				drawData.lookAngle * RAD2DEG,
				WHITE
			);
		}
	}

};