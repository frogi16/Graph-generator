#include "PrecisionSlider.h"



PrecisionSlider::PrecisionSlider(int minRange, int maxRange, int defaultValue, sf::IntRect dimensions):
	Slider(minRange, maxRange, defaultValue, dimensions)
{
	float temp = maxRange / minRange;
	steps = 0;

	while (temp>=10)
	{
		temp /= 10;
		steps++;
	}

	actualizeText();
	adjustButtonPositionToValue();
}


PrecisionSlider::~PrecisionSlider()
{
}

void PrecisionSlider::adjustValueToButtonPosition(double x)
{
	int iterations = (x - lineDimensions.left) / (lineDimensions.width/steps);
	value = range.first;

	for (size_t i = 0; i < iterations; i++)
	{
		value *= 10;
	}

	actualizeText();
}

void PrecisionSlider::adjustButtonPositionToValue()
{
	int temp = value / range.first;
	int step = 0;

	while (temp >= 10)
	{
		temp /= 10;
		step++;
	}

	button.setPosition(lineDimensions.left+lineDimensions.width/steps*step, button.getPosition().y);
	actualizeText();
}

void PrecisionSlider::actualizeText()
{
	valueText.setString(std::string(std::to_string((float)1/value)));
	valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);	//center the origin point
	valueText.setPosition(valueBox.getPosition().x + valueBox.getSize().x / 2, valueBox.getPosition().y + valueBox.getSize().y / 2);	//set the text's origin's position exactly as box's origin's position
}
