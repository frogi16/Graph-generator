#include "Slider.h"


Slider::Slider(float minRange, float maxRange, float defaultValue, sf::IntRect dimensions) : 
	lineDimensions(dimensions.left, dimensions.top, dimensions.width-(dimensions.height*1.4f)-2, dimensions.height*0.2f),
	isHovered(false),
	isGrabbed(false)
{
	range.first = minRange;
	range.second = maxRange;
	value = defaultValue;

	arial.loadFromFile("arial.ttf");

	button.setOrigin(dimensions.height*0.333, 0);
	button.setSize(sf::Vector2f(dimensions.height*0.6666, dimensions.height));
	button.setFillColor(sf::Color::White);
	button.setOutlineColor(sf::Color::Black);
	button.setOutlineThickness(1);
	button.setPosition(0, dimensions.top);
	adjustButtonPositionToValue();

	line.setPosition(lineDimensions.left, lineDimensions.top+dimensions.height*0.4f);
	line.setSize(sf::Vector2f(lineDimensions.width, lineDimensions.height));
	line.setFillColor(sf::Color::White);
	line.setOutlineColor(sf::Color::Black);
	line.setOutlineThickness(1);

	valueBox.setPosition(dimensions.left+dimensions.width-dimensions.height, dimensions.top);
	valueBox.setSize(sf::Vector2f(dimensions.height, dimensions.height));
	valueBox.setFillColor(sf::Color::White);
	valueBox.setOutlineColor(sf::Color::Black);
	valueBox.setOutlineThickness(1);

	valueText.setColor(sf::Color::Black);
	valueText.setPosition(valueBox.getPosition());
	valueText.setFont(arial);

	float charSize = dimensions.height*0.45f;
	valueText.setCharacterSize(charSize);
	while (valueText.getLocalBounds().width > valueBox.getLocalBounds().width)
	{
		charSize *= 0.9f;
		valueText.setCharacterSize(charSize);
	}

	actualizeText();
}

void Slider::updateMouse(const sf::Mouse & mouse)
{
	isHovered = (button.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition().x, mouse.getPosition().y)));

	if (!(isGrabbed && mouse.isButtonPressed(sf::Mouse::Button::Left))) //you can drag mouse beyond the button if you still hold LPM
	{
		isGrabbed = (isHovered && mouse.isButtonPressed(sf::Mouse::Button::Left));
	}

	if (isGrabbed)
	{
		if (mouse.getPosition().x > lineDimensions.left && mouse.getPosition().x < lineDimensions.left + lineDimensions.width)
		{
			button.setPosition(mouse.getPosition().x, button.getPosition().y);
			adjustValueToButtonPosition(mouse.getPosition().x);
		}
		else if(mouse.getPosition().x<lineDimensions.left)
		{
			value = range.first;
			adjustButtonPositionToValue();
		}
		else if (mouse.getPosition().x>lineDimensions.left+ lineDimensions.width)
		{
			value = range.second;
			adjustButtonPositionToValue();
		}
	}
}

void Slider::draw(sf::RenderTarget & target)
{
	target.draw(line);
	target.draw(button);
	target.draw(valueBox);
	target.draw(valueText);
}

void Slider::adjustValueToButtonPosition(double x)
{
	value = (x - lineDimensions.left) / lineDimensions.width*(range.second - range.first) + range.first;
	actualizeText();
}

void Slider::adjustButtonPositionToValue()
{
	button.setPosition((value - range.first) / (range.second - range.first)*lineDimensions.width + lineDimensions.left, button.getPosition().y);
	actualizeText();
}

void Slider::actualizeText()
{
	valueText.setString(std::to_string(value));
	valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);	//center the origin point
	valueText.setPosition(valueBox.getPosition().x + valueBox.getSize().x / 2, valueBox.getPosition().y + valueBox.getSize().y / 2);	//set the text's origin's position exactly as box's origin's position
}
