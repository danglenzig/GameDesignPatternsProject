#pragma once
#include <iostream>
#include <vector>
#include "raylib.h"
#include <functional>


enum EnumAnimEvent
{
	STARTED,
	FINISHED,
	LOOPED,
	FRAME_UPDATED
};
struct AnimEvent
{
	std::string animName;
	EnumAnimEvent event;
};

struct AnimationConfig
{
	std::string name;
	std::vector<std::string> framePaths;
	float fps = 1.0f;
};
struct Animation
{
	std::string name;
	std::vector<Texture2D> frames;
	float fps = 1.0f;
	float interval = 1.0f;
	size_t currentFrame = 0;
	float timer = 0.0f;
	bool loop = true;
};

class PlayerAnimator
{
private:
	void Initialize();
	void LoadAnimations(const std::vector<AnimationConfig>& configs);
public:
	PlayerAnimator() {
		Initialize();
	}
	void OnUpdate(const float& dT);
	void Play(const std::string& animName, bool loop = true);
	Texture2D* GetPtrToCurrentTexture();
	bool IsPlaying(const std::string& animName) const;
	std::string GetCurrentAnimName() const;
	size_t GetCurrentFrame() const;
	void AddListener(std::function<void(const AnimEvent&)> cBack);
	void Dispatch(const AnimEvent& animEvent);
};



void PlayerAnimator::Initialize() 
{
	std::vector<AnimationConfig> consfigs = {

	};
}

void PlayerAnimator::LoadAnimations(const std::vector<AnimationConfig>& configs)
{

}

void PlayerAnimator::OnUpdate(const float& dT)
{

}

void PlayerAnimator::Play(const std::string& animName, bool loop = true)
{

}

Texture2D* PlayerAnimator::GetPtrToCurrentTexture()
{

}

bool PlayerAnimator::IsPlaying(const std::string& animName) const
{

}

std::string PlayerAnimator::GetCurrentAnimName() const
{

}

size_t PlayerAnimator::GetCurrentFrame() const
{

}

void PlayerAnimator::AddListener(std::function<void(const AnimEvent&)> cBack)
{

}

void PlayerAnimator::Dispatch(const AnimEvent& animEvent)
{

}
