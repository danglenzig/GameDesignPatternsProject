#pragma once
#include "../GameObjects/GameObject.h"
#include "raylib.h"
#include "PlayerStatus.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>



class RenderSystem
{
private:
	RenderSystem()
	{
		
	}
	
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	std::string bgPath = "Resources/Textures/Grass.png";

public:
	static RenderSystem& Instance() {
		static RenderSystem instance;
		return instance;
	}

	void UpdateHud(const size_t secondsPlayed)
	{
		DrawText("WASD to move...", 10, 10, 30, BLACK);
		DrawText("Left-click to slap...", 10, 40, 30, BLACK);


		int playerHealth = PlayerStatus::Instance().GetPlayerHealth();
		std::string phStr = "Player Health: " + std::to_string(playerHealth);
		DrawText(phStr.c_str(), 10, 90, 30, BLACK);

		std::string secondsPlayedStr = "Seconds Played: " + std::to_string(secondsPlayed);
		DrawText(secondsPlayedStr.c_str(), 10, 130, 30, BLACK);

		size_t beesSlapped = PlayerStatus::Instance().GetBugsSlapped();
		std::string bugsSlappedStr = "Bees Slapped: " + std::to_string(beesSlapped);
		DrawText(bugsSlappedStr.c_str(), 10, 170, 30, BLACK);
	}

	void RenderBackground()
	{
		Texture t = LoadTexture(bgPath.c_str());
		DrawTexturePro(
			t,
			{ 0 ,0, 1280, 720 },
			{ 0, 0, 1280, 720 },
			{ 1280, 720 },
			0.0f,
			WHITE

		);

	}

	void RenderGameEndUI()
	{
		
		DrawText("Thank you for playing. Goodbye.", 10, 320, 40, {128,0,0,255});
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