#include <iostream>
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
//
//Klasa do tworzenia przeszkód na mapie
class obstacle
{
public:
	sf::Sprite obstacleSprite;
	sf::Clock immune;
	//konstruktor klasy, trzeba podac link do tekstury oraz pocz¹tkow¹ pozycje przeszkody
	obstacle(string& texture_link,int basic_pos)
	{
		obstacleTexture.loadFromFile(texture_link);
		obstacleSprite.setTexture(obstacleTexture);
		obstacleSprite.setPosition(basic_pos, 472);
	}

	//metoda klasy odpowiadaj¹ca za przemieszczanie siê przeszkody oraz kolizjê z dino
	void move(float speed,sf::Sprite& dino,int& health)
	{
		//przemieszczanie siê przeszkody
		obstacleSprite.setPosition(obstacleSprite.getPosition().x - speed,obstacleSprite.getPosition().y);

		//ustawienie nowej losowej pozycji po prawej stronie ekranu gdy przeszkoda zniknie z lewej storny ekranu
		if(obstacleSprite.getPosition().x<-44) obstacleSprite.setPosition(rand()%1000 + 800, obstacleSprite.getPosition().y);

		//os³uga kolizji, wykona siê gdy dwa sprite'y siê zetkn¹ i gdy czas nieœmiertelnoœci bêdzie wiêkszy ni¿ 1 sekunda (zabezpieczenie przed setkami wykonañ podczas przechodzenia przez przeszkode)
		
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

	//metoda wyœwietlania przeszkody na ekranie
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
			if (windowEvent.type == sf::Event::KeyPressed && (windowEvent.key.code == sf::Keyboard::Up || windowEvent.key.code == sf::Keyboard::W))
			{
				menu.MoveUp();
			}
			//obs³uga przejœcia w dó³ przy u¿yciu klawisza S
			if (windowEvent.type == sf::Event::KeyPressed && (windowEvent.key.code == sf::Keyboard::Down || windowEvent.key.code == sf::Keyboard::S))
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

int jump(sf::Sprite &dino, bool& is_jump, bool& on_ground, sf::RenderWindow& window)
{

	//predkosc opadania i wznoszenia
	double gravity = 0.3;

	//jesli prawda, ze skok to dino porusza sie do gory
	if (is_jump)
	{
		dino.setPosition(dino.getPosition().x + 0.10, dino.getPosition().y - gravity);
	}

	//poruszanie sie w dol w innych przypadkach niz prawda ze skok
	else if(dino.getPosition().y != 441)
	{
		dino.setPosition(dino.getPosition().x + 0.10, dino.getPosition().y+gravity);
	}
	//sprawdzanie czy dino nie spada
	if (dino.getPosition().y >= 441 && dino.getPosition().x > window.getSize().x / 10)
	{
		dino.setPosition(dino.getPosition().x - 0.15, 441);
		on_ground = true;
	}
	//sprawdzanie czy jest aktualnie w locie
	if (dino.getPosition().y < 441)
	{
		on_ground = false;
	}
	//limit wysokosci dla skoku
	if (dino.getPosition().y <= 361)
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

	//!dziêki temu sf::Event::KeyPressed bêdzie wykonywa³o siê tylko raz na wciœniêcie klawisza!
	window.setKeyRepeatEnabled(false);

	//utworzenie uchwytu do otwarcia pliku
	fstream scores;

	//uruchomienie funkcji menu wyœwietlaj¹cej menu gry
	menuidentifier:
	menu(window,scores);

	//utworzenie sprite dinozaura oraz utworzenie tekstury i za³adowanie jej z pliku (³adowanie tekstury z obs³ug¹ b³êdu)
	sf::Texture dino_texture;
	if (!dino_texture.loadFromFile("./textures/almighty_dragon.png"))
		return EXIT_FAILURE;
	//sprawia, ¿e tekstury s¹ wyg³adzone? nw czy to coœ daje ale zawsze spoko
	dino_texture.setSmooth(true);
	sf::Sprite dino(dino_texture);
		dino.setTextureRect({ 0,0,55,60 });
		dino.setPosition(window.getSize().x / 10, 441);


	//t³o gry
	sf::Texture backgroundtxt1;
		backgroundtxt1.loadFromFile("./textures/background1.jpg");
		backgroundtxt1.setSmooth(true);
	sf::Texture backgroundtxt2;
		backgroundtxt2.loadFromFile("./textures/background2.jpg");
		backgroundtxt2.setSmooth(true);
	sf::Sprite backgroundSprite(backgroundtxt1);

	//zegar animacji i do naliczania punktów
	sf::Clock t0;
	sf::Clock score_clock;

	//iterator animacji
	int animation = 0;

	//utworzenie przeszkody
	string bushtxt= "./textures/bush.png";
	obstacle bush(bushtxt, 500);
	obstacle bush2(bushtxt, 800);
	obstacle bush3(bushtxt, 650);
	

	//u¿yte przy obs³udze skakania
	bool is_jump = false;
	bool on_ground = true;
	bool is_bending = false;
	/*Position dino_pos;
	dino_pos.y = dino.getPosition().y;
	dino_pos.x = dino.getPosition().x;*/

	//¯ycia dinozaura
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


	//g³ówna pêtla programu
	while (window.isOpen())
	{
		//animacja biegn¹cego dinozaura oraz zmiana t³a gry
		/*if (t0.getElapsedTime().asSeconds() >= 0.1f)
		{
			dino.setTextureRect({ 55 * animation,0,55,60 });
			(animation % 2) ? backgroundSprite.setTexture(backgroundtxt2) : backgroundSprite.setTexture(backgroundtxt1);
			animation++;
			t0.restart();
			if (animation == 4)
				animation = 0;
		}*/
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) && on_ground)
			{
				is_bending = true;
				if (!dino_texture.loadFromFile("./textures/almighty_dragon_bending.png"))
					return EXIT_FAILURE;
				dino.setTextureRect({ 0,0,83,23 });
				dino.setPosition(window.getSize().x / 10, 541);
				if (t0.getElapsedTime().asSeconds() >= 0.1f)
				{
					dino.setTextureRect({ 55 * animation,0,83,23 });
					(animation % 2) ? backgroundSprite.setTexture(backgroundtxt2) : backgroundSprite.setTexture(backgroundtxt1);
					animation++;
					t0.restart();
					if (animation == 4)
						animation = 0;
				}
				
			}
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
			{
				is_bending = false;
				
			}
			if (!is_bending)
			{
				if (!dino_texture.loadFromFile("./textures/almighty_dragon.png"))
					return EXIT_FAILURE;
				dino.setTextureRect({ 0,0,55,60 });
				if (t0.getElapsedTime().asSeconds() >= 0.1f)
				{
					dino.setTextureRect({ 55 * animation,0,55,60 });
					(animation % 2) ? backgroundSprite.setTexture(backgroundtxt2) : backgroundSprite.setTexture(backgroundtxt1);
					animation++;
					t0.restart();
					if (animation == 4)
						animation = 0;
				}
			}
		}
		jump(dino, is_jump, on_ground,window);

		bush.move(0.35, dino, health);
		bush2.move(0.35, dino, health);
		bush3.move(0.35, dino, health);
		//podanie koloru w window.clear() sprawia, ¿e ten kolor staje siê kolorem t³a
		window.clear(sf::Color::White);
		window.draw(backgroundSprite);
		bush.draw(window);
		bush2.draw(window);
		bush3.draw(window);
		window.draw(dino);
		
		//Proba detekcji kolizji pixelowej
		
		if (Collision::PixelPerfectTest(dino,bush.obstacleSprite))
		{
			cout << "Collison!" << endl;
			

		}
		else if (Collision::PixelPerfectTest(dino, bush2.obstacleSprite))
		{
			cout << "Collison!" << endl;
			
		}
		else if (Collision::PixelPerfectTest(dino, bush3.obstacleSprite))
		{
			cout << "Collison!" << endl;
			
		}
		else
		{
			cout << "No collision!" << endl;
		}
		


 

		//naliczanie punktów
		player_score = score_clock.getElapsedTime().asSeconds();
		scoreText.setString(to_string(player_score));
		window.draw(scoreText);

		//wyœwietlanie ¿yæka
		if (health >= 1)
		{
			healthText.setString(to_string(health));
			window.draw(healthText);
		}
		else
		{
			//wyswietlanie wyniku UWAGA - NIE ZATRZYMUJE SIE, BO ZEGAR CALY CZAS NALICZA
			int total_score = player_score;
			scoreText.setString(to_string(total_score));
			scoreText.setPosition(window.getSize().x / 2 - healthText.getLocalBounds().width / 2, window.getSize().y / 2 - healthText.getLocalBounds().height / 2 + 100);
			healthText.setString(" GAME OVER\n(press ENTER)");
			healthText.setPosition(window.getSize().x / 2 - healthText.getLocalBounds().width/2, window.getSize().y / 2 - healthText.getLocalBounds().height/2);
			window.draw(healthText);
			window.draw(scoreText);
			on_ground = false;

		}
		window.display();
	}
}
