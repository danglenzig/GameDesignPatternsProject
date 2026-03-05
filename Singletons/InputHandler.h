#pragma once
#include "raylib.h"
#include"raymath.h"
#include "../EventSystem/GameEvents.h"
#include <iostream>

struct Foo {
	std::string bar = "BAR";
};

class InputHandler
{
private:
	// private constructor -- can't create from outside
	InputHandler(){}

	// delete copy constructor and assignment operator
	// This prevents duplication
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

public:

	// the ONLY way to get the instance
	static InputHandler& Instance() {
		static InputHandler instance; // created once, lives forever
		return instance;
	}

	void TestInputHandler() {
		std::cout << "Working!!\n";
	}

	Vector2 GetMoveInput() {
		float moveX = 0.0f;
		float moveY = 0.0f;
		if (IsKeyDown(KEY_W)) moveY -= 1.0f;
		if (IsKeyDown(KEY_S)) moveY += 1.0f;
		if (IsKeyDown(KEY_A)) moveX -= 1.0f;
		if (IsKeyDown(KEY_D)) moveX += 1.0f;
		Vector2 moveInput = Vector2Normalize({ moveX, moveY });
		return moveInput;
	}

	void GetAttackInput() {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			GameEvents::Instance().OnAttackInput.Invoke();
		}
	}

	

};
