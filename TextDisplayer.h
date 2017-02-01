#pragma once

#include "GUIElement.h"
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Rect.hpp>

class TextDisplayer : public GUIElement
{
public:
	TextDisplayer(sf::IntRect dimensions, std::string string);
	TextDisplayer(sf::Vector2i position, float characterSize, std::string string);
	Informations getInformations() { return Informations{ false, false }; }
	void draw(sf::RenderTarget& target);
private:
	void initialize(float x, float yCenter, std::string string);
	sf::Text text;
	sf::Font arial;
};

