#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <fstream>
#include <thread>
#include <forward_list>

#include "GUIElement.h"
#include "Slider.h"
#include "PrecisionSlider.h"
#include "TextDisplayer.h"
#include "TextField.h"
#include "Button.h"
#include "StateButton.h"

#include "MathematicalParser.h"

MathematicalParser ParserLeft, ParserRight, PParser;
sf::Image Result;
int Resolution;
float RenderArea;
sf::VideoMode Pulpit = sf::VideoMode::getDesktopMode();
sf::Vector2i CenterPosition;
sf::RenderWindow Window;
sf::RectangleShape LoadingLine;
bool isComputed = false;

double getLeft(double x, double y)
{
	ParserLeft.update(x, y, PParser.getValue());
	return ParserLeft.getValue();
}

double getRight(double x, double y)
{
	ParserRight.update(x, y, PParser.getValue());
	return ParserRight.getValue();
}

struct InequalityComponents
{
	InequalityComponents(std::shared_ptr<TextField> Left, std::shared_ptr<TextField> Right, std::shared_ptr<TextField> pField, std::shared_ptr<StateButton> gOperator)
	{
		LeftSide = Left;
		RightSide = Right;
		P = pField;
		Operator = gOperator;
	}

	std::shared_ptr<TextField> LeftSide;
	std::shared_ptr<TextField> RightSide;
	std::shared_ptr<TextField> P;
	std::shared_ptr<StateButton> Operator;
};

void compute(InequalityComponents Inequality, std::shared_ptr<PrecisionSlider> EpsilonSlider)
{
	PParser.setCalculation(Inequality.P->getValueString());
	ParserLeft.setCalculation(Inequality.LeftSide->getValueString());
	ParserRight.setCalculation(Inequality.RightSide->getValueString());

	PParser.parse();

	if (!ParserLeft.parse())
	{
		Inequality.LeftSide->showError();
	}
	else if(!ParserRight.parse())
	{
		Inequality.RightSide->showError();
	}
	else if (!PParser.parse())
	{
		Inequality.P->showError();
	}
	else
	{
		float epsilon = 1.0f / EpsilonSlider->getValue();

		for (size_t StepX = 0; StepX < Resolution; StepX++)
		{
			LoadingLine.setSize(sf::Vector2f(2, Pulpit.height / (float)Resolution * StepX));
			LoadingLine.setPosition(Pulpit.width - Pulpit.height - 2, 0);

			for (size_t StepY = 0; StepY < Resolution; StepY++)
			{
				double x = CenterPosition.x - RenderArea + (double)StepX*RenderArea * 2 / (double)Resolution;
				double y = ((0 - CenterPosition.y) - RenderArea + (double)StepY*RenderArea * 2 / (double)Resolution)*(-1);
				PParser.update(x, y, 0);

				if (Inequality.Operator->getState() == '<')
				{
					if (getLeft(x, y) < getRight(x, y))
					{
						Result.setPixel(StepX, StepY, sf::Color::Black);
					}
				}
				else if (Inequality.Operator->getState() == '>')
				{
					if (getLeft(x, y) > getRight(x, y))
					{
						Result.setPixel(StepX, StepY, sf::Color::Black);
					}
				}
				else if (Inequality.Operator->getState() == '=')
				{
					if (getLeft(x, y) > getRight(x, y) - epsilon&&getLeft(x, y) < getRight(x, y) + epsilon)
					{
						Result.setPixel(StepX, StepY, sf::Color::Black);
					}
				}
			}
		}

		LoadingLine.setSize(sf::Vector2f(2, 0));
		isComputed = true;
	}
}

void saveImage(InequalityComponents Inequality)
{
	std::fstream config("config.cfg");
	int ordinal = 0;

	if (config.is_open())
	{
		config >> ordinal;
		config.close();
		config.open("config.cfg", std::ofstream::trunc | std::ofstream::out );
		config << ordinal + 1;
	}
	else
	{
		config.open("config.cfg", std::ofstream::trunc | std::ofstream::out );
		config << 1;
	}
	config.close();

	Result.saveToFile(std::to_string(ordinal)+".png");

	std::ofstream txt(std::to_string(ordinal)+".txt");

	if (txt.is_open())
	{
		txt << Inequality.LeftSide->getValueString() <<"\n";
		txt << Inequality.Operator->getState() << "\n";
		txt << Inequality.RightSide->getValueString() << "\n";
		txt << Inequality.P->getValueString() << "\n";
		txt.close();
	}
}

int main(int argc, char** argv)
{
	std::string
		left = "abs(sin((P*(pow(x,2)+pow(y,2)))/16)+sin((P*(x+(2*y)))/4)+sin((P*((2*x)-y))/4))",
		right = "0.2",
		pString = "pow((10/9),(pow(x,2)+pow(y,2))/16)";

	if (argc == 2)
	{
		std::string filePath = argv[1];
		char trash;

		std::ifstream file(filePath);
		if (file.is_open())
		{
			file >> left;
			file >> trash;
			file >> right;
			file >> trash;
			file >> pString;
		}
	}

	std::vector<std::shared_ptr<GUIElement>> GUI;
	Window.create(sf::VideoMode(Pulpit.width, Pulpit.height), "Graph Generator", sf::Style::Fullscreen);

	sf::FloatRect GraphDimensions(Pulpit.width - Pulpit.height, 0, Pulpit.height, Pulpit.height);
	sf::FloatRect SettingsDimensions(0, 0, Pulpit.width - Pulpit.height - 2, Pulpit.height);
	
	sf::RectangleShape Line;
	Line.setFillColor(sf::Color::Black);
	Line.setPosition(SettingsDimensions.width, 0);
	Line.setSize(sf::Vector2f(2, SettingsDimensions.height));

	LoadingLine.setFillColor(sf::Color::White);
	LoadingLine.setPosition(SettingsDimensions.width, 0);
	LoadingLine.setSize(sf::Vector2f(2, 0));

	sf::Texture ResultTexture;
	sf::Sprite ResultSprite;
	int margin = 20, unit = SettingsDimensions.height / 40;
	
	GUI.push_back(std::shared_ptr<GUIElement>(new TextDisplayer(sf::IntRect(SettingsDimensions.width / 2 + margin, unit, SettingsDimensions.width / 2, unit * 2), "Resolution")));
	std::shared_ptr<Slider> ResolutionSlider(new Slider(500, 5000, 1000, sf::IntRect(margin, unit, SettingsDimensions.width / 2 - margin * 2, unit * 2)));
	GUI.push_back(std::shared_ptr<GUIElement>(ResolutionSlider));

	GUI.push_back(std::shared_ptr<GUIElement>(new TextDisplayer(sf::IntRect(SettingsDimensions.width / 2 + margin, unit * 4, SettingsDimensions.width / 2, unit * 2), "Area of rendering")));
	std::shared_ptr<Slider> RenderAreaSlider(new Slider(5, 25, 15, sf::IntRect(margin, unit * 4, SettingsDimensions.width / 2 - margin * 2, unit * 2)));
	GUI.push_back(std::shared_ptr<GUIElement>(RenderAreaSlider));

	GUI.push_back(std::shared_ptr<GUIElement>(new TextDisplayer(sf::IntRect(SettingsDimensions.width / 2 + margin, unit * 7, SettingsDimensions.width / 2, unit * 2), "Epsilon")));
	std::shared_ptr<PrecisionSlider> EpsilonSlider(new PrecisionSlider(100, 1000000, 100000, sf::IntRect(margin, unit * 7, SettingsDimensions.width / 2 - margin * 2, unit * 2)));
	GUI.push_back(std::shared_ptr<GUIElement>(EpsilonSlider));

	GUI.push_back(std::shared_ptr<GUIElement>(new TextDisplayer(sf::IntRect(SettingsDimensions.width / 2 - margin - unit * 2, unit*9, SettingsDimensions.width/8, unit*2), "X")));
	std::shared_ptr<TextField> CenterX(new TextField(sf::IntRect(SettingsDimensions.width/2-margin - unit * 2, unit * 11, unit * 2, unit * 2), FieldType::Number));
	GUI.push_back(std::shared_ptr<GUIElement>(CenterX));

	GUI.push_back(std::shared_ptr<GUIElement>(new TextDisplayer(sf::IntRect(SettingsDimensions.width/2+margin, unit*9, SettingsDimensions.width / 8, unit*2), "Y")));
	std::shared_ptr<TextField> CenterY(new TextField(sf::IntRect(SettingsDimensions.width / 2 + margin, unit * 11, unit * 2, unit * 2), FieldType::Number));
	GUI.push_back(std::shared_ptr<GUIElement>(CenterY));

	std::shared_ptr<TextField> LeftInequalitySide(new TextField(sf::IntRect(margin, unit*14, SettingsDimensions.width/2-margin*3, unit*7), FieldType::All, 5));
	GUI.push_back(std::shared_ptr<GUIElement>(LeftInequalitySide));
	LeftInequalitySide->setString(left);

	std::shared_ptr<TextField> RightInequalitySide(new TextField(sf::IntRect(SettingsDimensions.width / 2+margin*2, unit*14, SettingsDimensions.width / 2 - margin * 3, unit * 7), FieldType::All, 5));
	GUI.push_back(std::shared_ptr<GUIElement>(RightInequalitySide));
	RightInequalitySide->setString(right);

	std::shared_ptr<StateButton> Operator(new StateButton(sf::IntRect(SettingsDimensions.width / 2 - margin, unit * 17.5f - margin, margin * 2, margin * 2)));
	GUI.push_back(std::shared_ptr<GUIElement>(Operator));

	GUI.push_back(std::shared_ptr<GUIElement>(new TextDisplayer(sf::IntRect(margin, unit * 21, SettingsDimensions.width / 2, unit * 2), "P variable")));
	std::shared_ptr<TextField> P(new TextField(sf::IntRect(margin, unit * 24, SettingsDimensions.width / 2, unit * 2), FieldType::All, 2));
	GUI.push_back(std::shared_ptr<GUIElement>(P));
	P->setString(pString);

	std::shared_ptr<Button> GenerateButton(new Button(sf::IntRect(SettingsDimensions.width/2-SettingsDimensions.width/6, SettingsDimensions.height-unit*6, SettingsDimensions.width/3, unit*4), "Generate"));
	GUI.push_back(std::shared_ptr<GUIElement>(GenerateButton));

	std::shared_ptr<Button> SaveButton(new Button(sf::IntRect(SettingsDimensions.width - unit, SettingsDimensions.height - unit, unit, unit), "S"));
	GUI.push_back(std::shared_ptr<GUIElement>(SaveButton));

	std::thread computing;

	while (Window.isOpen())
	{
		sf::Event Event;
		sf::Mouse mouse;

		while (Window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed || (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Escape))
				Window.close();
			else if (Event.type == sf::Event::TextEntered && Event.text.unicode!=8 && Event.text.unicode != 10 && Event.text.unicode != 13)		//backspace, space, enter
			{
				for (auto &element : GUI)
				{
					if (element->getInformations().updateKeyboard)
					{
						element->updateKeyboard(Event);
					}
				}
			}
			else if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::BackSpace)
			{
				for (auto &element : GUI)
				{
					if (element->getInformations().updateKeyboard)
					{
						element->eraseCharacter();
					}
				}
			}
		}

		if (GenerateButton->clicked())
		{
			Resolution = ResolutionSlider->getValue();
			RenderArea = RenderAreaSlider->getValue();
			CenterPosition.y = CenterY->getValueInt();
			CenterPosition.x = CenterX->getValueInt();
			Result.create(Resolution, Resolution, sf::Color::White);

			isComputed = false;
			computing = std::thread(compute, InequalityComponents(LeftInequalitySide, RightInequalitySide, P, Operator), EpsilonSlider);
			computing.detach();
		}

		if (SaveButton->clicked())
		{
			saveImage(InequalityComponents(LeftInequalitySide, RightInequalitySide, P, Operator));
		}

		if (isComputed)
		{
			ResultTexture.loadFromImage(Result);
			ResultSprite.setTexture(ResultTexture, true);
			ResultSprite.setPosition(GraphDimensions.left, GraphDimensions.top);
			float scale = (float)GraphDimensions.width / Resolution;
			ResultSprite.setScale(scale, scale);
			isComputed = false;
		}
		
		for (auto &element : GUI)
		{
			if (element->getInformations().updateMouse)
			{
				element->updateMouse(mouse);
			}
		}

		Window.clear(sf::Color::White);
		
		Window.draw(Line);
		Window.draw(LoadingLine);
		Window.draw(ResultSprite);
		
		for (auto &element : GUI)
		{
			element->draw(Window);
		}

		Window.display();
	}
	
	return 0;
}