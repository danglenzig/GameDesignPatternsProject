#pragma once
#include <iostream>
#include <vector>
#include "raylib.h"
#include <functional>
#include <unordered_map>
#include <algorithm>


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
	std::vector<std::function<void(const AnimEvent&)>> listeners;
private:
	void Initialize();
	void LoadAnimations(const std::vector<AnimationConfig>& configs);
	std::vector<Animation> animations;
	Animation* ptrToCurrentAnim = nullptr;
	std::unordered_map<std::string, size_t> animIndexMap;
	Animation* GetPtrToAnimation(const std::string animName);

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
	std::vector<AnimationConfig> configs = {
		{"IDLE", {"Resources/Textures/POSEY_IDLE_01.png"}, 1.0f},
		{"WALKING",
			{
				"Resources/Textures/POSEY_RUN_00.png",
				"Resources/Textures/POSEY_RUN_01.png",
				"Resources/Textures/POSEY_RUN_02.png",
				"Resources/Textures/POSEY_RUN_03.png",
				"Resources/Textures/POSEY_RUN_04.png",
				"Resources/Textures/POSEY_RUN_05.png",
			}, 12.0f
		},
		{"REACTING",
			{
				"Resources/Textures/POSEY_REACT_01.png",
				"Resources/Textures/POSEY_REACT_03.png",
				"Resources/Textures/POSEY_REACT_05.png",
				"Resources/Textures/POSEY_REACT_07.png",
				"Resources/Textures/POSEY_REACT_09.png",
				"Resources/Textures/POSEY_REACT_11.png",
				"Resources/Textures/POSEY_REACT_13.png",
				"Resources/Textures/POSEY_REACT_15.png",
				"Resources/Textures/POSEY_REACT_17.png",
			}, 20.0f
		}
	};

	LoadAnimations(configs);

}

void PlayerAnimator::LoadAnimations(const std::vector<AnimationConfig>& configs)
{
	animations.clear();
	animIndexMap.clear();

	for (const auto& config : configs) {
		Animation anim;
		anim.name = config.name;
		anim.fps = std::clamp(config.fps, 1.0f, 60.0f);
		anim.interval = 1.0f / anim.fps;
		animIndexMap[config.name] = animations.size();

		for (const auto& path : config.framePaths) {
			Texture2D tex = LoadTexture(path.c_str());
			if (tex.id == 0) {
				std::cerr << "Failed to load texture: " << path << "\n";
			}
			anim.frames.push_back(tex);
		}
		animations.push_back(anim);
	}
}

void PlayerAnimator::OnUpdate(const float& dT)
{
	if (ptrToCurrentAnim == nullptr || ptrToCurrentAnim->frames.empty()) return;

	ptrToCurrentAnim->timer += dT;
	if (ptrToCurrentAnim->timer < ptrToCurrentAnim->interval) return;
	ptrToCurrentAnim->timer = 0.0f;
	ptrToCurrentAnim->currentFrame++;
	std::string animName = ptrToCurrentAnim->name;

	if (ptrToCurrentAnim->currentFrame >= ptrToCurrentAnim->frames.size()) {
		if (ptrToCurrentAnim->loop) {
			ptrToCurrentAnim->currentFrame = 0;
			Dispatch({ ptrToCurrentAnim->name, EnumAnimEvent::LOOPED });
		}
		else
		{
			if (!ptrToCurrentAnim->frames.empty()) {
				ptrToCurrentAnim->currentFrame = ptrToCurrentAnim->frames.size() - 1;
			}
			Dispatch({ animName, EnumAnimEvent::FINISHED });
		}
		return;
	}
	Dispatch({ ptrToCurrentAnim->name, EnumAnimEvent::FRAME_UPDATED });
}

void PlayerAnimator::Play(const std::string& animName, bool loop)
{
	auto ptrToAnim = GetPtrToAnimation(animName);
	if (ptrToAnim == nullptr) {
		std::cerr << "Animation " << animName << " not found!\n";
		return;
	}
	ptrToCurrentAnim = ptrToAnim;
	ptrToCurrentAnim->loop = loop;
	ptrToCurrentAnim->currentFrame = 0;
	ptrToCurrentAnim->timer = 0.0f;
	Dispatch({ ptrToCurrentAnim->name, EnumAnimEvent::STARTED });
}

Texture2D* PlayerAnimator::GetPtrToCurrentTexture()
{
	if (ptrToCurrentAnim != nullptr && !ptrToCurrentAnim->frames.empty()) {
		size_t idx = ptrToCurrentAnim->currentFrame;
		return &ptrToCurrentAnim->frames[idx];
	}
	return nullptr;
}

bool PlayerAnimator::IsPlaying(const std::string& animName) const
{
	if (ptrToCurrentAnim == nullptr) return false;
	return ptrToCurrentAnim->name == animName;
}

std::string PlayerAnimator::GetCurrentAnimName() const
{
	return (ptrToCurrentAnim != nullptr) ? ptrToCurrentAnim->name : "NONE";
}

size_t PlayerAnimator::GetCurrentFrame() const
{
	// sentinel value incicates no current anim
	if (ptrToCurrentAnim == nullptr) return -1;
	return ptrToCurrentAnim->currentFrame;
}

void PlayerAnimator::AddListener(std::function<void(const AnimEvent&)> cBack)
{
	listeners.push_back(cBack);
}

void PlayerAnimator::Dispatch(const AnimEvent& animEvent)
{
	for (auto& cBack : listeners) {
		if (cBack) cBack(animEvent);
	}
}

Animation* PlayerAnimator::GetPtrToAnimation(const std::string animName)
{
	auto iterator = animIndexMap.find(animName);
	if (iterator != animIndexMap.end()) {
		return &animations[iterator->second];
	}
}
