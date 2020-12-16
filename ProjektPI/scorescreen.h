#pragma once
#include <SFML/Graphics.hpp>

#define MAX_NUMBER_ITEMS 2

class scorescreen
{
public:
	scorescreen(int width, int height,std::string line);
	void draw(sf::RenderWindow& window);

private:
	sf::Font font;
	sf::Text menu[MAX_NUMBER_ITEMS];
};

