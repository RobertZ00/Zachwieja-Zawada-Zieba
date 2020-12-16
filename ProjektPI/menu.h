#pragma once
#include <SFML/Graphics.hpp>

#define MAX_NUMBER_ITEMS 3

class Menu
{
public:
	Menu(int width, int height);
	void draw(sf::RenderWindow& window);
	void MoveUp();
	void MoveDown();
	int getPressedItem() { return selectedItemIndex; }

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_ITEMS];
};
