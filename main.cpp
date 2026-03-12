//////////////
// main.cpp //
//////////////

#include "raylib.h"
#include "Singletons/InputHandler.h"
#include "Singletons/RenderSystem.h"
#include "Singletons/EnemySpawner.h"
#include "Singletons/UpdateRelay.h"
#include "Singletons/PlayerStatus.h"
#include "EventSystem/Event.h"
#include "EventSystem/GameEvents.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "GameObjects/EnemyFactory.h"

int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "Bug Slapper: The Slappening");
	SetTargetFPS(60);

	// Get singleton references
	InputHandler& inputHandler	= InputHandler::Instance();
	RenderSystem& renderSystem	= RenderSystem::Instance();
	GameEvents& gameEvents		= GameEvents::Instance();
	PlayerStatus& playerStatus = PlayerStatus::Instance();
	
	Player player({ 640, 360 }, 1000);
	EnemyFactory enemyFactory;
	enemyFactory.InitializeEnemyPool();
	size_t secondsPlayed = 0;

	float ta = 0.0;


	// game loop
	while (!WindowShouldClose()) {
		
		// update
		float dT = GetFrameTime();
		gameEvents.OnFrameUpdate.Invoke(dT);

		ta += dT;
		if (ta >= 1.0f) {
			ta = 0.0f;
			secondsPlayed += 1;
		}
		
		if (playerStatus.GetPlayerHealth() > 0) {
			// draw
			ClearBackground({ 85, 107,47,255 }); // olive green		

			// tell the renderSystem to render the player
			renderSystem.RenderGameObject(&player);
			// 
			// tell the enemy system to render the enemies & hit markers
			enemyFactory.RenderEnemies();
			enemyFactory.RenderHitMarkers();
			renderSystem.UpdateHud(secondsPlayed);
		}
		else {
			ClearBackground({ 85, 107,47,255 }); // olive green	
			renderSystem.RenderGameEndUI();
		}

		

		EndDrawing();
	}

}