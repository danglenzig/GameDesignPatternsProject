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

enum EnumPlayerState {
	IDLE_STATE,
	WALKING_STATE,
	ATTACKING_STATE
};



class Player : public GameObject
{
private:
	const std::string IDLE = "IDLE";
	const std::string WALKING = "WALKING";
	const std::string ATTACKING = "ATTACKING";
	const std::unordered_map<std::string, EnumPlayerState> statesMap = {
		{"IDLE", IDLE_STATE},
		{"WALKING", WALKING_STATE},
		{"ATTACKING", ATTACKING_STATE}
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
	
	void ConfigureFSM();
	void OnAttackInput();
	void OnStateEvent(const StateEvent& e);
	void OnStateEntered(const std::string& stateName);
	void OnStateExited(const std::string& stateName);
	EnumPlayerState GetStateNameAsEnum(const std::string& stateName);
	void IdleUpdateBehavior(const float& dT, const Vector2& moveInput);
	void WalkingUpdateBehavior(const float& dT, const Vector2& moveInput);
	void AttackingUpdateBehavior(const float& dT, const Vector2& moveInput);

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
		GameEvents::Instance().OnAttackInput.Subscribe(
			[this]() {
				OnAttackInput();
			}
		);
	}
};

void Player::OnFrameUpdate(const float& dT) 
{
	PlayerStatus::Instance().SetPlayerPosition(position);
	animator.OnUpdate(dT);

	Vector2 moveInput = InputHandler::Instance().GetMoveInput();
	//std::cout << "( " << moveInput.x << ", " << moveInput.y << " )\n";

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
	}
}

void Player::IdleUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	if (Vector2LengthSqr(moveInput) > FLT_EPSILON * FLT_EPSILON) {
		playerFSM.TryTransition(WALKING);
		return;
	}
	InputHandler::Instance().GetAttackInput();
	FixDrawData();
}
void Player::WalkingUpdateBehavior(const float& dT, const Vector2& moveInput)
{
	if (Vector2LengthSqr(moveInput) <= FLT_EPSILON * FLT_EPSILON) {
		playerFSM.TryTransition(IDLE);
		return;
	}
	InputHandler::Instance().GetAttackInput();
	Vector2 newPos = position + (moveInput * speed * dT);
	position = newPos;

	FixDrawData();
}
void Player::AttackingUpdateBehavior(const float& dT, const Vector2& moveInput)
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
	playerFSM.AddTransition(IDLE, WALKING);
	playerFSM.AddTransition(IDLE, ATTACKING);

	playerFSM.AddTransition(WALKING, IDLE);
	playerFSM.AddTransition(WALKING, ATTACKING);

	playerFSM.AddTransition(ATTACKING, IDLE);

	playerFSM.AddListener(
		[this](const StateEvent& event) {
			OnStateEvent(event);
		}
	);
	OnStateEntered(IDLE);
	PlayerStatus::Instance().SetPlayerState(playerFSM.currentStateName);
}

void Player::OnAttackInput()
{
	// TODO: transition to attacking state
	std::cout << "SLAP!\n";
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
		// calculate a damage area
		// invoke GameEvents::Instance().OnPlayerSlap with damage area
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