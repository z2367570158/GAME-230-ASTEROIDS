#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	float speed = 50.f;
	int hp;
	Asteroid(float x, float y, float radius);
	void setSize(float radius);
	void setPosition(float x, float y);


};

