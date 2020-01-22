#include "Trail.h"
#include <algorithm>
#include "SFML/Graphics/VertexArray.hpp"

Trail::Trail() {
	lines.setPrimitiveType(sf::PrimitiveType::LinesStrip);
	
	lines.resize(2);
	


}

sf::VertexArray Trail::draw() {
	return(lines);
}

void Trail::getBeamPos(Laser& beam) {
	if (firstCheck) {
		firstCheck = false; 
		lines[0].position = sf::Vector2f(beam.pos.x, beam.pos.y);
		lines[0].color = sf::Color::Red;
		//std::cout << "Pos x: " << beamPos.x

	}
	
	lines[lines.getVertexCount() - 1].position = sf::Vector2f(beam.pos.x, beam.pos.y);//Constantly sets the last vertex to beam pos so line follow beam
	lines[lines.getVertexCount() - 1].color = sf::Color::Red;
}

void Trail::newVertex(float x, float y) {
	
	lines.resize(lines.getVertexCount() + 1);
	lines[lines.getVertexCount() - 1].position = sf::Vector2f(x, y);//This part where we  give it the current X and Y as parameters are needed because sometimes when it creates the new vertex ( with lines.resize() ), it makes the position to (0, 0) by default so lines jumps to the top left before it snaps back to the beam when it is set to beam position in getBeamPos() which happens after this method                                                            
	lines[lines.getVertexCount() - 1].color = sf::Color::Red;
	
	
	
}

void Trail::clearArr() {
	lines.clear();
	lines.resize(2);
	firstCheck = true;

}