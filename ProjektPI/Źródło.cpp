#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "menu.h"

using namespace std;

void menu(sf::RenderWindow &window)
{
	Menu menu(window.getSize().x, window.getSize().y);
	while (window.isOpen())
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if(windowEvent.type == sf::Event::Closed || (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape))
			{
				window.close();
			}
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::W)
			{
				menu.MoveUp();
			}
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::S)
			{
				menu.MoveDown();
			}
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Enter)
			{
				switch (menu.getPressedItem())
				{
				case 0:
					break;
				case 1:
					break;
				case 2:
					window.close();
				}
				break;
			}
		}
		window.clear(sf::Color::White);
		menu.draw(window);
		window.display();
	}
}

int main()
{
	sf::RenderWindow window{ sf::VideoMode(800, 600, 32), "Dinozaur" };
	menu(window);
	while (window.isOpen())
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::Closed)
			{
				window.close();
			}
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}
		window.clear(sf::Color::White);

		window.display();
	}
}