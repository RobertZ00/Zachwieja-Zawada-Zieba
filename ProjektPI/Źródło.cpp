#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "menu.h"
#include "scorescreen.h"
#include <fstream>

using namespace std;

int score(sf::RenderWindow& window, fstream& scores,Menu& menu)
{
	//otwarcie pliku score.txt z opcj¹ odczytu
	scores.open("score.txt", ios::in);
	if (scores.good())
	{
		string out;
		string linia;
		//zapisanie ca³ego pliku do zmiennej linia
		for (int i = 0; i < 10; i++)
		{
			getline(scores, out);
			linia += out + "\n";
		}
		//utworzenie obiektu klasy scorescreen
		scorescreen scoresc(window.getSize().x,window.getSize().y,linia);
		//pêtla wyœwietlaj¹ca ekran z najlepszymi wynikami
		while (window.isOpen())
		{
			menu.MoveUp();
			//obs³uga eventow
			sf::Event windowEvent;
			while (window.pollEvent(windowEvent))
			{
				//wyjœcie z gry ESC i zamkniêcie okna
				if (windowEvent.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}
				//obs³uga entera (wyjœcie z tabeli wyników)
				if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Enter)
				{
					scores.close();
					return 0;
				}
			}
			//wyœwietlanie 
			window.clear(sf::Color::White);
			//odwo³anie siê do w³aœciwoœci klasy scoresc(definicja w³aœciwoœci tej klasy w oddzielnym pliku)
			scoresc.draw(window);
			window.display();
		}
	}
}

int menu(sf::RenderWindow &window,fstream &scores)
{
	//utworzenie obiektu klasy MENU (w nawiasach s¹ podane wartoœci dla konstruktora klasy)
	Menu menu(window.getSize().x, window.getSize().y);//zamiast podawaæ sta³¹ wartoœæ lepiej podaæ wzglêdn¹ poniewa¿ teraz wystarczy zmieniæ rozmiary okna w jednym miejscu i wszystko siê dostosuje
	while (window.isOpen())
	{
		//Obs³uga zdarzeñ takich jak zamkniêcie gry, lub naciœniêcie klawisza
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			//obs³uga wyjœcia z gry przez zamkniêcie okna lub naciœniêcie ESC
			if(windowEvent.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
			//obs³uga przejœcia w górê przy u¿yciu klawisza W
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::W)
			{
				menu.MoveUp();
			}
			//obs³uga przejœcia w dó³ przy u¿yciu klawisza S
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::S)
			{
				menu.MoveDown();
			}
			//Akceptacja wyboru na którym jest obecnie u¿ytkownik
			if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Enter)
			{
				switch (menu.getPressedItem())
				{
				case 0: // START - wraca do main
					return 0;
					break;
				case 1: // BEST SCORE - wywo³anie funkcji score
					score(window,scores,menu);
					break;
				case 2: // EXIT - zamkniêcie gry
					window.close();
				}
				break;
			}
		}
		//Wyœwietlanie rzeczy na ekran (na ekranie wyœwietlane s¹ jedynie elementy znajduj¹ce siê pomiêdzy clear i display)
		window.clear(sf::Color::White);
		//odwo³anie siê do w³aœciwoœci klasy menu (definicja klasy i w³aœciwoœci w oddzielnym pliku)
		menu.draw(window); 
		window.display();
	}
}

int main()
{
	//Utworzenie okna gry
	sf::RenderWindow window{ sf::VideoMode(800, 600, 32), "Dinozaur" };
	//utworzenie zmiennej do otwierania plików tekstowych?xD(nw jak to siê nazywa profesjonalnie)
	fstream scores;
	//uruchomienie funkcji menu wyœwietlaj¹cej menu gry
	menu(window,scores);
	//utworzenie sprite dinozaura oraz utworzenie teksturyi za³adowanie jej z pliku (³adowanie tekstury z obs³ug¹ b³êdu)
	sf::Texture dino_texture;
	if (!dino_texture.loadFromFile("./textures/almighty_dragon.png"))
		return EXIT_FAILURE;
	sf::Sprite dino(dino_texture);

	dino.setTextureRect({ 0,0,55,60 });

	sf::Clock t0;
	int animation = 0;
	//g³ówna pêtla programu
	while (window.isOpen())
	{
		//animacja biegn¹cego dinozaura
		if (t0.getElapsedTime().asSeconds() >= 0.1f)
		{
			dino.setTextureRect({ 55*animation,0,55,60 });
			animation++;
			t0.restart();
			if (animation == 4)
				animation = 0;
		}
		//obs³uga eventów
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if (windowEvent.type == sf::Event::Closed)
			{
				window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
		}
		window.clear(sf::Color::White);
		window.draw(dino);
		window.display();
	}
}
