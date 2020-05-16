#pragma once
#include <vector>
#include "Surface.h"
#include <fstream>
#include "Launcher.h"
#include "Vector.h"


class Level {
private:
	std::string path;//String to the path of the level.txt
	std::vector<Surface>* planesPointer;//A pointer to the planes vector. A pointer so it doesn't take up too much memoery and we dont want to copy all of the surfaces
	Vector* launcherPos;//Pointer to the location of the launcher's position because the user can change it half way through the the game
	Vector* keyPos;
	Vector* endpointPos;
	
public:
	Level(std::string levelPath);
	void deleteLevel();
	void saveLevel();

};