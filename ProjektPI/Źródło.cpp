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
	//otwarcie pliku score.txt z opcj� odczytu
	scores.open("score.txt", ios::in);
	if (scores.good())
	{
		string out;
		string linia;
		//zapisanie ca�ego pliku do zmiennej linia
		for (int i = 0; i < 10; i++)
		{
			getline(scores, out);
			linia += out + "\n";
		}
		//utworzenie obiektu klasy scorescreen
		scorescreen scoresc(window.getSize().x,window.getSize().y,linia);
		//p�tla wy�wietlaj�ca ekran z najlepszymi wynikami
		while (window.isOpen())
		{
			menu.MoveUp();
			//obs�uga eventow
			sf::Event windowEvent;
			while (window.pollEvent(windowEvent))
			{
				//wyj�cie z gry ESC i zamkni�cie okna
				if (windowEvent.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}
				//obs�uga entera (wyj�cie z tabeli wynik�w)
				if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Enter)
				{
					scores.close();
					return 0;
				}
			}
			//wy�wietlanie 
			window.clear(sf::Color::White);
			//odwo�anie si� do w�a�ciwo�ci klasy scoresc(definicja w�a�ciwo�ci tej klasy w oddzielnym pliku)
			scoresc.draw(window);
			window.display();
		}
	}
}
//

int menu(sf::RenderWindow &window,fstream &scores)
{
	//utworzenie obiektu klasy MENU (w nawiasach s� podane warto�ci dla konstruktora klasy)
	Menu menu(window.getSize().x, window.getSize().y);//zamiast podawa� sta�� warto�� lepiej poda� wzgl�dn� poniewa� teraz wystarczy zmieni� rozmiary okna w jednym miejscu i wszystko si� dostosuje
	while (window.isOpen())
	{
		//Obs�uga zdarze� takich jak zamkni�cie gry, lub naci�ni�cie klawisza
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			//obs�uga wyj�cia z gry przez zamkni�cie okna lub naci�ni�cie ESC
			if(windowEvent.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
			//obs�uga przej�cia w g�r� przy u�yciu klawisza W
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::W)
			{
				menu.MoveUp();
			}
			//obs�uga przej�cia w d� przy u�yciu klawisza S
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::S)
			{
				menu.MoveDown();
			}
			//Akceptacja wyboru na kt�rym jest obecnie u�ytkownik
			if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Enter)
			{
				switch (menu.getPressedItem())
				{
				case 0: // START - wraca do main
					return 0;
					break;
				case 1: // BEST SCORE - wywo�anie funkcji score
					score(window,scores,menu);
					break;
				case 2: // EXIT - zamkni�cie gry
					window.close();
				}
				break;
			}
		}
		//Wy�wietlanie rzeczy na ekran (na ekranie wy�wietlane s� jedynie elementy znajduj�ce si� pomi�dzy clear i display)
		window.clear(sf::Color::White);
		//odwo�anie si� do w�a�ciwo�ci klasy menu (definicja klasy i w�a�ciwo�ci w oddzielnym pliku)
		menu.draw(window); 
		window.display();
	}
}

int jump(sf::Sprite &dino)
{
	return 0;
}

int main()
{
	//Utworzenie okna gry
	sf::RenderWindow window{ sf::VideoMode(800, 600, 32), "Dinozaur" };
	//utworzenie zmiennej do otwierania plik�w tekstowych?xD(nw jak to si� nazywa profesjonalnie)
	fstream scores;
	//uruchomienie funkcji menu wy�wietlaj�cej menu gry
	menu(window,scores);
	//utworzenie sprite dinozaura oraz utworzenie tekstury i za�adowanie jej z pliku (�adowanie tekstury z obs�ug� b��du)
	sf::Texture dino_texture;
	if (!dino_texture.loadFromFile("./textures/almighty_dragon.png"))
		return EXIT_FAILURE;
	sf::Sprite dino(dino_texture);
	dino.setTextureRect({ 0,0,55,60 });
	dino.setPosition(window.getSize().x / 10, 441);
	//t�o gry
	sf::Texture backgroundtxt1;
	backgroundtxt1.loadFromFile("./textures/background1.jpg");
	sf::Texture backgroundtxt2;
	backgroundtxt2.loadFromFile("./textures/background2.jpg");
	sf::Sprite backgroundSprite(backgroundtxt1);
	//zegar animacji
	sf::Clock t0;
	//iterator animacji
	int animation = 0;
	//g��wna p�tla programu
	while (window.isOpen())
	{
		//animacja biegn�cego dinozaura oraz zmiana t�a gry
		if (t0.getElapsedTime().asSeconds() >= 0.1f)
		{
			dino.setTextureRect({ 55*animation,0,55,60 });
			(animation % 2) ? backgroundSprite.setTexture(backgroundtxt2) : backgroundSprite.setTexture(backgroundtxt1);
			animation++;
			t0.restart();
			if (animation == 4)
				animation = 0;
		}
		//obs�uga event�w
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
		window.draw(backgroundSprite);
		window.draw(dino);
		window.display();
	}
}
