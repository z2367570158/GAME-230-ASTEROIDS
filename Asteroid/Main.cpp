#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <math.h>

#include "Asteroid.h"
#include "Bullet.h"
#include "Ship.h"
#include "GameObject.h"
#include "Animation.h"

#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

using namespace std;
using namespace sf;

void Initiate();
void Reset();
void Update();
void Render();
void HandleInput();
void loadLevel(int level);

Vector2i getBucket(Vector2f pos);
void bucket_add(Vector2i b, Asteroid* obj);
void bucket_remove(Vector2i b, Asteroid* obj);
void detect_collisions(GameObject* obj, Vector2i b);


const float pi = 3.14159f;

RenderWindow window;

Clock gameClock;
Font font;


Text lifeText;
Text scoreText;
Text gameoverText;

float deltaTime;

int frameWidth = 1366;
int frameHeight = 768;

float mouseX = 0.f;
float mouseY = 0.f;

float nextshot = 0.f;
float shotDuration = 0.3f;

int life = 3;
int level = 0;
int score = 0;

bool isPlaying = false;
bool gameover = false;
bool win = false;
bool die = false;

vector<Asteroid*> asteroids;
vector<Bullet*> bullets;
vector<GameObject*> explosions;

Ship ship(frameWidth / 2, frameHeight / 2, 25);

Texture lasserTexture;

Texture t1, t2, t3, t4, t5, t6, t7;


Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);


const float BUCKET_WIDTH = 200;
const float BUCKET_HEIGHT = 200;
const int COLUMNS = 14;
const int ROWS = 8;
vector<Asteroid*> grid[COLUMNS][ROWS];


SoundBuffer shootBuf;
SoundBuffer explosionBuf;
SoundBuffer winBuf;
SoundBuffer loseBuf;
SoundBuffer BGMbuf;
Sound shootSound;
Sound explosionSound;
Sound winSound;
Sound loseSound;
Sound BGMSound;


int main()
{
	srand(time(0));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);

	Initiate();
	window.create(VideoMode(frameWidth, frameHeight), "Asteroid");
	window.setFramerateLimit(60);
	while (window.isOpen())
	{

		deltaTime = gameClock.restart().asSeconds();
		HandleInput();
		if (isPlaying&&!die&&!gameover&&!win)
		{
			Update();

		}
		Render();
	}

	return EXIT_SUCCESS;
}

void Initiate()
{
	font.loadFromFile("consola.ttf");
	t1.loadFromFile("images/spaceship.png");
	t2.loadFromFile("images/background.jpg");
	t3.loadFromFile("images/explosions/type_C.png");
	t4.loadFromFile("images/rock.png");
	t6.loadFromFile("images/rock_small.png");
	t7.loadFromFile("images/explosions/type_B.png");

	t1.setSmooth(true);
	t2.setSmooth(true);


	shootBuf.loadFromFile("shoot.wav");
	explosionBuf.loadFromFile("explosion.wav");
	winBuf.loadFromFile("win.wav");
	loseBuf.loadFromFile("lose.wav");

	shootSound.setBuffer(shootBuf);
	explosionSound.setBuffer(explosionBuf);
	winSound.setBuffer(winBuf);
	loseSound.setBuffer(loseBuf);


	BGMbuf.loadFromFile("BGM.flac");
	BGMSound.setBuffer(BGMbuf);
	BGMSound.setLoop(true);
	BGMSound.play();
	Sprite background(t2);

	lasserTexture.loadFromFile("lasser.png");

	ship.anim = sPlayer;

	lifeText.setFont(font);
	lifeText.setCharacterSize(20);
	lifeText.setPosition(1220, frameHeight - 30);

	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setPosition(20, frameHeight - 30);

	gameoverText.setFont(font);
	gameoverText.setCharacterSize(60);
	gameoverText.setPosition(260, 250);
	gameoverText.setString("       Asteroids!\n press 'Enter' to start");


}

void Reset()
{

}

void Update()
{
	ship.changeSpeed(sPlayer,sPlayer_go);
	ship.picture.move(ship.speedX*deltaTime, ship.speedY*deltaTime);
	ship.invincibleTime -= deltaTime;
	ship.angle = atan2f(mouseY - ship.picture.getPosition().y, mouseX - ship.picture.getPosition().x)+pi/2;
	ship.picture.setRotation(ship.angle * 180 / pi);

	if (ship.picture.getPosition().x > frameWidth)
		ship.picture.setPosition(0.f, ship.picture.getPosition().y);
	else if (ship.picture.getPosition().x < 0.f)
		ship.picture.setPosition(frameWidth-0.1f, ship.picture.getPosition().y);


	if (ship.picture.getPosition().y > frameHeight)
		ship.picture.setPosition(ship.picture.getPosition().x, 0.1f);
	else if (ship.picture.getPosition().y < 0.f)
		ship.picture.setPosition(ship.picture.getPosition().x, frameHeight - 0.1f);

	detect_collisions(&ship, getBucket(ship.picture.getPosition()));

	nextshot += deltaTime;



	for (int i = 0; i < bullets.size(); ++i)
	{
		bullets[i]->picture.move(std::cos(bullets[i]->angle)*bullets[i]->speed*deltaTime, std::sin(bullets[i]->angle)*bullets[i]->speed*deltaTime);

		if (bullets[i]->picture.getPosition().x > frameWidth || bullets[i]->picture.getPosition().x<0.f || bullets[i]->picture.getPosition().y>frameHeight || bullets[i]->picture.getPosition().y < 0.f)
		{
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
		else
		{
			detect_collisions(bullets[i], getBucket(bullets[i]->picture.getPosition()));
		}
	}



	for (int i = 0; i < asteroids.size(); ++i)
	{
		Vector2i curBucket =getBucket(asteroids[i]->picture.getPosition());

		asteroids[i]->picture.move(std::cos(asteroids[i]->angle)*asteroids[i]->speed*deltaTime, std::sin(asteroids[i]->angle)*asteroids[i]->speed*deltaTime);
		detect_collisions(asteroids[i], getBucket(asteroids[i]->picture.getPosition()));

		if (asteroids[i]->picture.getPosition().x > frameWidth)
			asteroids[i]->picture.setPosition(0.f, asteroids[i]->picture.getPosition().y);
		else if (asteroids[i]->picture.getPosition().x < 0.f)
			asteroids[i]->picture.setPosition(frameWidth - 0.1f, asteroids[i]->picture.getPosition().y);


		if (asteroids[i]->picture.getPosition().y > frameHeight)
			asteroids[i]->picture.setPosition(asteroids[i]->picture.getPosition().x, 0.1f);
		else if (asteroids[i]->picture.getPosition().y < 0.f)
			asteroids[i]->picture.setPosition(asteroids[i]->picture.getPosition().x, frameHeight - 0.1f);

		Vector2i newBucket = getBucket(asteroids[i]->picture.getPosition());
		if (curBucket != newBucket)
		{
			bucket_add(newBucket, asteroids[i]);
			bucket_remove(curBucket, asteroids[i]);
		}

	}

	if (asteroids.size() <= 0)
	{
		win = true;
		winSound.play();
		gameoverText.setString("          you win!\npress 'Enter' to next level");

	}

	lifeText.setString("life:"+to_string(life));
	scoreText.setString("score:"+to_string(score));
}

void Render()
{
	window.clear(sf::Color::Black);
	if (isPlaying)
	{
		ship.anim.update();

		ship.anim.sprite.setPosition(ship.picture.getPosition().x, ship.picture.getPosition().y);
		ship.anim.sprite.setRotation(ship.angle * 180 / pi);
		window.draw(ship.anim.sprite);



		for (int i = 0; i < bullets.size(); ++i)
		{
			window.draw(bullets[i]->picture);
		}
		for (int i = 0; i < asteroids.size(); ++i)
		{
			asteroids[i]->anim.update();
			asteroids[i]->anim.sprite.setPosition(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y);
			asteroids[i]->anim.sprite.setRotation(asteroids[i]->angle * 180 / pi);
			window.draw(asteroids[i]->anim.sprite);
		}
		for (int i = 0; i < explosions.size(); ++i)
		{
			explosions[i]->anim.update();
			if (explosions[i]->anim.isEnd())
			{
				delete explosions[i];
				explosions.erase(explosions.begin() + i);
			}
			else
			{
				explosions[i]->anim.sprite.setPosition(explosions[i]->picture.getPosition().x, explosions[i]->picture.getPosition().y);
				explosions[i]->anim.sprite.setRotation(explosions[i]->angle * 180 / pi);
				window.draw(explosions[i]->anim.sprite);
			}
		}
	}
	else
	{
		gameoverText.setString("       Asteroids!\n press 'Enter' to start");
		lifeText.setString("");
		scoreText.setString("");
	}

	window.draw(lifeText);
	window.draw(gameoverText);
	window.draw(scoreText);
	window.display();
}

void HandleInput()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
			for (int i = 0; i < bullets.size(); i++)
			{
				delete bullets[i];
				bullets[i] = nullptr;
			}

			for (int i = 0; i < asteroids.size(); i++)
			{
				delete asteroids[i];
				asteroids[i] = nullptr;
			}

			for (int i = 0; i < explosions.size(); i++)
			{
				delete explosions[i];
				explosions[i] = nullptr;
			}
			bullets.clear();
			asteroids.clear();
			explosions.clear();
		}
		else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A))
		{
			ship.moveleft = true;
		}
		else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D))
		{
			ship.moveright = true;
		}
		else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W))
		{
			ship.moveup = true;
		}
		else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S))
		{
			ship.movedown = true;
		}

		else if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A))
		{
			ship.moveleft = false;
		}
		else if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D))
		{
			ship.moveright = false;
		}
		else if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W))
		{
			ship.moveup = false;
		}
		else if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S))
		{
			ship.movedown = false;
		}
		else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Return))
		{
			if (die)
			{
				die = false;
				ship.setPosition(frameWidth/2,frameHeight/2);
			}
			
			if (gameover)
			{
				gameover = false;
				isPlaying = false;
				return;
			}
			if (win)
			{
				level++;
				loadLevel(level);
				win = false;
			}

			if (!isPlaying)
			{
				isPlaying = true;
				loadLevel(0);
			}

			gameoverText.setString("");
		}
		else if (event.type == sf::Event::MouseMoved && !gameover && !win)
		{
			if (Mouse::getPosition(window).x<frameWidth && Mouse::getPosition(window).x  > 0.f)
			{
				mouseX = Mouse::getPosition(window).x;
			}

			if (Mouse::getPosition(window).y<frameHeight && Mouse::getPosition(window).y  > 0.f)
			{
				mouseY = Mouse::getPosition(window).y;
			}
		}
		else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape))
		{
			isPlaying = false;
			die = false;
			gameover = false;
			win = false;
		}
	}


	if (isPlaying && !gameover && !win)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (nextshot > shotDuration)
			{
				Bullet* bptr = new Bullet(ship.picture.getPosition().x, ship.picture.getPosition().y, 10);
				bptr->picture.setTexture(&lasserTexture);
				bptr->angle = ship.angle - pi / 2;
				bptr->picture.setRotation((bptr->angle + pi / 2) * 180 / pi);
				bullets.push_back(bptr);

				shootSound.play();

				nextshot = 0.f;
			}
		}
	}

}

void loadLevel(int level)
{
	ship.setPosition(frameWidth/2,frameHeight/2);
	if (!win)
	{
		score = 0;
		life = 3;
		level = 0;
	}
	for (int i = 0; i < asteroids.size(); ++i)
	{
		delete asteroids[i];
		asteroids[i] = nullptr;
	}
	asteroids.clear();

	for (int i = 0; i<15; i++)
	{

		int x, y;
		do
		{
			x = rand() % frameWidth;
			y = rand() % frameHeight;

		} while (x<frameWidth/2+25&&x>frameWidth/2-25&&y<frameHeight+25&&y>frameHeight-25);

		Asteroid* aptr = new Asteroid(x,y, 25);
		aptr->angle = static_cast<float>(rand())/static_cast<float>(RAND_MAX)*2*pi;
		aptr->hp = 3;
		aptr->speed = aptr->speed + level * 50;
		aptr->anim = sRock;
		asteroids.push_back(aptr);
		bucket_add(getBucket(aptr->picture.getPosition()), aptr);
	}
}

Vector2i getBucket(Vector2f pos)
{
	int col = int(pos.x / BUCKET_WIDTH);
	if (col < 0)
		col = 0;
	else if (col >= COLUMNS)
		col = COLUMNS - 1;

	int row = int(pos.y / BUCKET_HEIGHT);
	if (row < 0)
		row = 0;
	else if (row >= ROWS)
		row = ROWS - 1;

	return Vector2i(col, row);
}


void bucket_add(Vector2i b,	Asteroid* obj)
{
	grid[b.x][b.y].push_back(obj);
}

void bucket_remove(Vector2i b, Asteroid* obj)
{
	vector<Asteroid*> & v = grid[b.x][b.y];

	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i] == obj)
		{
			v.erase(v.begin() + i);
			break;
		}
	}
}

void detect_collisions(GameObject* obj, Vector2i b)
{
	int left = max(b.x - 1, 0);
	int right = min(b.x + 1, COLUMNS - 1);
	int top = max(b.y - 1, 0);
	int bot = min(b.y + 1, ROWS - 1);
	for (int i = 0; i < asteroids.size(); i++)
	{
		if (obj->checkCollisionWith(asteroids[i]))
		{
			if (obj->name == "bullet")
			{
				for (int i = 0; i < bullets.size(); i++)
				{
					if (bullets[i] == obj)
					{
						delete obj;
						bullets.erase(bullets.begin() + i);
						break;
					}
				}
				asteroids[i]->hp = asteroids[i]->hp - 1;
				if (asteroids[i]->hp <= 0)
				{

					if (asteroids[i]->picture.getRadius() > 20)
					{
						Asteroid* aptr1 = new Asteroid(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y, 15);
						aptr1->angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * pi;
						aptr1->hp = 1;
						aptr1->anim = sRock_small;
						asteroids.push_back(aptr1);

						Asteroid* aptr2 = new Asteroid(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y, 15);
						aptr2->angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * pi;
						aptr2->hp = 1;
						aptr2->anim = sRock_small;
						asteroids.push_back(aptr2);

					}

					GameObject* explosion = new GameObject;
					explosion->picture.setPosition(asteroids[i]->picture.getPosition().x, asteroids[i]->picture.getPosition().y);
					explosion->anim = sExplosion;
					explosions.push_back(explosion);

					explosionSound.play();

					score += 100;
					delete asteroids[i];
					asteroids.erase(asteroids.begin() + i);
					break;
				}
			}
			else if (obj->name == "asteroid")
			{

				if (obj != asteroids[i])
				{
					float collisionAngle = atan2f(obj->picture.getPosition().y - asteroids[i]->picture.getPosition().y, obj->picture.getPosition().x - asteroids[i]->picture.getPosition().x);
					obj->angle = collisionAngle;
					asteroids[i]->angle = collisionAngle + pi;
				}

			}
			else if (obj->name == "ship")
			{
				if (ship.invincibleTime < 0)
				{
					GameObject* explosion = new GameObject;
					explosion->picture.setPosition(ship.picture.getPosition().x, ship.picture.getPosition().y);
					explosion->anim = sExplosion;
					explosions.push_back(explosion);
					
					loseSound.play();
					die = true;
					ship.invincibleTime = 1.0;
					life--;
					ship.setPosition(-1,-1);

					if (life <= 0)
					{
						gameoverText.setString("	   game over \n press 'Enter' to return");
						gameover = true;
					}
					else
					{
						gameoverText.setString("         you die\n press 'Enter to continue'");
					}
				}
			}

		}
		

	}
			
}