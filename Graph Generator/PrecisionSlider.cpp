#include "PrecisionSlider.h"

#include <cmath>

PrecisionSlider::PrecisionSlider(int minRange, int maxRange, int defaultValue, sf::IntRect dimensions):
	Slider(static_cast<float>(minRange), static_cast<float>(maxRange), static_cast<float>(defaultValue), dimensions)
{
	steps = log10(maxRange / minRange);
	actualizeText();
	adjustButtonPositionToValue();
}


PrecisionSlider::~PrecisionSlider()
{
}

void PrecisionSlider::adjustValueToButtonPosition(double x)
{
	int iterations = (x - lineDimensions.left) / (lineDimensions.width/steps);
	value = static_cast<int>(range.first);

	for (size_t i = 0; i < iterations; i++)
	{
		value *= 10;
	}

	actualizeText();
}

void PrecisionSlider::adjustButtonPositionToValue()
{
	int newSteps = log10(value / range.first);

	button.setPosition(static_cast<float>(lineDimensions.left+lineDimensions.width/steps*newSteps), button.getPosition().y);
	actualizeText();
}

void PrecisionSlider::actualizeText()
{
	valueText.setString(std::string(std::to_string((float)1/value)));

	unsigned int charSize = valueText.getCharacterSize();
	while (valueText.getLocalBounds().width > valueBox.getLocalBounds().width)
	{
		charSize *= 0.9f;
		valueText.setCharacterSize(static_cast<int>(charSize));
	}

	valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);	//center the origin point
	valueText.setPosition(valueBox.getPosition().x + valueBox.getSize().x / 2, valueBox.getPosition().y + valueBox.getSize().y / 2);	//set the text's origin's position exactly as box's origin's position
}
