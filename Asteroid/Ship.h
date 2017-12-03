#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"


class Ship : public GameObject
{
public:
	bool isInvincible = false;
	bool moveup, movedown, moveleft, moveright;

	float invincibleTime = 1.f;
	float speedX = 0.f;
	float speedY = 0.f;
	float damping = 350.f;
	float acceleration = 500.f;
	Ship(float x, float y, float radius);
	void setSize(float radius);
	void setPosition(float x, float y);
	void changeSpeed(Animation &animStand, Animation &animGo);
};
