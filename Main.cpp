#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Text.hpp"
#include <iostream>

#include "Button.h"
#include "Endpoint.h"
#include "Key.h"
#include "Laser.h"
#include "Launcher.h"
#include "Level.h"
#include "Main.h"
#include "Surface.h"
#include "Trail.h"
#include "Vector.h"
#include <ctime>
#include <vector>

const int surfaceRotateSpeed = 1;
bool gameStarted = false;
bool firstTime = true;
bool gotKey = false;
bool gameWon = false;
bool inEditorMode = false;
const int buttonsLength = 2;
const int editingButtonsLength = 2;


Button buttons[buttonsLength] = {Button(150.0f + 500.0f - 300.0f, 400.0f, 300.0f, 100.0f, std::string("playBtn")),
					Button(150.0f + 500.0f - 300.0f, 520.0f, 300.0f, 100.0f, std::string("importbtn"))};

Button editingButtons[editingButtonsLength] = { Button(10.0f, 150.0f, 100.0f, 500.0f, std::string("editingButtons")),  Button(120.0f, 180.0f, 60.0f, 40.0f, std::string("arrow")) };
int currentEditingObject = 0;//SurfaceF = 0, SurfaceFixed = 1,Launcher = 2, Key = 3, Endpoint = 4
Vector mouseVector(0.0f, 0.0f);
sf::VertexArray Trail::lines;
int Laser::laserSpeed = 3;
int windowWidth = 1000;
int windowHeight = 800;
std::vector<Surface> planes;
float Surface::length = 100.0f;
float Surface::width = 20.0f;
float Surface::amountToAdd =
    atan(Surface::width / Surface::length); // Static members of a class must be
// defined outside of main function.
sf::Text winnerTexture;
enum GAMESTATE { mainMenu, inGame };
enum GAMESTATE state = mainMenu;

Launcher launcher;
Laser Beam;
Key key;
Endpoint endPoint;

int main() {
  //buttons.emplace_back(playButton);
  //buttons.emplace_back(importButton);

  sf::Font font;
  font.loadFromFile("Splatch.ttf");

  sf::Text winnerText("WINNER", font, 30);
  winnerText.setPosition(sf::Vector2f(500.0f, 400.0f));

  sf::Text inEditorModeText("", font, 10);
  inEditorModeText.setPosition(sf::Vector2f(10.0f, 10.0f));

  sf::Text titleText("REFLITE", font, 64);
  titleText.setPosition(sf::Vector2f((float) windowWidth / 3.0f, (float)windowHeight / 8.0f));

  std::clock_t timer = clock();

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "ReFlite",
                          sf::Style::Close | sf::Style::Titlebar, settings);
  window.setFramerateLimit(120);
  Trail trail;

  Level l("level.txt");
  key.applyTexture();
  endPoint.applyTexture();
  launcher.applyTexture();

  for (int i = 0; i < buttonsLength; i++) {
    buttons[i].applyTexture();
  }
  for (int i = 0; i < editingButtonsLength; i++) {
	  editingButtons[i].applyTexture();
  }
  for (int i = 0; i < (int)planes.size(); i++) {
    std::cout << planes.at(i).pos.x << ", " << planes.at(i).pos.y << std::endl;
  }

  while (window.isOpen()) { // Game Loop. Similar to functino draw()
    // Window.isOpen is window. because we made a
    // varible called window;
    sf::Event evnt;                  // Make an event variable.
    while (window.pollEvent(evnt)) { // Event handler. If event gotten
      switch (evnt.type) {           // Check which event was triggered
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::MouseWheelMoved:
        // Plane.angle++;
        mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);
        mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);

        for (int i = 0; i < (int)planes.size(); i++) {
          // planes.at(i).angle += 3;
          if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) < 20.0f && !gameStarted && (planes.at(i).canRotate() || inEditorMode)) { // Collision detection with a point
            planes.at(i).angle +=
                evnt.mouseWheel.delta < 0
                    ? surfaceRotateSpeed
                    : surfaceRotateSpeed * -1; // If mousewheel direction is
            // positive, move one way.
            // Otherwise the other.

            planes.at(i).updatePoints();
          }
        }

        break;
      case (sf::Event::KeyPressed):
        if (evnt.key.code == sf::Keyboard::R) {
          Beam = Laser(launcher.pos.x + 40.0f, launcher.pos.y + 40.0f);
          Beam.setAngle((float)launcher.angle + 180.0f, launcher.pos.x + 40.0f,
                        launcher.pos.y + 40.0f);
          trail.clearArr();
          gameStarted = false;
          gotKey = false;
          gameWon = false;
        } else if (evnt.key.code == sf::Keyboard::E) {
          inEditorMode = !inEditorMode; // Toggle between editor modes
          if (inEditorMode) {
            inEditorModeText.setString("EDITOR MODE");
          } else {
            inEditorModeText.setString("");
          }
        } else if (evnt.key.code == sf::Keyboard::C) {
          
          if (((clock() - timer) / (double)CLOCKS_PER_SEC * 1000 < 500) && !gameStarted) {
            std::cout << "Double C" << std::endl;
            l.deleteLevel();
			} else {
            timer = clock();
          }
          
        }
		else if(evnt.key.code == sf::Keyboard::S){
			if (((clock() - timer) / (double)CLOCKS_PER_SEC * 1000 < 500) && !gameStarted) {
				l.saveLevel();
				
			}
			else {
				timer = clock();
			}

		}
		
		else if (evnt.key.code == sf::Keyboard::Num1) {//Switch the arrow's y position to curredntly editing object sprite
			currentEditingObject = 0;
			editingButtons[1].rect.setPosition(120.0f, 180.0f);
		}
		else if (evnt.key.code == sf::Keyboard::Num2) {
			currentEditingObject = 1;
			editingButtons[1].rect.setPosition(120.0f, 280.0f);
		}
		else if (evnt.key.code == sf::Keyboard::Num3) {
			currentEditingObject = 2;
			editingButtons[1].rect.setPosition(120.0f, 380.0f);
		}
		else if (evnt.key.code == sf::Keyboard::Num4) {
			currentEditingObject = 3;
			editingButtons[1].rect.setPosition(120.0f, 480.0f);
		}
		else if (evnt.key.code == sf::Keyboard::Num5) {
			currentEditingObject = 4;
			editingButtons[1].rect.setPosition(120.0f, 580.0f);
		}

        break;
      case (sf::Event::MouseButtonPressed):
        /*std::cout << (clock() - t1) / (double)CLOCKS_PER_SEC * 1000 <<
         * std::endl;*/

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
          if (state == mainMenu) {
            
            if (buttons[0].isClicked((float)sf::Mouse::getPosition(window).x, (float) sf::Mouse::getPosition(window).y)) { // Play button
				state = inGame;
            
			} else if (buttons[1].isClicked(
                            (float)sf::Mouse::getPosition(window).x,
                            (float)sf::Mouse::getPosition(window).y)) {
            std::cout << "Hello";
            }
            
          } else if (inEditorMode) {
			  switch (currentEditingObject) {
					case(0):
						planes.emplace_back(Surface((float)sf::Mouse::getPosition(window).x,
							(float)sf::Mouse::getPosition(window).y, 90.0f,
							false));//Emplace back a non-rotatable surface
					break;
					
					case(1):
						planes.emplace_back(Surface((float)sf::Mouse::getPosition(window).x,
							(float)sf::Mouse::getPosition(window).y, 90.0f,
							true));//Emplace back rotatable surface
					break;
					
					case(2):
						launcher.pos.x = (float)sf::Mouse::getPosition(window).x - launcher.launcherLength / 2;
						launcher.pos.y = (float)sf::Mouse::getPosition(window).y - launcher.launcherLength / 2;
						launcher.updateSprite();
					break;
					case(3):
						key.pos.x = (float)sf::Mouse::getPosition(window).x - key.length / 2;
						key.pos.y = (float)sf::Mouse::getPosition(window).y - key.length / 2;
						key.updateSprite();
					break;
					case(4):
						endPoint.pos.x = (float)sf::Mouse::getPosition(window).x - endPoint.length / 2;
						endPoint.pos.y = (float)sf::Mouse::getPosition(window).y - endPoint.length / 2;
						endPoint.updateSprite();
						break;

				}
            
          } else {
            // std::cout << "New Plane at X: " <<
            // sf::Mouse::getPosition(window).x << " Y: " <<
            // sf::Mouse::getPosition(window).y << std::endl;
            if (!gameStarted) {
              Beam =
                  Laser(launcher.pos.x + 40.0f,
                        launcher.pos.y + 40.0f); // 40.0f is launcher width / 2
              Beam.setAngle((float)launcher.angle + 180.0f, launcher.pos.x + 40.0f,
                            launcher.pos.y + 40.0f);
              firstTime = false;
              gameStarted = true;
            }
          }
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
          mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);
          mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);

          for (unsigned int i = 0; i < planes.size(); i++) {
            if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) <
                20.0f) {
              std::cout << "Clicked on a suraface" << std::endl;
              planes.erase(planes.begin() + i);
            }
          }
        }

        break;
      }
    }

    window.clear(sf::Color::Black);
    switch (state) {
    case (inGame):
      if (gameStarted) {
        trail.getBeamPos(Beam);
        if (key.checkCollision(Beam)) {
          std::cout << "Key!" << std::endl;
          gotKey = true;
        }
        if (endPoint.checkCollision(Beam) && gotKey) {
          std::cout << "EndPoint!" << std::endl;
          gameWon = true;
        } else {
          Beam.move();
        }
      } else if (!inEditorMode) {
        launcher.setAngle((float)sf::Mouse::getPosition(window).x,
                          (float)sf::Mouse::getPosition(window).y);
      }
      for (unsigned int i = 0; i < planes.size();
           i++) { // Optimize where draw points goes. right now it is in the
        // move() method.
        if (gameStarted) {
          planes.at(i).checkCollision(Beam);
        }
        // planes.at(i).drawPoints();

        window.draw(planes.at(i).plane);
        // window.draw(planes.at(i).point1);
      }
      window.draw(trail.draw()); // This order matters. Draw trail after the
      // surface so we can see the trail when laser
      // goes "through" surface as a prism
      window.draw(
          key.shape); // Draw key and endpoint at end so laser is behind them
      window.draw(endPoint.shape);

      // window.draw(Beam.beam);
      for (int i = 0; i < 2; i++) {
        window.draw(launcher.shapeArr[i]);
      }

      break;

    case (mainMenu):
      window.draw(titleText);
      for (int i = 0; i < buttonsLength; i++) {
        window.draw(buttons[i].rect);
      }

      break;
    }
	if (inEditorMode) {
		for (int i = 0; i < editingButtonsLength; i++) {
			window.draw(editingButtons[i].rect);

		}
	}
    // Display winnerText
    if (gameWon) {
      window.draw(winnerText);
    }

    window.draw(inEditorModeText);
    window.display(); // Display the window
  }

  return 0;
}