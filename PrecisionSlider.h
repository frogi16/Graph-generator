#pragma once
#include "Slider.h"
class PrecisionSlider :
	public Slider
{
public:
	PrecisionSlider(int minRange, int maxRange, int defaultValue, sf::IntRect dimensions);
	~PrecisionSlider();
protected:
	int steps;
	void adjustValueToButtonPosition(double x);
	void adjustButtonPositionToValue();
	void actualizeText();
};

