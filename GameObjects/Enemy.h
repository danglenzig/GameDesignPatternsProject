#pragma once
#include "../Data/Structs.h"
#include "../Singletons/PlayerStatus.h"
#include "GameObject.h"
#include "Player.h"
#include "../EventSystem/GameEvents.h"
#include <iostream>
#include "raymath.h"
#include "../Animation/FlyweightAnimator.h"

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
	size_t currentFrame = 0;

	float frameTimer = 0.0f;

	EnemyConfig myConfig;

	FlyweightAnimator& animator;

public:
	Enemy(const EnemyConfig& _config, FlyweightAnimator& animatorRef) : animator(animatorRef)
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


	frameTimer += dT;
	if (frameTimer >= 1 / myConfig.fps) {
		frameTimer = 0.0f;
		currentFrame = (currentFrame + 1) % myConfig.numberOfFrames;
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