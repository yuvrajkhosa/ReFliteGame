#pragma once
#include "Surface.h"
#include "Key.h"
#include <vector>
#include <fstream>
#include "Launcher.h"
#include "Vector.h"


class Level {
private:
	std::string path;//String to the path of the level.txt
	std::vector<Surface>* planesPointer;//A pointer to the planes vector. A pointer so it doesn't take up too much memoery and we dont want to copy all of the surfaces
	std::vector<Key>* keysPointer;
	Vector* launcherPos;//Pointer to the location of the launcher's position because the user can change it half way through the the game

	Vector* endpointPos;
	std::string levelsPath;
	std::string newLevelPath;
	std::string levelName;
	
public:
	Level(std::string levelPath);
	Level() = default;
	void clearLevel();
	static void deleteLevel(std::string& deleteLevelName);//References to avoid memory consumption
	void newLevel(std::string& newLevelName);
	void saveLevel();

};