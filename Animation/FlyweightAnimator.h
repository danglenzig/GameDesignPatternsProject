#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

class FlyweightAnimator
{
private:
	std::vector<std::string> skeeterPaths = {
		"Resources/Textures/SKEETER_A00",
		"Resources/Textures/SKEETER_A01",
	};
	std::vector<Texture2D> skeeterTextures;

public:
	FlyweightAnimator();
	Texture2D* GetPtrToTexture(const std::string& configName, const size_t& idx);
};

FlyweightAnimator::FlyweightAnimator()
{
	for (const auto& path : skeeterPaths) {
		Texture2D tex = LoadTexture(path.c_str());
		if (tex.id == 0) {
			std::cerr << "Failed to load texture: " << path << "\n";
		}
		else {
			skeeterTextures.push_back(tex);
		}
	}
}
Texture2D* FlyweightAnimator::GetPtrToTexture(const std::string& configName, const size_t& idx)
{
	if (configName == "SKEETER") {
		return &skeeterTextures[idx];
	}

	return nullptr;
}