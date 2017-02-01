#include "TextDisplayer.h"

TextDisplayer::TextDisplayer(sf::IntRect dimensions, std::string string)
{
	text.setCharacterSize(dimensions.height*0.6f);
	initialize(dimensions.left, dimensions.top+dimensions.height / 2.0f, string);
}

TextDisplayer::TextDisplayer(sf::Vector2i position, float characterSize, std::string string)
{
	text.setCharacterSize(characterSize);
	initialize(position.x, position.y, string);
}

inline void TextDisplayer::initialize(float x, float yCenter, std::string string)
{
	arial.loadFromFile("arial.ttf");
	text.setString(string);
	text.setColor(sf::Color::Black);
	text.setFont(arial);
	text.setPosition(x, yCenter - text.getLocalBounds().height / 2);
}

void TextDisplayer::draw(sf::RenderTarget & target)
{
	target.draw(text);
}