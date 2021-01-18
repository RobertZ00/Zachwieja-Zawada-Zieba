#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <time.h>
#include <string>
#include <cstdlib>
#include "menu.h"
#include "scorescreen.h"
#include "collision.h"

using namespace std;

//Klasa do tworzenia przeszkód na mapie
class obstacle
{
public:
	sf::Sprite obstacleSprite;
	sf::Clock immune;
	//konstruktor klasy, trzeba podac link do tekstury oraz pocz�tkow� pozycje przeszkody
	obstacle(string& texture_link,int basic_pos)
	{
		obstacleTexture.loadFromFile(texture_link);
		obstacleSprite.setTexture(obstacleTexture);
		obstacleSprite.setPosition(basic_pos, 472);
	}

	//metoda klasy odpowiadaj�ca za przemieszczanie si� przeszkody oraz kolizj� z dino
	void move(float speed,sf::Sprite& dino,int& health)
	{
		//przemieszczanie si� przeszkody
		obstacleSprite.setPosition(obstacleSprite.getPosition().x - speed,obstacleSprite.getPosition().y);

		//ustawienie nowej losowej pozycji po prawej stronie ekranu gdy wywołana przeszkoda zniknie z lewej storny ekranu
		if(obstacleSprite.getPosition().x<-120) obstacleSprite.setPosition(rand()%1000 + 800, obstacleSprite.getPosition().y);

		//os�uga kolizji, wykona si� gdy dwa sprite'y si� zetkn� i gdy czas nie�miertelno�ci b�dzie wi�kszy ni� 1 sekunda (zabezpieczenie przed setkami wykona� podczas przechodzenia przez przeszkode)
		
		if (Collision::PixelPerfectTest(dino,obstacleSprite))
		{
			if(immune.getElapsedTime().asSeconds() > 1)
			{ 
			health = health - 1;
			immune.restart();
			cout << "dziala" << endl;
			}
		}
	}

	//metoda wy�wietlania przeszkody na ekranie
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
	//otwarcie pliku score.txt z opcj� odczytu
	scores.open("score.txt");
	if (scores.good())
	{
		string out;
		string linia;
		//zapisanie ca�ego pliku do zmiennej linia
		for (int i = 0; i < 3; i++)
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
			if (windowEvent.type == sf::Event::KeyPressed && (windowEvent.key.code == sf::Keyboard::Up || windowEvent.key.code == sf::Keyboard::W))
			{
				menu.MoveUp();
			}
			//obs�uga przej�cia w d� przy u�yciu klawisza S
			if (windowEvent.type == sf::Event::KeyPressed && (windowEvent.key.code == sf::Keyboard::Down || windowEvent.key.code == sf::Keyboard::S))
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

	//podwojny krzak
	obstacle bush4(double_bush_txt, 820);

	//ptaszor
	obstacle ptaszor(ptaszortxt, 830);
	ptaszor.obstacleSprite.setPosition(820, 410);

	//użyte przy obs�udze skakania
	bool is_jump = false;
	bool on_ground = true;
	bool is_bending = false;

	//predkosc poruszania sie przeszkod
	float speed = 0.9f;

	//życia dinozaura
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
	string topScores[3];
	for (int i = 0; i < 3; i++)
	{
		getline(scores, topScores[i]);
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
		
		//podanie koloru w window.clear() sprawia, że ten kolor staje się kolorem tła
		window.clear(sf::Color::White);
		window.draw(backgroundSprite);
		bush.draw(window);
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
		else if (Collision::PixelPerfectTest(dino, ptaszor.obstacleSprite))
		{
			std::cout << "speed1 = " << speed << " " << "Collison!" << std::endl;
			//cout << "Collison!" << endl;

		}
		else
		{
			std::cout << "speed1 = " << speed << " " << "No Collison!" << topScores[0] << std::endl;
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
			speed = 0.9f + temp;
			
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
			scores.close();
			ofstream scoresfile("score.txt");
			if (total_score > atoi(topScores[0].c_str()) && scoresfile.is_open())
			{
				scoresfile << total_score << "\n" << topScores[0] << "\n" << topScores[1];
				scoresfile.close();
			}
			else if (total_score > atoi(topScores[1].c_str()) && scoresfile.is_open())
			{
				scoresfile << topScores[0] << "\n" << total_score << "\n" << topScores[1];
				scoresfile.close();
			}
			else if (total_score > atoi(topScores[2].c_str()) && scoresfile.is_open())
			{
				scoresfile << topScores[0] << "\n" << topScores[1] << "\n" << total_score;
				scoresfile.close();
			}
		}
		window.display();
	}
}
