#include "Ship.h"

extern bool gameover;
extern float deltaTime;

Ship::Ship(float x, float y, float radius)
{
	picture.setFillColor(sf::Color::White);
	setSize(radius);
	setPosition(x,y);
	name = "ship";

}

void Ship::setSize(float radius)
{
	picture.setRadius(radius);
	picture.setOrigin(radius, radius);
}

void Ship::setPosition(float x, float y)
{
	picture.setPosition(sf::Vector2f(x, y));
}

void Ship::changeSpeed(Animation &animStand, Animation &animGo)
{
	if (!gameover)
	{
		if (moveleft && speedX > -500.f)
		{
			speedX -= acceleration*deltaTime;
			anim = animGo;
		}

		if (moveright&& speedX < 500.f)
		{
			speedX += acceleration*deltaTime;
			anim = animGo;
		}

		if (moveup && speedY > -500.f)
		{

			speedY -= acceleration*deltaTime;
			anim = animGo;
		}

		if (movedown && speedY < 500.f)
		{
			speedY += acceleration*deltaTime;
			anim = animGo;
		}


		if (speedX > 0.5 && !moveleft && !moveright)
		{
			speedX -= damping*deltaTime;
		}
		else if (speedX < -0.5 && !moveleft && !moveright)
		{
			speedX += damping*deltaTime;
		}
		else if (!moveleft && !moveright)
		{
			speedX = 0;
		}

		if (speedY > 0.5 && !moveup && !movedown)
		{
			speedY -= damping*deltaTime;
		}
		else if (speedY < -0.5 && !moveup && !movedown)
		{
			speedY += damping*deltaTime;
		}
		else if (!moveup && !movedown)
		{
			speedY = 0;
		}

		if (!moveup && !movedown && !moveleft && !moveright)
		{
			anim = animStand;
		}
	}


}
