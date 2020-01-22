#pragma once
#include "SFML/Graphics/VertexArray.hpp"

#include "Laser.h"
#include <iostream>
class Trail {
public:
		static sf::VertexArray lines;
		Trail();
		sf::VertexArray draw();
		static void newVertex(float x, float y);
		void clearArr();
		void getBeamPos(Laser& beam);//Must get pos because laser is using polar coordinate system.
		bool firstCheck = true;
};
