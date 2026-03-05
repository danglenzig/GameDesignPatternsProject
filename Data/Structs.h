#pragma once
#include<iostream>
#include "raylib.h"

enum EnumAggro
{
	LOW,
	MEDIUM,
	HIGH
};


struct EnemyConfig
{
	std::string configName;
	float speed;
	Vector2 size;
	EnumAggro aggro;

	//...more coming
};