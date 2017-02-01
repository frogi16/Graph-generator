#pragma once
#include "SFML\Graphics\RectangleShape.hpp"
#include "SFML\Graphics\RenderWindow.hpp"
#include "Box2D\Box2D.h"

class Box
{
public:
	Box(b2World* dSwiat, int METR, int width, int height, float x, float y);
	void Update();
	void Draw(sf::RenderWindow* Okno);
	~Box();
	b2World* Swiat;
	int METR_NA_PIKSELE;
	sf::RectangleShape Kwadrat;
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape dynamicBox;
	b2FixtureDef fixtureDef;
};

