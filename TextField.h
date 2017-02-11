#pragma once

#include "GUIElement.h"
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>

enum class FieldType
{
	Number,
	All
};

class TextField : public GUIElement
{
public:
	TextField(sf::IntRect dimensions, FieldType type, int textLines = 1);
	Informations getInformations() { return Informations{ true, true }; }
	void updateMouse(const sf::Mouse& mouse);
	void updateKeyboard(sf::Event& event);
	void eraseCharacter();
	void draw(sf::RenderTarget& target);
	void setString(std::string string) { contentText.setString(string);	actualizeText(); }
	std::string getValueString();
	int getValueInt() { std::string temp = contentText.getString(); return std::atoi(temp.c_str()); }
private:
	bool isTypeMatching(sf::Event& event);
	void actualizeText();
	bool isHovered, isActive;
	int textLines;
	sf::IntRect dimensions;
	sf::RectangleShape box;
	sf::Font arial;
	sf::Text contentText;
	FieldType type;
};

