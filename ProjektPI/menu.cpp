#include "Menu.h"


Menu::Menu(int width, int height)
{
	if (!font.loadFromFile("./fonts/Texturina-VariableFont_opsz,wght.ttf"))
	{
		// error
	}

	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Cyan);
	menu[0].setString("PLAY");
	menu[0].setPosition(sf::Vector2f(width / 8, height / (MAX_NUMBER_ITEMS + 1) * 1));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::Black);
	menu[1].setString("BEST SCORE");
	menu[1].setPosition(sf::Vector2f(width / 8, height / (MAX_NUMBER_ITEMS + 1) * 2));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::Black);
	menu[2].setString("EXIT");
	menu[2].setPosition(sf::Vector2f(width / 8, height / (MAX_NUMBER_ITEMS + 1) * 3));

	selectedItemIndex = 0;
}

void Menu::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < 3;i++)
	{
		window.draw(menu[i]);
	}
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::Black);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Cyan);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_NUMBER_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::Black);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Cyan);
	}
}
