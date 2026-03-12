#pragma once

#include "GameObject.h"
#include "../Singletons/InputHandler.h"
#include "../StateMachine/StateMachine.h"
#include "../Singletons/PlayerStatus.h"
#include "raylib.h"
#include <vector>
#include <unordered_map>
#include "../EventSystem/GameEvents.h"
#include "../Animation/PlayerAnimator.h"
#include <algorithm>

enum EnumPlayerState {
	IDLE_STATE,
	WALKING_STATE,
	ATTACKING_STATE,
	REACTING_STATE
};



class Player : public GameObject
{
private:
	const std::string IDLE = "IDLE";
	const std::string WALKING = "WALKING";
	const std::string ATTACKING = "ATTACKING";
	const std::string REACTING = "REACTING";
	const std::unordered_map<std::string, EnumPlayerState> statesMap = {
		{"IDLE", IDLE_STATE},
		{"WALKING", WALKING_STATE},
		{"ATTACKING", ATTACKING_STATE},
		{"REACTING", REACTING_STATE}
	};

	float speed = 300.0f;
	FSM playerFSM;
	int health;
	float damageSectorRadius = 150.0f;
	float damageSectorAngle = PI / 3.0f; // 60 degrees
	float attackCooldown = 0.1f;
	float cooldownTimer = 0.0f;
	size_t attackInputHandle = -1;
	PlayerAnimator animator;

	const float REACT_COOLDOWN = 1.0f;
	float reactCooldownTimer = 0.0f;
	bool onReactCooldown = false;
	
	void ConfigureFSM();
	void OnAttackInput();
	void OnStateEvent(const StateEvent& e);
	void OnStateEntered(const std::string& stateName);
	void OnStateExited(const std::string& stateName);
	EnumPlayerState GetStateNameAsEnum(const std::string& stateName);
	void IdleUpdateBehavior(const float& dT, const Vector2& moveInput);
	void WalkingUpdateBehavior(const float& dT, const Vector2& moveInput);
	void AttackingUpdateBehavior(const float& dT, const Vector2& moveInput);
	void ReactingUpdateBehavior(const float& dT, const Vector2& moveInput);
	void OnDamageTaken();
	void HandleAnimationEvent(const AnimEvent& event);
	DamageSectorData GetDamageSectorArea();

public:
	Vector2 GetPosition() const { return position; }
	void OnFrameUpdate(const float& dT) override;
	void FixDrawData() override;
	void Initialize() override;

	Player(const Vector2& startPos, const int& startHealth) {
		position = startPos;
		health = startHealth;
		ConfigureFSM();
		FixDrawData();
		PlayerStatus::Instance().IncreasePlayerHealth(health + 1);
		GameEvents::Instance().OnAttackInput.Subscribe(
			[this]() {
				OnAttackInput();
			}
		);
	}
};

void Player::OnFrameUpdate(const float& dT) 
{

	if (onReactCooldown) {
		reactCooldownTimer += dT;
		if (reactCooldownTimer >= REACT_COOLDOWN) {
			onReactCooldown = false;
			reactCooldownTimer = 0.0f;
		}
	}

	PlayerStatus::Instance().SetPlayerPosition(position);
	animator.OnUpdate(dT);

	Vector2 moveInput = InputHandler::Instance().GetMoveInput();

	switch (GetStateNameAsEnum(playerFSM.currentStateName)) {
	case EnumPlayerState::IDLE_STATE:
		IdleUpdateBehavior(dT, moveInput);
		break;
	case EnumPlayerState::WALKING_STATE:
		WalkingUpdateBehavior(dT, moveInput);
		break;
	case EnumPlayerState::ATTACKING_STATE:
		AttackingUpdateBehavior(dT, moveInput);
		break;
	case EnumPlayerState::REACTING_STATE:
		ReactingUpdateBehavior(dT, moveInput);
		break;
	}
}

void Player::IdleUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	if (Vector2LengthSqr(moveInput) > FLT_EPSILON) {
		playerFSM.TryTransition(WALKING);
		return;
	}
	InputHandler::Instance().GetAttackInput();
	FixDrawData();
}
void Player::WalkingUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	if (Vector2LengthSqr(moveInput) <= FLT_EPSILON) {
		playerFSM.TryTransition(IDLE);
		return;
	}
	InputHandler::Instance().GetAttackInput();
	Vector2 newPos = position + (moveInput * speed * dT);

	newPos.x = std::clamp(newPos.x, 20.0f, 1260.0f);
	newPos.y = std::clamp(newPos.y, 20.0f, 700.0f);

	position = newPos;

	FixDrawData();
}
void Player::AttackingUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	FixDrawData();
}
void Player::ReactingUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	
	FixDrawData();
}



void Player::FixDrawData() 
{
	drawData.position = position;
	drawData.size = { 270, 270 };
	if (playerFSM.currentStateName == WALKING) {
		drawData.lookAngle = Vector2Angle({ 1,0 }, InputHandler::Instance().GetMoveInput());
	}
	
	// fix after animator is ready
	drawData.ptrToTexture = animator.GetPtrToCurrentTexture();
}

void Player::ConfigureFSM()
{
	playerFSM.Initialize(IDLE);
	playerFSM.AddState(ATTACKING);
	playerFSM.AddState(WALKING);
	playerFSM.AddState(REACTING);

	playerFSM.AddTransition(IDLE, WALKING);
	playerFSM.AddTransition(IDLE, ATTACKING);
	playerFSM.AddTransition(IDLE, REACTING);

	playerFSM.AddTransition(WALKING, IDLE);
	playerFSM.AddTransition(WALKING, ATTACKING);
	playerFSM.AddTransition(WALKING, REACTING);

	playerFSM.AddTransition(REACTING, IDLE);
	playerFSM.AddTransition(ATTACKING, IDLE);

	playerFSM.AddListener(
		[this](const StateEvent& event) {
			OnStateEvent(event);
		}
	);
	OnStateEntered(IDLE);
	PlayerStatus::Instance().SetPlayerState(playerFSM.currentStateName);


	

	GameEvents::Instance().DamageTaken.Subscribe(
		[this]() {
			if (playerFSM.currentStateName != REACTING) {
				OnDamageTaken();
			}
		}
	);

	animator.AddListener(
		[this](const AnimEvent& event) {
			HandleAnimationEvent(event);
		}
	);

}

void Player::HandleAnimationEvent(const AnimEvent& event)
{
	if (event.event == EnumAnimEvent::FINISHED) {
		if (event.animName == REACTING) {
			playerFSM.TryTransition(IDLE);
			return;
		}
		if (event.animName == ATTACKING) {
			playerFSM.TryTransition(IDLE);
			return;
		}

	}
}

void Player::OnAttackInput()
{
	if (playerFSM.currentStateName != ATTACKING) {
		playerFSM.TryTransition(ATTACKING);
	}



}

void Player::OnDamageTaken()
{
	if (onReactCooldown) return;
	reactCooldownTimer = 0.0f;
	onReactCooldown = true;

	playerFSM.TryTransition(REACTING);
}

void Player::OnStateEvent(const StateEvent& e) 
{
	switch (e.eventType) {
	case EnumStateEventType::STATE_ENTERED:
		OnStateEntered(e.stateName);
		break;
	case EnumStateEventType::STATE_EXITED:
		OnStateExited(e.stateName);
		break;
	default: break;
	}
}

void Player::OnStateEntered(const std::string& stateName)
{
	switch (GetStateNameAsEnum(stateName)) {
	case EnumPlayerState::IDLE_STATE:
		// tell the animator to be playing the IDLE anim
		animator.Play(IDLE, true);
		break;
	case EnumPlayerState::WALKING_STATE:
		// tell the animator to be playing the WALKING anim
		animator.Play(WALKING, true);
		break;
	case EnumPlayerState::ATTACKING_STATE:
		// tell the animator to be playing the ATTACKING anim
		animator.Play(ATTACKING, false);
		// calculate a damage area
		// invoke GameEvents::Instance().OnPlayerSlap with damage area
		GameEvents::Instance().OnPlayerSlap.Invoke(GetDamageSectorArea());
		break;
	case EnumPlayerState::REACTING_STATE:
		animator.Play(REACTING, false);
		break;
	}
	
	PlayerStatus::Instance().SetPlayerState(playerFSM.currentStateName);
}

void Player::Initialize()
{
	// depricated
}

void Player::OnStateExited(const std::string& stateName)
{
	switch (GetStateNameAsEnum(stateName)) {
	case EnumPlayerState::IDLE_STATE:
		break;
	case EnumPlayerState::WALKING_STATE:
		break;
	case EnumPlayerState::ATTACKING_STATE:
		break;
	}
}

EnumPlayerState Player::GetStateNameAsEnum(const std::string& stateName)
{
	auto iterator = statesMap.find(stateName);
	if (iterator != statesMap.end()) {
		return iterator->second;
	}
	std::cerr << "Error: '" << stateName << "' is not a valid Player State.\n";
}

DamageSectorData Player::GetDamageSectorArea()
{
	// create a sector in front of the player that can damage enemies
	// this will be called in the ATTACKING state, and will be timed to the anim events

	// the sector will be a circle with a radius of damageSectorRadius, centered on the player, but only in the direction the player is facing (so like a slice of pie)
	// How?
	// 1. get the direction the player is facing (use the lookAngle in drawData)
	// 2. get the position of the player
	// 3. create a sector that starts at lookAngle - some angle, and ends at lookAngle + some angle (so like a slice of pie)

	const float OFFSET = 10.0f;
	Vector2 facingDir = {
		cosf(drawData.lookAngle), sinf(drawData.lookAngle)
	};
	DamageSectorData sector = {};
	sector.position = {
		drawData.position.x + (facingDir.x * OFFSET),
		drawData.position.y + (facingDir.y * OFFSET)
	};
	sector.radius = damageSectorRadius;
	sector.angleStart = drawData.lookAngle - (damageSectorAngle / 2.0f);
	sector.angleEnd = drawData.lookAngle + (damageSectorAngle / 2.0f);
	return sector;

}