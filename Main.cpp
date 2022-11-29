#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/Music.hpp"

#include <string>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <vector>

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

//The timer() is always running :(
void updateDisplayLevels(sf::Font* font);
void applyTextures(bool onlyPlanes = false);//Define the function to apply the textures
void changeScore();
const int surfaceRotateSpeed = 1;//Constant to determine the mouse wheel rotation speed of surface
bool gameStarted = false;//Whether laser has started moving
bool firstTime = true;//
bool displayUnsaveWarning = false;//If user tries closing window without saving unsaved work
bool gotKeys = false;//Whether key is obtained
bool gameWon = false;//If endpoint and key have been touched
bool inEditorMode = false;//If editing
bool isUnsaved = false;//Unsaved work
bool displayLevels = false;
bool levelSelected = false;
bool typingNewLevel = false;
bool removeLevelDoubleClicked = false;
bool displayNoLevelSelected = false;
bool displayMainHelp = false;
bool displayEditorHelp = false;
const int buttonsLength = 4;//Length of buttons array
const int editingButtonsLength = 2;//Editing buttons array length
const int cursorObjectLength = 5;
const float arrowXPosition = 140.0f;
int keysGotten = 0;
Vector mouseVector(0.0f, 0.0f);//Used to calculate position of mouse when a surface is being rotated to see which surface to affect
sf::VertexArray Trail::lines;//Array of laser trail
int Laser::laserSpeed = 3;//Speed at which laser moves
sf::Clock Clock;
int windowWidth = 1000;
int windowHeight = 800;
int score = 0;

std::string newLevelString;
int isDisplayingSaved = false;
std::vector<Surface> planes;//Vector of surfaces
float Surface::length = 100.0f;//Size of the surface
float Surface::width = 20.0f;
std::vector<Key> keys;


sf::Text currentLevelText;
//Buttons
std::vector<Button> levelsVector;
Button mainHelpButton(500.0f, 400.0f, 600.0f, 400.0f, "gameInstructions");
Button editorHelpButton(500.0f, 400.0f, 600.0f, 400.0f, "editorInstruction");
Button typingLevelTextButton(450.0f, 150.0f, 100.0f, 50.0f, "");
Button scoreText(50.0f, 50.0f, 70.0f, 50.0f, "0");
Button newLevelBtn(500.0f, 200.0f, 150.0f, 50.0f, "Create Level");
Button noLevelSelectedButton(500.0f, 450.0f, 405.0f, 135.0f, "noLevelSelected");
Button unsavedWorkText(500.0f, 400.0f, 405.0f, 135.0f, "unsaved");
Button buttons[buttonsLength] = { Button(500.0f, 400.0f, 300.0f, 100.0f, std::string("playBtn")), Button(500.0f, 550.0f, 300.0f, 100.0f, std::string("editBtn")), Button(500.0f, 680.0f, 300.0f, 100.0f, std::string("selectLevelBtn")), Button(500.0f, 120.0f, 805.0f, 190.0f, std::string("title")) };
Button editingButtons[editingButtonsLength] = { Button(80.0f, 350.0f, 160.0f, 500.0f, std::string("editingButtons")),  Button(180.0f, 130.0f, 60.0f, 40.0f, std::string("arrow")) };
Button cursorObject[cursorObjectLength] = { Button(0.0f, 0.0f, Surface::length * 0.5f, Surface::width * 0.5f, "surfaceF"), Button(0.0f, 0.0f, Surface::length * 0.5f, Surface::width * 0.5f, "surfaceR"),
											Button(0.0f, 0.0f, 80.0f, 80.0f, "emitter"), Button(0.0f, 0.0f, 30.0f, 30.0f, "key"), Button(0.0f, 0.0f, 30.0f, 30.0f, "target") };
//Enum type thing to see which object is being used in editor mode
int currentEditingObject = 0;//SurfaceF = 0, SurfaceFixed = 1,Launcher = 2, Key = 3, Endpoint = 4

float Surface::amountToAdd =
atan(Surface::width / Surface::length); // Static members of a class must be
// defined outside of main function.

enum GAMESTATE { mainMenu, inGame };//For a switch statement to see which game state we're in
enum GAMESTATE state = mainMenu;//State to start the game in

Launcher launcher;
Laser Beam;
Endpoint endPoint;
Level l;

int main() {
	//Audio
	sf::Music music;
	music.openFromFile("audio/music.wav");
	music.setLoop(true);
	music.play();
	
	
	sf::Font font;
	font.loadFromFile("font.ttf");
	
	currentLevelText.setFont(font);
	currentLevelText.setString("Current Level: None!");
	currentLevelText.setCharacterSize(16);
	currentLevelText.setPosition(sf::Vector2f(400.0f, 200.0f));
	//buttons.emplace_back(playButton);
	//buttons.emplace_back(importButton);
	typingLevelTextButton.applyTexture(&font);
	newLevelBtn.applyTexture(&font);
	scoreText.applyTexture(&font);
	sf::Text winnerText("WINNER", font, 42);
	winnerText.setPosition(sf::Vector2f(300.0f, 400.0f));

	sf::Text saveTooltip("CLICK 'H' FOR HELP! DOUBLE TAP 'S' TO SAVE LEVEL!", font, 14);
	saveTooltip.setPosition(sf::Vector2f(650.0f, 780.0f));

	std::clock_t timer = clock();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "ReFlite",
		sf::Style::Close | sf::Style::Titlebar, settings);
	window.setFramerateLimit(120);
	Trail trail;

	/*key.applyTexture();//Since we recreate the level everytime a new one is selected, we must apply the texture each time.
	endPoint.applyTexture();
	launcher.applyTexture();*/
	mainHelpButton.applyTexture();
	editorHelpButton.applyTexture();
	unsavedWorkText.applyTexture();
	noLevelSelectedButton.applyTexture();
	for (int i = 0; i < buttonsLength; i++) {//Apply the sprites to all the buttons
		buttons[i].applyTexture();
	}
	for (int i = 0; i < editingButtonsLength; i++) {
		editingButtons[i].applyTexture();
	}
	for (int i = 0; i < cursorObjectLength; i++) {
		cursorObject[i].applyTexture();
	}

	while (window.isOpen()) { // Game Loop. Similar to functino draw()
	  // Window.isOpen is window. because we made a
	  // varible called window;
		sf::Event evnt;                  // Make an event variable.
		while (window.pollEvent(evnt)) { // Event handler. If event gotten
			switch (evnt.type) {           // Check which event was triggered
			case sf::Event::Closed://Only close if work is saved
				if (isUnsaved) {
					displayUnsaveWarning = true;
				}
				else {
					window.close();
				}

				break;
			case(sf::Event::MouseWheelMoved):
				// Plane.angle++;
				mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);//Get a vector of mouse location too see which plane is being rotated
				mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);

				for (int i = 0; i < (int)planes.size(); i++) {//Loop through planes and see which one is being roatated
					// planes.at(i).angle += 3;
					if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) < 20.0f && !gameStarted && (planes.at(i).canRotate() || inEditorMode)) { // Collision detection with a point
						planes.at(i).angle +=
							evnt.mouseWheel.delta < 0
							? surfaceRotateSpeed
							: surfaceRotateSpeed * -1; // If mousewheel direction is
					// positive, move one way.
					// Otherwise the other.

						planes.at(i).updatePoints();//When it is moved update it's 4 points (vertices)
					}
				}

				break;

			case (sf::Event::KeyPressed):
				if (evnt.key.code == sf::Keyboard::R) {//Set the laser object back to the launchers center if game reset
					Beam = Laser(launcher.pos.x + 40.0f, launcher.pos.y + 40.0f);
					Beam.setAngle((float)launcher.angle + 180.0f, (float)launcher.pos.x + 40.0f,
						(float)launcher.pos.y + 40.0f);
					trail.clearArr();//Remove all the trails
					gameStarted = false;//Game is not started because we restarted
					gotKeys = false;
					keysGotten = 0;
					gameWon = false;
					for (unsigned int i = 0; i < keys.size(); i++) {//When game restarts make all keys isGotten to false
						keys.at(i).isGotten = false;
					}
					score = 0;
					changeScore();
				}
				//else if (evnt.key.code == sf::Keyboard::E && !gameStarted) {
				//	isUnsaved = true;
				//	inEditorMode = !inEditorMode; // Toggle between editor modes
				//	if (inEditorMode) {//Always have text at top right that says editor mode or nothing
				//		inEditorModeText.setString("EDITOR MODE! USE THE NUMBER KEYS TO SELECT AN OBJECT AND LEFT CLICK TO PLACE!");
				//	}
				//	else {
				//		inEditorModeText.setString("PRESS 'E' TO ENABLE EDITOR MODE!");
				//	}
				//}
				else if (evnt.key.code == sf::Keyboard::C) {
					if (inEditorMode) {
						if (((clock() - timer) / (double)CLOCKS_PER_SEC * 1000 < 500) && !gameStarted) {//Check if last 'c' button was clicked 500ms before to see if double clicked
							std::cout << "Double C" << std::endl;

							l.clearLevel();
						}
						else {
							timer = clock();
						}
					}
				}
				else if (evnt.key.code == sf::Keyboard::Escape) {
					if(state == inGame && !displayUnsaveWarning && !displayEditorHelp){
						state = mainMenu;
					}
					if (inEditorMode && !displayUnsaveWarning && !displayEditorHelp) {
						std::cout << displayEditorHelp;
						l.saveLevel();
						isUnsaved = false;
						saveTooltip.setString("SAVED!");
						isDisplayingSaved = true;
						inEditorMode = false;
						state = mainMenu;
					}
					if (inEditorMode && displayEditorHelp) {
						displayEditorHelp = !displayEditorHelp;
					}
					if (displayLevels) {
						displayLevels = false;
						typingNewLevel = false;
					}
					
					displayMainHelp = false;
				}
				else if (evnt.key.code == sf::Keyboard::S) {
					if (((clock() - timer) / (double)CLOCKS_PER_SEC * 1000 < 500) && !gameStarted) {
						l.saveLevel();
						isUnsaved = false;
						displayUnsaveWarning = false;
						saveTooltip.setString("SAVED!");
						isDisplayingSaved = true;
					}
					else {
						timer = clock();//This starts timer if not within 500ms. Means first click
					}
				}

				else if (evnt.key.code == sf::Keyboard::Num1 || evnt.key.code == sf::Keyboard::Numpad1) {//Switch the arrow's y position to curredntly editing object sprite
					currentEditingObject = 0;
					editingButtons[1].rect.setPosition(arrowXPosition, (currentEditingObject * 100.0f) + 120.0f);//Fancy little system here. Since currentEditingObject is an int, multiply it by the base y position
				}
				else if (evnt.key.code == sf::Keyboard::Num2 || evnt.key.code == sf::Keyboard::Numpad2) {
					currentEditingObject = 1;
					editingButtons[1].rect.setPosition(arrowXPosition, (currentEditingObject * 100.0f) + 120.0f);
				}
				else if (evnt.key.code == sf::Keyboard::Num3 || evnt.key.code == sf::Keyboard::Numpad3) {
					currentEditingObject = 2;
					editingButtons[1].rect.setPosition(arrowXPosition, (currentEditingObject * 100.0f) + 120.0f);
				}
				else if (evnt.key.code == sf::Keyboard::Num4 || evnt.key.code == sf::Keyboard::Numpad4) {
					currentEditingObject = 3;
					editingButtons[1].rect.setPosition(arrowXPosition, (currentEditingObject * 100.0f) + 120.0f);
				}
				else if (evnt.key.code == sf::Keyboard::Num5 || evnt.key.code == sf::Keyboard::Numpad5) {
					currentEditingObject = 4;
					editingButtons[1].rect.setPosition(arrowXPosition, (currentEditingObject * 100.0f) + 120.0f);
				}
				else if (evnt.key.code == sf::Keyboard::Enter) {
					if (typingNewLevel) {
						typingNewLevel = false;
						newLevelString += ".txt";
						l.newLevel(newLevelString);//Create the new level and add .txt file extension
						newLevelString.clear();
						typingLevelTextButton.spriteLocation.clear();
						typingLevelTextButton.update();
						updateDisplayLevels(&font);
					}
				}
				else if (evnt.key.code == sf::Keyboard::Left) {
					mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);//Get a vector of mouse location too see which plane is being rotated
					mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);

					for (int i = 0; i < (int)planes.size(); i++) {//Loop through planes and see which one is being roatated
						// planes.at(i).angle += 3;
						if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) < 20.0f && !gameStarted && (planes.at(i).canRotate() || inEditorMode)) { // Collision detection with a point
							planes.at(i).angle += surfaceRotateSpeed * -1;
							// positive, move one way.
							// Otherwise the other.

							planes.at(i).updatePoints();//When it is moved update it's 4 points (vertices)
						}
					}
				}
				else if (evnt.key.code == sf::Keyboard::Right) {
					mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);//Get a vector of mouse location too see which plane is being rotated
					mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);

					for (int i = 0; i < (int)planes.size(); i++) {//Loop through planes and see which one is being roatated
						// planes.at(i).angle += 3;
						if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) < 20.0f && !gameStarted && (planes.at(i).canRotate() || inEditorMode)) { // Collision detection with a point
							planes.at(i).angle +=
								evnt.mouseWheel.delta < 0
								? surfaceRotateSpeed
								: surfaceRotateSpeed; // If mousewheel direction is
						// positive, move one way.
						// Otherwise the other.

							planes.at(i).updatePoints();//When it is moved update it's 4 points (vertices)
						}
					}
				}
				else if (evnt.key.code == sf::Keyboard::H) {
					if (typingNewLevel) {
						break;
					}
					if (inEditorMode) {
						displayEditorHelp = !displayEditorHelp;
					}
					else {
						displayMainHelp = !displayMainHelp;
					}
				}

				break;
			case(sf::Event::MouseMoved):
				if(state == mainMenu){
					for(int i = 0; i < buttonsLength - 1; i++){
						if(buttons[i].isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)){
							buttons[i].rect.setFillColor(sf::Color::Cyan);
						}
						else{
							buttons[i].rect.setFillColor(sf::Color::White);
						}
					}
				}
				if (displayLevels) {
					if (newLevelBtn.isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {
						newLevelBtn.rect.setFillColor(sf::Color::Magenta);
					}
					else {
						newLevelBtn.rect.setFillColor(sf::Color::White);
					}
					for (unsigned int i = 0; i < levelsVector.size(); i++) {
						if (levelsVector.at(i).isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {
							levelsVector.at(i).rect.setFillColor(sf::Color::Red);
						}
						else {
							levelsVector.at(i).rect.setFillColor(sf::Color::White);
						}
					}
				}
				if(state == inEditorMode){
					mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);
					mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);

					for (int i = 0; i < (int)planes.size(); i++) {//Loop through planes and see which one is being roatated
											// planes.at(i).angle += 3;
						if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) < 20.0f) { // Collision detection with a point
							planes.at(i).plane.setScale(1.5f, 1.5f);
							std::cout << "Plane";
						}
						else {
							planes.at(i).plane.setScale(1.0f, 1.0f);
							planes.at(i).plane.setFillColor(sf::Color::White);
						}
					}
				}
				break;

			case (sf::Event::MouseButtonPressed):
				/*std::cout << (clock() - t1) / (double)CLOCKS_PER_SEC * 1000 <<
				 * std::endl;*/

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					if (state == mainMenu) {
						if (buttons[0].isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y) && !displayLevels) { // Play button
							if (levelSelected) {
								state = inGame;
							}
							else {
								displayNoLevelSelected = true;
								timer = clock();
							}
						}
						else if (buttons[1].isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {//Editor button
							if (levelSelected) {
								state = inGame;
								inEditorMode = true;
								isUnsaved = true;
							}
							else {
								displayNoLevelSelected = true;
								timer = clock();
							}
						}
						else if (buttons[2].isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {//Clicked selecta level
							updateDisplayLevels(&font);
						}
						else if (displayLevels) {//Clicked on a Level
							if (newLevelBtn.isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {
								typingNewLevel = true;
							}
							else {
								for (unsigned int i = 0; i < levelsVector.size(); i++) {
									if (levelsVector.at(i).isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {
										l = Level(levelsVector.at(i).spriteLocation + ".txt");

										applyTextures();
										applyTextures(true);
										levelSelected = true;
										displayLevels = false;
										currentLevelText.setString("Current Level: '" + levelsVector.at(i).spriteLocation + "'");
									}
								}
							}
						}
					}
					else if (displayUnsaveWarning) {//Check if unsaved work button is clicked to close without saving
						if (unsavedWorkText.isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {
							window.close();
						}
					}
					else if (inEditorMode) {
						switch (currentEditingObject) {
						case(0):
							planes.emplace_back(Surface((float)sf::Mouse::getPosition(window).x,
								(float)sf::Mouse::getPosition(window).y, 90.0f,
								false));//Emplace back a non-rotatable surface. Emplace creates the object inside the vector instead of creating it here then creating it there
							applyTextures(true);
							break;

						case(1):
							planes.emplace_back(Surface((float)sf::Mouse::getPosition(window).x,
								(float)sf::Mouse::getPosition(window).y, 90.0f,
								true));//Emplace back rotatable surface
							applyTextures(true);
							break;

						case(2):
							launcher.pos.x = (float)sf::Mouse::getPosition(window).x - launcher.launcherLength / 2;//There can only be one of these so just change their positions
							launcher.pos.y = (float)sf::Mouse::getPosition(window).y - launcher.launcherLength / 2;
							launcher.updateSprite();
							break;
						case(3):

							keys.emplace_back(Key((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y));
							applyTextures();
							break;
						case(4):
							endPoint.pos.x = (float)sf::Mouse::getPosition(window).x - endPoint.length / 2;
							endPoint.pos.y = (float)sf::Mouse::getPosition(window).y - endPoint.length / 2;
							endPoint.updateSprite();
							break;
						}
					}

					else {
						// std::cout << "New Plane at X: " <<
						// sf::Mouse::getPosition(window).x << " Y: " <<
						// sf::Mouse::getPosition(window).y << std::endl;
						if (!gameStarted) {//If game just started set the beam to the launchers position
							Beam =
								Laser(launcher.pos.x + 40.0f,
									launcher.pos.y + 40.0f); // 40.0f is launcher width / 2
							Beam.setAngle((float)launcher.angle + 180.0f, (float)launcher.pos.x + 40.0f,
								(float)launcher.pos.y + 40.0f);
							firstTime = false;
							gameStarted = true;
						}
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {//Delete the surface that was right clicked
					mouseVector.x = static_cast<float>(sf::Mouse::getPosition(window).x);
					mouseVector.y = static_cast<float>(sf::Mouse::getPosition(window).y);
					if (inEditorMode) {
						for (unsigned int i = 0; i < planes.size(); i++) {
							if (Vector::getMag(Vector::sub(mouseVector, planes.at(i).pos)) <
								20.0f) {//20.0f diameter circle around middle of plane to check for collision
								planes.erase(planes.begin() + i);//Remove the one  here
								applyTextures(true);
								break;
							}
						}
						std::cout << "RC " << mouseVector.x << " " << mouseVector.y << std::endl;
						for (unsigned int i = 0; i < keys.size(); i++) {
							if (Vector::getMag(Vector::sub(mouseVector, keys.at(i).pos)) < 20.0f) {
								keys.erase(keys.begin() + i);
								applyTextures();
								break;
							}
						}
					}
					else if (displayLevels) {
						for (int i = 0; i < levelsVector.size(); i++) {
							//std::cout << "i: " + i << std::endl;
							if (levelsVector.at(i).isClicked((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y)) {
								std::cout << "Removing " + levelsVector.at(i).spriteLocation << std::endl;
								Level::deleteLevel(levelsVector.at(i).spriteLocation);
								updateDisplayLevels(&font);
							}
							else {
								timer = clock();
							}
						}
					}
				}

				break;
			case(sf::Event::TextEntered):
				if (((evnt.text.unicode < 128 && evnt.text.unicode > 31) || evnt.text.unicode == 8) && typingNewLevel) {
					/*
					Bug where the user clicked enter after typing the name of the variable, then enter ascii code was put into string. It messed up string, and didn't
					allow to create a new level. Then I made a range to make sure the enter key is out of range. Only space bar ascii code and above.
					Since backspace is below 31, also make exception for it. Then make sure 'typingNewLevel' check other wise any key entered throughout the program
					is being put into the string.

					Make sure to have this enabled, the typing new level var.
					Otherwise we only make sure if ascii is below 128.
					So when the user clicks enter it puts that enter into the string too. Make sure ascii code is within range,
					expent space becuz it is below the characters
					Then check if the the last char is space and the current adding char is space. if so break out. No more than 1 space.
					Must do if statement inside because referencing the last element of the string can sometimes be undefined. Since the string is empty
					Then check if the backsapce was pressed and the string has chars in it. If so delete last char.
					If the backspace was pressed with 0 chars then it just adds a backspace char into the string, so we check after that to make sure it doesnt happen*/
					if (newLevelString.length() > 0) {
						if ((int)newLevelString[newLevelString.length() - 1] == (int)evnt.text.unicode && (int)evnt.text.unicode == 32) {
							break;
						}
					}
					if (evnt.text.unicode == 8 && typingLevelTextButton.spriteLocation.length() > 0) {
						newLevelString.pop_back();
						typingLevelTextButton.spriteLocation.pop_back();
						typingLevelTextButton.update();
					}
					else if (evnt.text.unicode != 8) {
						newLevelString += evnt.text.unicode;
						typingLevelTextButton.spriteLocation += evnt.text.unicode;
						typingLevelTextButton.update();
					}
				}

				break;
			}
		}
		if (isDisplayingSaved && ((clock() - timer) / (double)CLOCKS_PER_SEC * 1000 > 3500)) {
			isDisplayingSaved = false;
			saveTooltip.setString("CLICK 'H' FOR HELP! DOUBLE TAP 'S' TO SAVE LEVEL!");
		}
		window.clear(sf::Color::Black);
		switch (state) {
		case (inGame):
			if (gameStarted) {
				trail.getBeamPos(Beam);
				/*if (key.checkCollision(Beam)) {
					std::cout << "Key!" << std::endl;
					gotKeys = true;
				}*/
				for (int i = 0; i < keys.size(); i++) {//Loop through the keys and if beam hit it then set the keys 'isGotten' property to true.
					if (keys.at(i).checkCollision(Beam)) {
						keys.at(i).isGotten = true;
					}
				}
				keysGotten = 0;
				for (int i = 0; i < keys.size(); i++) {//This way when beam hits key the keysGotten is not incremented like 100 times
					if (keys.at(i).isGotten) {
						keysGotten++;
					}
				}

				if (endPoint.checkCollision(Beam)) {
					if (keysGotten == keys.size()) {//If all keys gotten then make game winnable
						gameWon = true;
					}
				}
				else {
					Beam.move();
				}
			}
			else if (!inEditorMode) {//If in game and not in editor mode then move the launcher's barrel to mouse
				launcher.setAngle((float)sf::Mouse::getPosition(window).x,
					(float)sf::Mouse::getPosition(window).y);
			}
			for (unsigned int i = 0; i < planes.size(); i++) { // Optimize where draw points goes. right now it is in the
			 // move() method.
				if (gameStarted) {
					if (planes.at(i).checkCollision(Beam)) {
						score++;
						changeScore();
					}
				}
				// planes.at(i).drawPoints();

				window.draw(planes.at(i).plane);
				window.draw(scoreText.rect);
				window.draw(scoreText.text);

				// window.draw(planes.at(i).point1);
			}
			window.draw(trail.draw()); // This order matters. Draw trail after the
			// surface so we can see the trail when laser
			// goes "through" surface as a prism
			if (Clock.getElapsedTime().asMilliseconds() > 80) {
				Clock.restart();
			}
			for (int i = 0; i < keys.size(); i++) {
				if (Clock.getElapsedTime().asMilliseconds() == 0) {
					keys.at(i).updateAnimation();
				}

				window.draw(keys.at(i).shape);
			}
			window.draw(endPoint.shape);

			// window.draw(Beam.beam);
			for (int i = 0; i < 2; i++) {
				window.draw(launcher.shapeArr[i]);
			}

			break;

		case (mainMenu):
			//Draw TitleText
			for (int i = 0; i < buttonsLength; i++) {
				window.draw(buttons[i].rect);
			}
			window.draw(currentLevelText);
			break;
		}
		if (inEditorMode) {
			for (int i = 0; i < editingButtonsLength; i++) {
				window.draw(editingButtons[i].rect);
			}
			cursorObject[currentEditingObject].rect.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x - cursorObject[currentEditingObject].rect.getSize().x / 2,
				sf::Mouse::getPosition(window).y - cursorObject[currentEditingObject].rect.getSize().y / 2));//Set the sprite to the cursor's position in the middle

			window.draw(cursorObject[currentEditingObject].rect);
		}

		// Display winnerText
		if (gameWon && state == inGame) {
			winnerText.setString("Won with a score of: " + scoreText.text.getString());
			window.draw(winnerText);
		}
		if (displayLevels) {
			window.draw(newLevelBtn.rect);
			window.draw(newLevelBtn.text);
			if (typingNewLevel) {
				window.draw(typingLevelTextButton.rect);
				window.draw(typingLevelTextButton.text);
			}

			for (unsigned int i = 0; i < levelsVector.size(); i++) {
				window.draw(levelsVector.at(i).rect);
				window.draw(levelsVector.at(i).text);
			}
		}
		if (displayUnsaveWarning) {
			window.draw(unsavedWorkText.rect);
		}
		if (displayMainHelp) {
			window.draw(mainHelpButton.rect);
		}
		if (displayEditorHelp) {
			window.draw(editorHelpButton.rect);
		}
		if (displayNoLevelSelected) {
			if (((clock() - timer) / (double)CLOCKS_PER_SEC * 1000 < 800)) {
				window.draw(noLevelSelectedButton.rect);
			}
			else {
				displayNoLevelSelected = false;
			}
		}
		window.draw(saveTooltip);

		window.display(); // Display the window
	}

	return 0;
}

void applyTextures(bool onlyPlanes) {//This is to ensure that the planes texture refresh. Vector reorganizes memory alot so each time a new one is added we must re apply textures. It randmoly just recreates the surfaces so the textures get taken off
	if (onlyPlanes) {
		for (int i = 0; i < planes.size(); i++) {
			planes.at(i).applyTexture();
		}
	}
	else {
		endPoint.applyTexture();
		launcher.applyTexture();
		/*	for (int i = 0; i < planes.size(); i++) {
				planes.at(i).applyTexture();
			}*/
		for (int i = 0; i < keys.size(); i++) {
			keys.at(i).applyTexture();
		}
	}
}
void changeScore() {
	scoreText.text.setString(std::to_string(score));
}

void updateDisplayLevels(sf::Font * font) {//Must pass in a pointer to the font because the font is declared in the main function and this is out of scope. For some reason cant make it global so I just passed it in as a pointer. and did not dereference becasue the applyTexture function needs a pointer anyways
	//List levels

	levelsVector.clear();//Clear otherwise buttons are being stacked upon each other each time.
	std::string path = "levels";
	int i = 1;
	for (const auto& entry : std::experimental::filesystem::directory_iterator(path)) {//Loop through everything in levels folder
		std::string levelName = entry.path().string().substr(7);//Cut off the "levels/" part
		levelName = levelName.substr(0, levelName.find(".txt"));//Go up until the .txt
		levelsVector.emplace_back(Button(70.0f, 800-(i * 50.0f), 100.0f, 40.0f, levelName));//Put it in the vector
		levelsVector.back().applyTexture(font);//Pass the main font as a pointer to the button. .back() Just referes to the latest added button
		i++;
	}

	newLevelBtn.pos.x = 20.0f;
	newLevelBtn.pos.y = 800 - (i * 60.0f);
	newLevelBtn.update();
	newLevelBtn.applyTexture(font);
	displayLevels = true;
}