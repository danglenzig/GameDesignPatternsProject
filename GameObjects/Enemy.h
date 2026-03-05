#pragma once
#include "../Data/Structs.h"
#include "../Singletons/PlayerStatus.h"
#include "GameObject.h"
#include "Player.h"
#include "../EventSystem/GameEvents.h"
#include <iostream>
#include "raymath.h"

class Enemy : public GameObject
{
private:
	Vector2 playerPos = { 0,0 };
	std::string playerState = "NONE";
	bool isActive = false;
	bool followPlayer = true;

	float speedSkew = 1.0f; // TODO; randomize
	float speedAdjust = 1.0f; // TODO, based on aggro & followPlayer valiue

	size_t slapHandle = -1;

	

	EnemyConfig myConfig;


public:
	Enemy(const EnemyConfig& _config)
	{
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
	void FixDrawData() override;
	void Initialize() override;
	void Activate(const Vector2& startPos);
	void Deactivate();
	void LowAggroBehaivior(const float& dT);
	void MediumAggroBehaivior(const float& dT);
	void HighAggroBehaivior(const float& dT);
	void MoveTowardPlayer(const float& dT);
	void OnPlayerSlap(const DamageSectorData& _data);

	bool IsActive() const { return isActive; }
	void SetFollowPlayer(const bool& value) { followPlayer = value; }
};

void Enemy::OnFrameUpdate(const float& dT)
{
	if (followPlayer) {
		MoveTowardPlayer(dT);
	}

	
}

void Enemy::FixDrawData()
{
	drawData.position = position;
	drawData.lookAngle = lookAngle;

	// TODO: get from config in constructor
	drawData.size = myConfig.size;

	// TODO: get from FlyweightAnimator
	drawData.ptrToTexture = nullptr;
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
		// die
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