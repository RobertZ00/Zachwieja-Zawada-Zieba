﻿#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <time.h>
#include <string>
#include "menu.h"
#include "scorescreen.h"
#include "collision.h";

using namespace std;

//Klasa do tworzenia przeszkód na mapie
class obstacle
{
public:
	sf::Sprite obstacleSprite;
	sf::Clock immune;
	//konstruktor klasy, trzeba podac link do tekstury oraz początkową pozycje przeszkody
	obstacle(string& texture_link,int basic_pos)
	{
		obstacleTexture.loadFromFile(texture_link);
		obstacleSprite.setTexture(obstacleTexture);
		obstacleSprite.setPosition(basic_pos, 472);
	}

	//metoda klasy odpowiadająca za przemieszczanie się przeszkody oraz kolizję z dino
	void move(float speed,sf::Sprite& dino,int& health)
	{
		//przemieszczanie się przeszkody
		obstacleSprite.setPosition(obstacleSprite.getPosition().x - speed,obstacleSprite.getPosition().y);

		//ustawienie nowej losowej pozycji po prawej stronie ekranu gdy przeszkoda zniknie z lewej storny ekranu
		if(obstacleSprite.getPosition().x<-120) obstacleSprite.setPosition(rand()%1000 + 800, obstacleSprite.getPosition().y);

		//osługa kolizji, wykona się gdy dwa sprite'y się zetkną i gdy czas nieśmiertelności będzie większy niż 1 sekunda (zabezpieczenie przed setkami wykonań podczas przechodzenia przez przeszkode)
		
		if (Collision::PixelPerfectTest(dino,obstacleSprite))
		{
			if(immune.getElapsedTime().asSeconds() > 1)
			{ 
			health = health - 1;
			immune.restart();
			cout << "dziala" << endl;
			}
		}

		/*if (obstacleSprite.getGlobalBounds().intersects(dino.getGlobalBounds()) && immune.getElapsedTime().asSeconds() > 1)
		{
			health = health - 1;
			immune.restart();
		}
		*/
	}

	//metoda wyświetlania przeszkody na ekranie
	void draw(sf::RenderWindow& window)
	{
		window.draw(obstacleSprite);
	}
private:
	sf::Texture none;
	sf::Texture obstacleTexture;
	
	
};

int score(sf::RenderWindow& window, fstream& scores,Menu& menu)
{
	//otwarcie pliku score.txt z opcją odczytu
	scores.open("score.txt");
	if (scores.good())
	{
		string out;
		string linia;
		//zapisanie całego pliku do zmiennej linia
		for (int i = 0; i < 3; i++)
		{
			getline(scores, out);
			linia += out + "\n";
		}
		//utworzenie obiektu klasy scorescreen
		scorescreen scoresc(window.getSize().x,window.getSize().y,linia);
		//pętla wyświetlająca ekran z najlepszymi wynikami
		while (window.isOpen())
		{
			menu.MoveUp();
			//obsługa eventow
			sf::Event windowEvent;
			while (window.pollEvent(windowEvent))
			{
				//wyjście z gry ESC i zamknięcie okna
				if (windowEvent.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}
				//obsługa entera (wyjście z tabeli wyników)
				if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Enter)
				{
					scores.close();
					return 0;
				}
			}
			//wyświetlanie 
			window.clear(sf::Color::White);
			//odwołanie się do właściwości klasy scoresc(definicja właściwości tej klasy w oddzielnym pliku)
			scoresc.draw(window);
			window.display();
		}
	}
}

int menu(sf::RenderWindow &window,fstream &scores)
{
	//utworzenie obiektu klasy MENU (w nawiasach są podane wartości dla konstruktora klasy)
	Menu menu(window.getSize().x, window.getSize().y);//zamiast podawać stałą wartość lepiej podać względną ponieważ teraz wystarczy zmienić rozmiary okna w jednym miejscu i wszystko się dostosuje
	while (window.isOpen())
	{
		//Obsługa zdarzeń takich jak zamknięcie gry, lub naciśnięcie klawisza
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			//obsługa wyjścia z gry przez zamknięcie okna lub naciśnięcie ESC
			if(windowEvent.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
			//obsługa przejścia w górę przy użyciu klawisza W
			if (windowEvent.type == sf::Event::KeyPressed && (windowEvent.key.code == sf::Keyboard::Up || windowEvent.key.code == sf::Keyboard::W))
			{
				menu.MoveUp();
			}
			//obsługa przejścia w dół przy użyciu klawisza S
			if (windowEvent.type == sf::Event::KeyPressed && (windowEvent.key.code == sf::Keyboard::Down || windowEvent.key.code == sf::Keyboard::S))
			{
				menu.MoveDown();
			}
			//Akceptacja wyboru na którym jest obecnie użytkownik
			if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Enter)
			{
				switch (menu.getPressedItem())
				{
				case 0: // START - wraca do main
					return 0;
					break;
				case 1: // BEST SCORE - wywołanie funkcji score
					score(window,scores,menu);
					break;
				case 2: // EXIT - zamknięcie gry
					window.close();
				}
				break;
			}
		}
		//Wyświetlanie rzeczy na ekran (na ekranie wyświetlane są jedynie elementy znajdujące się pomiędzy clear i display)
		window.clear(sf::Color::White);
		//odwołanie się do właściwości klasy menu (definicja klasy i właściwości w oddzielnym pliku)
		menu.draw(window); 
		window.display();
	}
}


//funkcja skakania
int jump(sf::Sprite &dino, bool& is_jump, bool& on_ground, sf::RenderWindow& window)
{

	//predkosc opadania i wznoszenia
	double gravity = 0.45;

	//jesli prawda, ze skok to dino porusza sie do gory
	if (is_jump)
	{
		dino.setPosition(dino.getPosition().x, dino.getPosition().y - gravity);
	}

	//poruszanie sie w dol w innych przypadkach niz prawda ze skok
	else if(dino.getPosition().y != 500)
	{
		dino.setPosition(dino.getPosition().x, dino.getPosition().y+gravity);
	}
	//sprawdzanie czy dino nie spada
	if (dino.getPosition().y >= 500)
	{
		dino.setPosition(dino.getPosition().x, 500);
		on_ground = true;
	}
	//sprawdzanie czy jest aktualnie w locie
	if (dino.getPosition().y < 500)
	{
		on_ground = false;
	}
	//limit wysokosci dla skoku
	if (dino.getPosition().y <= 380)
	{
		is_jump = false;
	}
	//std::cout << dino.getPosition().y << " " << is_jump << " " << on_ground << std::endl;

	return 0;
}


int main()
{
	srand(time(0));
	//Utworzenie okna gry
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window{ sf::VideoMode(800, 600, desktopMode.bitsPerPixel), "Dinozaur" };

	//!dzięki temu sf::Event::KeyPressed będzie wykonywało się tylko raz na wciśnięcie klawisza!
	window.setKeyRepeatEnabled(false);

	//utworzenie uchwytu do otwarcia pliku
	fstream scores;

	//uruchomienie funkcji menu wyświetlającej menu gry
	menuidentifier:
	menu(window,scores);

	//utworzenie sprite dinozaura oraz utworzenie tekstury i załadowanie jej z pliku (ładowanie tekstury z obsługą błędu)
	sf::Texture dino_texture;
	if (!dino_texture.loadFromFile("./textures/almighty_dragon.png"))
		return EXIT_FAILURE;
	//sprawia, że tekstury są wygładzone? nw czy to coś daje ale zawsze spoko
	dino_texture.setSmooth(true);
	sf::Sprite dino(dino_texture);
	//Origin - ustawienie punktu centralnego sprite'a
	dino.setOrigin(22.5f, 58.5f);
		dino.setTextureRect({ 0,0,55,60 });
		dino.setPosition(window.getSize().x / 10, 500);


	//tło gry
	sf::Texture backgroundtxt1;
		backgroundtxt1.loadFromFile("./textures/background1.jpg");
		backgroundtxt1.setSmooth(true);
	sf::Texture backgroundtxt2;
		backgroundtxt2.loadFromFile("./textures/background2.jpg");
		backgroundtxt2.setSmooth(true);
	sf::Sprite backgroundSprite(backgroundtxt1);

	//zegar animacji i naliczania punktów
	sf::Clock t0;
	sf::Clock score_clock;
	sf::Clock animation_time;

	//iterator animacji
	int animation = 0;
	int dino_animation = 0;

	int rectPointer = 55;
	int rectWidth = 55;
	int rectHeight = 60;

	//utworzenie przeszkody
	string bushtxt= "./textures/bush.png";
	string double_bush_txt = "./textures/double_bush.png";
	string ptaszortxt = "./textures/ptaszor.png";

	//pojedynczy krzak
	obstacle bush(bushtxt, 750);

	//obstacle bush2(bushtxt, 800);
	//obstacle bush3(bushtxt, 650);

	//podwojny krzak
	obstacle bush4(double_bush_txt, 820);

	//ptaszor
	obstacle ptaszor(ptaszortxt, 830);
	ptaszor.obstacleSprite.setPosition(820, 410);

	

	//użyte przy obsłudze skakania
	bool is_jump = false;
	bool on_ground = true;
	bool is_bending = false;

	//predkosc poruszania sie przeszkod
	float speed = 0.7f;

	//�ycia dinozaura
	sf::Text healthText;
	sf::Font healthFont;
	int health = 3;
	if (!healthFont.loadFromFile("./fonts/Texturina-VariableFont_opsz,wght.ttf"))
	{
		// error!
	}
	healthText.setFont(healthFont);
	healthText.setString(to_string(health));
	healthText.setFillColor(sf::Color::Red);
	healthText.setPosition( window.getSize().x - healthText.getLocalBounds().width , 0 );

	//wynik
	sf::Text scoreText;
	sf::Font scoreFont;
	int player_score = 0;
	if (!scoreFont.loadFromFile("./fonts/Texturina-VariableFont_opsz,wght.ttf"))
	{
		// error!
	}
	scoreText.setFont(scoreFont);
	scoreText.setString(to_string(player_score));
	scoreText.setPosition(10, 10);
	scoreText.setFillColor(sf::Color::Green);

	//wykorzystane do zapisywania wyniku do tabeli
	scores.open("score.txt");
	int topScores[3] = { 0,0,0 };
	for (int i = 0; i < 3; i++)
	{
		string temp;
		getline(scores, temp);
		topScores[i] = stoi("2");
	}

	//główna pętla programu
	while (window.isOpen())
	{

		//animacja biegnącego dinozaura oraz zmiana tła gry
		if (animation_time.getElapsedTime().asSeconds() >= 0.1f)
		{
			dino.setTextureRect({ rectPointer * dino_animation,0,rectWidth,rectHeight });
			dino_animation++;
			if (dino_animation == 4)
				dino_animation = 0;
			animation_time.restart();
		}
		if (t0.getElapsedTime().asSeconds() >= 0.1f)
		{
			(animation % 2) ? backgroundSprite.setTexture(backgroundtxt2) : backgroundSprite.setTexture(backgroundtxt1);
			animation++;
			t0.restart();
			if (animation == 4)
				animation = 0;
		}

		//obsługa eventów
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
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && on_ground && !is_jump)
			{
				is_jump = true;
				on_ground = false;
			}
			if (windowEvent.type == sf::Event::KeyReleased && (windowEvent.key.code == sf::Keyboard::Enter) && health <= 0)
			{
				goto menuidentifier;
				break;
			}

			//schylanie
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && on_ground)
			{
				is_bending = true;
				if (!dino_texture.loadFromFile("./textures/almighty_dragon_bending.png"))
					return EXIT_FAILURE;
				dino.setOrigin(22.5f, 49.0f);
				rectPointer = 78;
				rectWidth = 78;
				rectHeight = 49;
				dino.setTextureRect({ 78 * dino_animation,0,78,49 });				
			}
			if (windowEvent.type == sf::Event::KeyReleased && (windowEvent.key.code == sf::Keyboard::S || windowEvent.key.code == sf::Keyboard::Down))
			{
				is_bending = false;
				if (!dino_texture.loadFromFile("./textures/almighty_dragon.png"))
					return EXIT_FAILURE;
				dino.setOrigin(22.5f, 58.5f);
				rectPointer = 55;
				rectWidth = 55;
				rectHeight = 60;
				dino.setTextureRect({ 55 * dino_animation,0,55,60 });
			}
		}
		jump(dino, is_jump, on_ground,window);

		//nowa przeszkoda pojawi się jesli poprzednia zniknie
		if (!((bush.obstacleSprite.getPosition().x > -44) && (bush.obstacleSprite.getPosition().x < 800)) && !((ptaszor.obstacleSprite.getPosition().x > -60) && (ptaszor.obstacleSprite.getPosition().x < 800)))
		{
			bush4.move(speed, dino, health);
		}
		if (!((bush4.obstacleSprite.getPosition().x > -120) && (bush4.obstacleSprite.getPosition().x < 800)) && !((ptaszor.obstacleSprite.getPosition().x > -60) && (ptaszor.obstacleSprite.getPosition().x < 800)))
		{
			bush.move(speed, dino, health);
		}
		if (!((bush4.obstacleSprite.getPosition().x > -120) && (bush4.obstacleSprite.getPosition().x < 800)) && !((bush.obstacleSprite.getPosition().x > -44) && (bush.obstacleSprite.getPosition().x < 800)))
		{
			ptaszor.move(speed, dino, health);
		}
		//bush.move(0.55, dino, health);
		//bush2.move(0.55, dino, health);
		//bush3.move(0.55, dino, health);
		//bush4.move(0.55, dino, health);
		//podanie koloru w window.clear() sprawia, że ten kolor staje się kolorem tła
		window.clear(sf::Color::White);
		window.draw(backgroundSprite);
		bush.draw(window);
		//bush2.draw(window);
		//bush3.draw(window);
		bush4.draw(window);
		ptaszor.draw(window);
		window.draw(dino);
		
		//Proba detekcji kolizji pixelowej
		
		if (Collision::PixelPerfectTest(dino,bush.obstacleSprite))
		{
			std::cout << "speed1 = " << speed << " " << "Collison!" << std::endl;
			

		}
		else if (Collision::PixelPerfectTest(dino, bush4.obstacleSprite))
		{
			std::cout << "speed1 = " << speed << " " << "Collison!" << std::endl;
			
		}
		/*else if (Collision::PixelPerfectTest(dino, bush3.obstacleSprite))
		{
			cout << "Collison!" << endl;
			
		}*/
		else if (Collision::PixelPerfectTest(dino, ptaszor.obstacleSprite))
		{
			std::cout << "speed1 = " << speed << " " << "Collison!" << std::endl;
			//cout << "Collison!" << endl;

		}
		else
		{
			std::cout << "speed1 = " << speed << " " << "No Collison!" << std::endl;
			topScores[0];
		}
		
		//wyświetlanie żyćka
		if (health >= 1)
		{
			healthText.setString(to_string(health));
			window.draw(healthText);
			//naliczanie punktów
			player_score = score_clock.getElapsedTime().asSeconds();
			scoreText.setString(to_string(player_score));
			window.draw(scoreText);
			float temp = player_score;

			//uzaleznienie predkosci od czasu
			temp = temp * 0.002f; //o ile zwieksza przyrasta predkosc co sekunde, teraz 0.002
			speed = 0.7f + temp;
			
		}
		else
		{
			int total_score = player_score;
			scoreText.setString("Your score: " + to_string(total_score));
			scoreText.setPosition(window.getSize().x / 2 - scoreText.getLocalBounds().width / 2, window.getSize().y / 2 - scoreText.getLocalBounds().height / 2 - healthText.getLocalBounds().height);
			healthText.setString(" GAME OVER\n(press ENTER)");
			healthText.setPosition(window.getSize().x / 2 - healthText.getLocalBounds().width/2, window.getSize().y / 2 - healthText.getLocalBounds().height/2);
			window.draw(healthText);
			window.draw(scoreText);
			on_ground = false;
			if (total_score > topScores[0])
			{
				scores << total_score << "\n" << topScores[0] << "\n" << topScores[1];
				scores.close();
			}
			else if (total_score > topScores[1])
			{
				scores << topScores[0] << "\n" << total_score << "\n" << topScores[1];
				scores.close();
			}
			else if (total_score > topScores[2])
			{
				scores << topScores[0] << "\n" << topScores[1] << "\n" << total_score;
				scores.close();
			}
		}
		window.display();
	}
}
