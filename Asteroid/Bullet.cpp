#include "Bullet.h"



Bullet::Bullet(float x, float y, float radius)
{
	picture.setFillColor(sf::Color::White);
	setSize(radius);
	setPosition(x, y);
	name = "bullet";

}

void Bullet::setSize(float radius)
{
	picture.setRadius(radius);
	picture.setOrigin(radius, radius);

}

void Bullet::setPosition(float x, float y)
{
	picture.setPosition(sf::Vector2f(x, y));
}
