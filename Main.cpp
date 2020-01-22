#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Vector.h"
#include "Laser.h"
#include "Surface.h"
#include "Level.h"
#include "Trail.h"
#include "Launcher.h"
#include <vector>
#include <ctime>
#include "Key.h"
#include "Endpoint.h"
#include "Main.h"

const int surfaceRotateSpeed = 1;
bool gameStarted = false;
bool firstTime = true;
bool gotKey = false;
bool gameWon = false;
bool inEditorMode = true;

Vector mouseVector(0.0f, 0.0f);
sf::VertexArray Trail::lines;
int Laser::laserSpeed = 3;
int windowWidth = 1000;
int windowHeight = 800;
std::vector<Surface> planes;
float Surface::length = 100.0f;
float Surface::width = 20.0f;
float Surface::amountToAdd = atan(Surface::width / Surface::length);//Static members of a class must be defined outside of main function.
sf::Text winnerTexture;
enum GAMESTATE {mainMenu, inGame};
GAMESTATE state = inGame;

Launcher launcher;
Laser Beam;
Key key;
Endpoint endPoint;

int main() {

	sf::Font font;
	font.loadFromFile("Splatch.ttf");
	sf::Text winnerText("WINNER", font, 30);
	winnerText.setPosition(sf::Vector2f(500.0f, 400.0f));
	sf::Text inEditorModeText("", font, 10);
	inEditorModeText.setPosition(sf::Vector2f(100.0f, 50.0f));
	std::clock_t t1;
	t1 = clock();
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "ReFlite", sf::Style::Close | sf::Style::Titlebar, settings);
	window.setFramerateLimit(100);
	Trail trail;

	Level l("level.txt");
	key.applyTexture();
	endPoint.applyTexture();
	launcher.applyTexture();
	for (int i = 0; i < (int)planes.size(); i++) {
		std::cout << planes.at(i).pos.x << ", " << planes.at(i).pos.y << std::endl;
	}

	
	
	

	
	
	



	
	while (window.isOpen()) {//Game Loop. Similar to functino draw() Window.isOpen is window. because we made a varible called window;
		sf::Event evnt;//Make an event variable. 
		while (window.pollEvent(evnt)) {//Event handler. If event gotten
			switch (evnt.type) {//Check which event was triggered
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseWheelMoved:
				//Plane.angle++;
				mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);
				mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);

				for (int i = 0; i < (int)planes.size(); i++) {
					//planes.at(i).angle += 3;
					if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) < 20.0f && !gameStarted && planes.at(i).canRotate()) {//Collision detection with a point

						planes.at(i).angle += evnt.mouseWheel.delta < 0 ? surfaceRotateSpeed : surfaceRotateSpeed * -1;//If mousewheel direction is positive, move one way. Otherwise the other.

						planes.at(i).updatePoints();
					}
				}

				break;
			case(sf::Event::KeyPressed):
				if (evnt.key.code == sf::Keyboard::R) {
					Beam = Laser(launcher.pos.x + 40.0f, launcher.pos.y + 40.0f);
					Beam.setAngle(launcher.angle + 180.0f, launcher.pos.x + 40.0f, launcher.pos.y + 40.0f);
					trail.clearArr();
					gameStarted = false;
					gotKey = false;
					gameWon = false;
				}
				else if (evnt.key.code == sf::Keyboard::E) {
					inEditorMode = !inEditorMode;//Toggle between editor modes
					if (inEditorMode) { inEditorModeText.setString("EDITOR MODE"); }
					else { inEditorModeText.setString(""); }
				}

				break;
			case(sf::Event::MouseButtonPressed):
				/*std::cout << (clock() - t1) / (double)CLOCKS_PER_SEC * 1000 << std::endl;*/
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					if (inEditorMode) {
						planes.push_back(Surface(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 90.0f, true));
					}
					else {
						//std::cout << "New Plane at X: " << sf::Mouse::getPosition(window).x << " Y: " << sf::Mouse::getPosition(window).y << std::endl;
						if (!gameStarted) {
							Beam = Laser(launcher.pos.x + 40.0f, launcher.pos.y + 40.0f);//40.0f is launcher width / 2
							Beam.setAngle(launcher.angle + 180.0f, launcher.pos.x + 40.0f, launcher.pos.y + 40.0f);
							firstTime = false;
							gameStarted = true;
						}
					}
				}

				break;
			}
		}



		window.clear(sf::Color::Black);

		if(state == inGame){
			
			if (gameStarted) {
				trail.getBeamPos(Beam);
				if (key.checkCollision(Beam)) {
					std::cout << "Key!" << std::endl;
					gotKey = true;
				}
				if (endPoint.checkCollision(Beam) && gotKey) {
					std::cout << "EndPoint!" << std::endl;
					gameWon = true;

				}
				else {
					Beam.move();
				}

			}
			else {
				launcher.setAngle(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			}
			for (int i = 0; i < planes.size(); i++) {//Optimize where draw points goes. right now it is in the move() method.
				if (gameStarted) {
					planes.at(i).checkCollision(Beam);
				}
				//planes.at(i).drawPoints();

				window.draw(planes.at(i).plane);
				//window.draw(planes.at(i).point1);

			}
			window.draw(trail.draw());//This order matters. Draw trail after the surface so we can see the trail when laser goes "through" surface as a prism
			window.draw(key.shape);//Draw key and endpoint at end so laser is behind them
			window.draw(endPoint.shape);

			//window.draw(Beam.beam);
			for (int i = 0; i < 2; i++) {
				window.draw(launcher.shapeArr[i]);
			}
		}
		//Display winnerText
		if (gameWon) {
			window.draw(winnerText);
		}
		else if(state == mainMenu){
			//Display Main Menu Stuff
		}
		
		
		window.draw(inEditorModeText);
		window.display();//Display the window

	}

	return 0;


}