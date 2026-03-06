#pragma once
#include "../Data/Structs.h"
#include "../Singletons/MiscTools.h"
#include "GameObject.h"
#include "../Animation/FlyweightAnimator.h"
#include "Enemy.h"
#include <vector>
#include <algorithm>

class EnemyFactory
{
private:

	const int minSkewAdjust = -500;
	const int maxSkewAdjust = 1500;

	const size_t poolBatchSize = 500;
	float spawnInterval = 1.0f;
	float spawnTimer = 0.0f;
	FlyweightAnimator animator;
	bool beSpawning = true;




	size_t toSpawn = 10;
	size_t updateHandle = -1;
	size_t diedHandle = - 1;

	std::vector<Enemy> enemyPool;
	// std::vector<HitMarker> hitMarkerPool

	
	void OnEnemyDied(const EnemyConfig& enemyConfig);
	
	void OnFrameUpdate(const float& dT);
	void SpawnEnemy();
	std::vector<Enemy*> GetActiveEnemyPtrs();

	/*
	std::vector<Vector2> recyclePoints = {
		{ 0, 360	},
		{ 1280, 360	},
		{ 500, 0	},
		{ 500, 720	},
	};
	*/

	std::vector<Vector2> spawnPoints = {
		{ 0, 0		},
		{ 320, 0	},
		{ 640, 0	},
		{ 960, 0	},
		{ 1280, 0	},
		{ 0, 720	},
		{ 320, 720	},
		{ 640, 720	},
		{ 960, 720	},
		{ 1280, 720	},
		{ 0, 240	},
		{ 0, 480	},
		{ 0, 720	},
		{ 1280, 240 },
		{ 1280, 480 },
		{ 1280, 720 }
	};

	EnemyConfig GetSkeeterConfig() const {
		EnemyConfig skeeterConfig = {
		"SKEETER",
		80.0f,
		{50,50},
		EnumAggro::LOW,
		10.0f,
		2,
		2.0f,
		MiscTools::Instance().GetUUID()
		};
		return skeeterConfig;
	};


public:
	void RenderEnemies();
	void RenderHitMarkers();
	void InitializeEnemyPool();

	EnemyFactory() {
		//InitializeEnemyPool();
		diedHandle = GameEvents::Instance().OnEnemyDied.Subscribe(
			[this](const EnemyConfig& _config) {
				OnEnemyDied(_config);
			}
		);
		updateHandle = GameEvents::Instance().OnFrameUpdate.Subscribe(
			[this](const float& dT) {
				OnFrameUpdate(dT);
			}
		);
	}


};


void EnemyFactory::InitializeEnemyPool()
{
	// for now, just skeeters
	for (int i = 0; i < poolBatchSize; i++) {
		Enemy enemy(GetSkeeterConfig(), animator);
		enemyPool.push_back(enemy);

		//std::cout << "Adding " << enemy.GetUUID() << " -- Active: " << enemy.IsActive() <<"\n";
		
		// add a hit marker to the hit marker pool
	}

}



void EnemyFactory::RenderEnemies()
{
	for (Enemy* ptr : GetActiveEnemyPtrs()) {
		if (ptr) {
			RenderSystem::Instance().RenderGameObject(ptr);
		}
	}

}
void EnemyFactory::RenderHitMarkers()
{
	//
}

void EnemyFactory::OnEnemyDied(const EnemyConfig& enemyConfig)
{
	std::string uuid = enemyConfig.uuid;
	// find the enemy
	auto iterator = std::find_if(
		enemyPool.begin(), enemyPool.end(),
		[&uuid](const Enemy& enemy) {
			return enemy.GetUUID() == uuid;
		}
	);
	// check if actually found
	if (iterator != enemyPool.end()) {
		// enemy found
		//SpawnHitMarker(iterator->GetDrawData().position);
		iterator->Recycle();

		// TODO
		// update UI
		// etc...

	}
}

void EnemyFactory::SpawnEnemy()
{
	//std::cout << "[EnemyFactory.h] Attempting spawn...\n";

	// find the first enemy in the pool that is not active
	auto iterator = std::find_if(
		enemyPool.begin(), enemyPool.end(),
		[](const Enemy& enemy) {
			return !enemy.IsActive();
		}
	);

	// check if actually found
	if (iterator != enemyPool.end()) {
		size_t randoSpawnIdx = GetRandomValue(0, spawnPoints.size() - 1);

		float randoSpeedAdjust = (float)GetRandomValue(minSkewAdjust, maxSkewAdjust) / 1000.0f;
		iterator->AdjustSpeedSkew(randoSpeedAdjust);
		iterator->Activate(spawnPoints[randoSpawnIdx]);
	}
	else {
		// re-up the pool
	}

	
	
}

void SpawnHitMarker(const Vector2 position)
{

}

std::vector<Enemy*> EnemyFactory::GetActiveEnemyPtrs()
{
	std::vector<Enemy*> activeEnemyPtrs;
	size_t poolSize = enemyPool.size();
	activeEnemyPtrs.reserve(poolSize);

	for (auto& enemy : enemyPool) {
		if (enemy.IsActive()) {
			activeEnemyPtrs.push_back(&enemy);
		}
	}

	return activeEnemyPtrs;
}

void EnemyFactory::OnFrameUpdate(const float& dT) 
{

	for (Enemy* ptr : GetActiveEnemyPtrs()) {
		if (ptr) {
			ptr->OnUpdate(dT);
			//RenderSystem::Instance().RenderGameObject(ptr);
		}
	}
	


	if (beSpawning) {
		spawnTimer += dT;
		if (spawnTimer >= spawnInterval) {
			spawnTimer = 0.0f;
			SpawnEnemy();
		}
	}
}

