#pragma once

#include "GUIElement.h"
#include <utility>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>

class Slider : public GUIElement
{
public:
	Slider(float minRange, float maxRange, float defaultValue, sf::IntRect dimensions);
	Informations getInformations() { return Informations{ true, false }; }
	void updateMouse(const sf::Mouse& mouse);
	void draw(sf::RenderTarget& target);
	int getValue() { return value; }
protected:
	virtual void adjustValueToButtonPosition(double x);
	virtual void adjustButtonPositionToValue();
	virtual void actualizeText();
	int value;
	bool isHovered, isGrabbed;
	std::pair<float, float> range;
	sf::IntRect lineDimensions;
	sf::RectangleShape line, button, valueBox;
	sf::Font arial;
	sf::Text valueText;
};

