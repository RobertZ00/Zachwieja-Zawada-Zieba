#include "scorescreen.h"

scorescreen::scorescreen(int width, int height,std::string line)
{
	if (!font.loadFromFile("./fonts/Texturina-VariableFont_opsz,wght.ttf"))
	{
		// error
	}

	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Black);
	menu[0].setString(line);
	menu[0].setPosition(sf::Vector2f(width / 8, height / (MAX_NUMBER_ITEMS + 11) * 1));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::Cyan);
	menu[1].setString("BACK");
	menu[1].setPosition(sf::Vector2f(width / 8, height / (MAX_NUMBER_ITEMS + 12) * 13));

}

void scorescreen::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_NUMBER_ITEMS; i++)
	{
		window.draw(menu[i]);
	}
}
