#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
using namespace sf;

class GameObject
{
public:
	sf::CircleShape picture;
	Animation anim;
	std::string name;
	float angle = 0;
	bool checkCollisionWith(GameObject* o)
	{
		return ((picture.getPosition().x - o->picture.getPosition().x)*(picture.getPosition().x - o->picture.getPosition().x) +
			(picture.getPosition().y - o->picture.getPosition().y)*(picture.getPosition().y - o->picture.getPosition().y))<
			((picture.getRadius() + o->picture.getRadius())*(picture.getRadius() + o->picture.getRadius()));
		
	}
};

