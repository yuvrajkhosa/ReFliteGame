#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <string>
#include "Vector.h"

class Button {
private:
	float width;
	float height;

public:
	Vector pos;
	sf::Text text;
	sf::Texture texture;
	sf::RectangleShape rect;
	std::string spriteLocation;
	Button(float _x, float _y, float _width, float _height, std::string _textureAddress);

	void applyTexture();
	bool isClicked(float _mouseX, float mouseY);
	







};
