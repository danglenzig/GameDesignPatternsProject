#pragma once
#include "../Data/Structs.h"
#include "GameObject.h"
#include "../Animation/FlyweightAnimator.h"

class EnemyFactory
{
private:
	FlyweightAnimator animator;
public:
	void RenderEnemies();

	EnemyFactory() {

	}

};


void EnemyFactory::RenderEnemies()
{
	//
}