#include "Asteroid.h"



Asteroid::Asteroid(float x, float y, float radius)
{
	picture.setFillColor(sf::Color::White);
	setSize(radius);
	setPosition(x, y);
	name = "asteroid";
}

void Asteroid::setSize(float radius)
{
	picture.setRadius(radius);
	picture.setOrigin(radius, radius);
	
}

void Asteroid::setPosition(float x, float y)
{
	picture.setPosition(sf::Vector2f(x, y));

}

