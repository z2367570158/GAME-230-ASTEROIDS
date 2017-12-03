#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include <vector>
#include <math.h>

class Bullet : public GameObject
{
public:
	float speed = 1000.f;
	Bullet(float x, float y, float radius);
	void setSize(float radius);
	void setPosition(float x, float y);
};

