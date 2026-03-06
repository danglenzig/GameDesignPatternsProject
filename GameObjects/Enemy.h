#pragma once
#include "../Data/Structs.h"
#include "../Singletons/PlayerStatus.h"
#include "GameObject.h"
#include "Player.h"
#include "../EventSystem/GameEvents.h"
#include <iostream>
#include "raymath.h"
#include "raylib.h"
#include "../Animation/FlyweightAnimator.h"

class Enemy : public GameObject
{
private:
	Vector2 playerPos = { 0,0 };
	std::string playerState = "NONE";
	bool isActive = false;
	bool followPlayer = true;
	bool recycling = false;

	float speedSkew = 1.0f; // TODO; randomize
	float speedAdjust = 1.0f; // TODO, based on aggro & followPlayer valiue

	size_t slapHandle = -1;
	size_t currentFrame = 0;

	float frameTimer = 0.0f;

	EnemyConfig myConfig;

	FlyweightAnimator& animator;

	void HandleInPlayerHitRadius();
	void LowAggroBehaivior(const float& dT);
	void MediumAggroBehaivior(const float& dT);
	void HighAggroBehaivior(const float& dT);
	void MoveTowardPlayer(const float& dT);
	void MoveTowardGatherPoint(const float& dT);
	void OnPlayerSlap(const DamageSectorData& _data);

	std::vector<Vector2> recyclePoints = {
		{ 0, 360	},
		{ 1280, 360	},
		{ 500, 0	},
		{ 500, 720	},
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

	std::vector<Vector2> gatherPoints = {
		{ 256, 180	},
		{ 256, 360	},
		{ 256, 540	},
		{ 512, 180	},
		{ 512, 360	},
		{ 512, 540	},
		{ 768, 180	},
		{ 768, 360	},
		{ 768, 540	},
		{ 1024, 180 },
		{ 1024, 360 },
		{ 1024, 540 },
	};
	Vector2 currentGatherPoint = { 256,180 };

	float modeInterval = 3.0f;
	float modeTimer = 0.0f;


public:
	Enemy(const EnemyConfig& _config, FlyweightAnimator& animatorRef) : animator(animatorRef)
	{
		float randomFloat = (float)GetRandomValue(-500, 500) / 1000.0f;
		modeInterval += (modeInterval * randomFloat);
		myConfig = _config;
		slapHandle = GameEvents::Instance().OnPlayerSlap.Subscribe(
			[this](const DamageSectorData& _data) {
				OnPlayerSlap(_data);
			}
		);
		
	}
	~Enemy()
	{
		GameEvents::Instance().OnPlayerSlap.Unsubscribe(slapHandle);
	}

	void OnFrameUpdate(const float& dT) override;
	void OnUpdate(const float& dT);

	void FixDrawData() override;
	void Initialize() override;
	void Activate(const Vector2& startPos);
	void Deactivate();
	
	void Recycle();

	bool IsActive() const { return isActive; }
	void SetFollowPlayer(const bool& value) { followPlayer = value; }
	void SetEnemyConfig(const EnemyConfig& newConfig) { myConfig = newConfig; }
	void AdjustSpeedSkew(const float& adjust) { speedSkew += adjust; }
	std::string GetUUID() const { return myConfig.uuid; }
};

void Enemy::OnFrameUpdate(const float& dT)
{
	// not working (?)
}
// patch fix for^^
void Enemy::OnUpdate(const float& dT)
{
	const float hitRadius = PlayerStatus::Instance().GetHitRadius();
	const Vector2 playerPos = PlayerStatus::Instance().GetPlayerPosition();
	if (Vector2DistanceSqr(position, playerPos) <= hitRadius * hitRadius) {
		HandleInPlayerHitRadius();
		return;
	}

	if (followPlayer) {
		MoveTowardPlayer(dT);
	}
	else {
		MoveTowardGatherPoint(dT);
	}

	frameTimer += dT;
	if (frameTimer >= 1 / myConfig.fps) {
		frameTimer = 0.0f;
		currentFrame = (currentFrame + 1) % myConfig.numberOfFrames;
	}

	modeTimer += dT;
	if (modeTimer >= modeInterval) {
		modeTimer = 0.0f;
		// flip a coin
		if ((GetRandomValue(0, 9) % 2) == 0) {

			int randoIdx = GetRandomValue(0, gatherPoints.size() - 1);
			currentGatherPoint = gatherPoints[randoIdx];

			followPlayer = !followPlayer;
		}
	}
}




void Enemy::FixDrawData()
{
	drawData.position = position;
	drawData.lookAngle = lookAngle;
	drawData.size = myConfig.size;
	drawData.ptrToTexture = animator.GetPtrToTexture(myConfig.configName, currentFrame);
}

void Enemy::Initialize()
{
	
}

void Enemy::Activate(const Vector2& startPos)
{

	
	//...
	position = startPos;
	isActive = true;
}

void Enemy::Deactivate()
{
	isActive = false;
}

void Enemy::Recycle()
{
	isActive = false;
	// TODO: recycle behavior
}

void Enemy::MoveTowardPlayer(const float& dT)
{
	playerPos = PlayerStatus::Instance().GetPlayerPosition();
	playerState = PlayerStatus::Instance().GetPlayerState();

	Vector2 direction = Vector2Subtract(playerPos, position);
	direction = Vector2Normalize(direction);
	
	// TODO skew direction angle

	float distanceThisFrame = myConfig.speed * speedSkew * speedAdjust * dT;
	lookAngle = Vector2Angle({ 1,0 }, direction);
	position = Vector2Add(position, Vector2Scale(direction, distanceThisFrame));

	// TODO: handle animation

	FixDrawData();
}

void Enemy::MoveTowardGatherPoint(const float& dT)
{
	if (Vector2DistanceSqr(position, currentGatherPoint) <= 2.0f) {
		followPlayer = true;
		return;
	}


	Vector2 direction = Vector2Subtract(currentGatherPoint, position);
	direction = Vector2Normalize(direction);
	float distanceThisFrame = myConfig.speed * speedSkew * speedAdjust * dT;
	lookAngle = Vector2Angle({ 1,0 }, direction);
	position = Vector2Add(position, Vector2Scale(direction, distanceThisFrame));

	FixDrawData();
}

void Enemy::HandleInPlayerHitRadius()
{
	size_t randoIdx = GetRandomValue(0, recyclePoints.size() - 1);
	position = recyclePoints[randoIdx];
	PlayerStatus::Instance().DecreasePlayerHealth(myConfig.damage);
	GameEvents::Instance().DamageTaken.Invoke();
}


void Enemy::OnPlayerSlap(const DamageSectorData& slapData)
{
	bool withinRadius = Vector2DistanceSqr(position, slapData.position) <= (slapData.radius * slapData.radius);
	bool withinAngle = false;

	Vector2 directionToEnemy = Vector2Subtract(position, slapData.position);
	directionToEnemy = Vector2Normalize(directionToEnemy);

	float angleToEnemy = atan2f(directionToEnemy.y, directionToEnemy.x);

	bool angleRangeWraps = slapData.angleEnd < slapData.angleStart;

	if (angleRangeWraps) {
		withinAngle = (angleToEnemy > slapData.angleStart || angleToEnemy < slapData.angleEnd);
	}
	else {
		withinAngle = (angleToEnemy >= slapData.angleStart && angleToEnemy <= slapData.angleEnd);
	}

	if (withinRadius && withinAngle)
	{
		GameEvents::Instance().OnEnemyDied.Invoke(myConfig);
	}

}


void Enemy::LowAggroBehaivior(const float& dT)
{

}
void Enemy::MediumAggroBehaivior(const float& dT)
{

}
void Enemy::HighAggroBehaivior(const float& dT)
{

}