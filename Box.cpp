#define _USE_MATH_DEFINES

#include "Box.h"
#include <iostream>
Box::Box(b2World* dSwiat, int METR, int width, int height, float x, float y)
{
	METR_NA_PIKSELE = METR;
	Swiat = dSwiat;
	Kwadrat.setFillColor(sf::Color::Red);
	Kwadrat.setPosition(x, y);
	Kwadrat.setRotation(0);
	Kwadrat.setSize(sf::Vector2f(width * METR_NA_PIKSELE, height * METR_NA_PIKSELE));
	Kwadrat.setOrigin((float)width / 2 * METR_NA_PIKSELE, (float)height / 2 * METR_NA_PIKSELE);
	std::cout << "OK1 ";
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x*1/METR_NA_PIKSELE, y * 1 / METR_NA_PIKSELE);
	std::cout << "OK2 ";
	body = Swiat->CreateBody(&bodyDef);
	std::cout << "OK3 ";
	std::cout << (float)width / 2 << " " << (float)height / 2 << "\n";
	dynamicBox.SetAsBox((float)width/2, (float)height/2);
	std::cout << "OK4 ";
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	std::cout << "OK5 ";
	body->CreateFixture(&fixtureDef);
	std::cout << "OK6 ";
}

void Box::Update()
{
	Kwadrat.setPosition(sf::Vector2f(body->GetPosition().x * METR_NA_PIKSELE, body->GetPosition().y * METR_NA_PIKSELE));
	Kwadrat.setRotation(body->GetAngle() * 180 / M_PI);
}

void Box::Draw(sf::RenderWindow* Okno)
{
	Okno->draw(Kwadrat);
}

Box::~Box()
{
}
