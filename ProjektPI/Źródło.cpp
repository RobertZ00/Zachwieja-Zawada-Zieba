#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace std;

int main()
{
	sf::RenderWindow window{ sf::VideoMode(800, 600, 32), "Dinozaur" };

	while (window.isOpen())
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::Closed)
			{
				window.close(); //komentarz RObert Pala
			}
		}
		window.clear(sf::Color::White);
		window.display();
	}
}
//komentarz main sprawdzenie 1 2 3
//komentarz 2