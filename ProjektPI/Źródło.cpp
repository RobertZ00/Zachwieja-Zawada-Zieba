#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "menu.h"
#include "scorescreen.h"
#include <fstream>

using namespace std;

int score(sf::RenderWindow &window, fstream& scores)
{
	scores.open("score.txt", ios::in);
	if (scores.good())
	{
		string out;
		string linia;
		sf::Font font;
		font.loadFromFile("./fonts/Texturina-VariableFont_opsz,wght.ttf");
		sf::Text score;
		score.setFont(font);
		score.setFillColor(sf::Color::Black);
		for (int i = 0; i < 10; i++)
		{
			getline(scores, out);
			linia += out + "\n";
		}
		scorescreen scoresc(window.getSize().x,window.getSize().y,linia);
		while (window.isOpen())
		{
			sf::Event windowEvent;
			while (window.pollEvent(windowEvent))
			{
				if (windowEvent.type == sf::Event::Closed || (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape))
				{
					window.close();
				}
				if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Enter)
				{
					return 0;
				}
			}
			window.clear(sf::Color::White);
			scoresc.draw(window);
			window.display();
		}
	}
}

int menu(sf::RenderWindow &window,fstream &scores)
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
					return 0;
					break;
				case 1:
					score(window,scores);
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
	fstream scores;
	menu(window,scores);
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