#include "Level.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstring>
#include "Surface.h"
#include "Launcher.h"
#include "Key.h"
#include "Endpoint.h"
Level::Level(std::string levelPath) : path(levelPath) {
	path = "levels/" + levelPath;//Go to levels directory
	std::ifstream level(path);
	

	enum TYPEENUM { surfaceE, surfaceFixedE, launcherE,keyE, endPointE };
	int type;
	float x;
	float y;
	float angle;
	extern std::vector<Surface> planes;//This is because we will have to use 'extern' in each method and that is time consuming
	extern std::vector<Key> keys;
	extern Laser Beam;
	extern Launcher launcher;
	extern Endpoint endPoint;
	planesPointer = &planes;
	launcherPos = &launcher.pos;//Set the pointer for launcher position to the memory address of launcher's vector
	keysPointer = &keys;
	endpointPos = &endPoint.pos;
	clearLevel();
	while (level >> type >> x >> y >> angle) {
		//std::cout << type << ", " << x << ", " << y << ", " << angle << ", " << std::endl;
		//SurfaceF = 0, SurfaceFixed = 1,Launcher = 2, Key = 3, Endpoint = 4
		switch (type) {
			case(surfaceE):
				planes.emplace_back(Surface(x, y, angle, true));//Boolean tells whether it is rotatable | Emblace back instead of push back to make the instance inside the new memory location right away
				break;
			case(surfaceFixedE):
				planes.emplace_back(Surface(x, y, angle, false));
				break;
			case(launcherE): 
				launcher = Launcher(x, y);
				break;
			case(keyE):
				keys.emplace_back(Key(x, y));
				//key = Key(x, y);
				break;
			case(endPointE):
				endPoint = Endpoint(x, y);
				break;
		}
	}
}
 
void Level::clearLevel(){
	planesPointer->clear();//Same as (*planesPointer).clear();

////Could use .c_str(); LOL
	////path.size() is reffering to a c++ string
	//char* pathArr = new char[path.length() + 1];//Create a new pointer to a piece of memory that stores integers. Add one more space for the terminating character


	////Allocate the amount of spaces as there are characters in string
	//for (int i = 0; i < path.size(); i++) {//Go through the size of the array or just go through string length
	//	*(pathArr + i) = path.at(i);//Give the precedence to incrementing pointer that dereference it. Put in the charactar at that memory spot
	//}
	//pathArr[path.size()] = '\0';//End the peices of memory with this to know its the end of the array
	////Converted string to char[] array
 //	if (std::remove(pathArr) != 0) {//If can't find the path
	//	std::cout << "Not working" << std::endl;
	//}

	//else {
	//	std::cout<< "Worked" << std::endl;
	//	std::ofstream levelFile(pathArr);
	//}
	//std::cout << "here";
 //	delete[] pathArr;
	//pathArr = NULL;

}
void Level::newLevel(std::string& newLevelName) {

	
	levelsPath = "levels";
	newLevelPath.clear();
	newLevelPath = "levels/" + newLevelName;
	bool isDuplicate = false;
	for (const auto& entry : std::experimental::filesystem::directory_iterator(levelsPath)) {//Loop through everything in levels folder. if a level name matches current level display warning and break loop
		levelName = entry.path().string().substr(7);//Cut off the "levels/" part
		if (newLevelName == levelName) {
			std::cout << "COLLISION" << std::endl;
			isDuplicate = true;
			break;
		}
	}
	if (!isDuplicate) {
		std::cout << "Path final:" << std::endl;
		std::cout << newLevelPath << std::endl;
		std::ofstream levelFile(newLevelPath.c_str());//Create new level.txt
		levelFile << '2' << " " << 50.0f << " " << 50.0f << " " << '0' << "\n";//Dereferncing the pointer in two syntaxes 
		levelFile << '3' << " " << 50.0f << " " << 50.0f << " " << '0' << "\n";//Second way
		levelFile << '4' << " " << 50.0f << " " << 50.0f << " " << '0' << "\n";
	}

}
void Level::saveLevel() {
	extern std::vector<Surface> planes;
	if (std::remove(path.c_str()) != 0) {//Delete the level.txt file
		std::cout << "Error Deleting level.txt" << std::endl;
	}
	else {
		std::ofstream levelFile(path.c_str());//Create new level.txt
		for (unsigned int i = 0; i < planesPointer->size(); i++) {
			char type = planesPointer->at(i).rotatable ? '0' : '1';//Level syntax has 0 and 1 to tell if it is fixed or not
			
			levelFile << type << " " << planesPointer->at(i).pos.x << " " << (float) planesPointer->at(i).pos.y << " " << planesPointer->at(i).angle << "\n";
		}
		levelFile << '2' << " " << (*launcherPos).x << " " << (*launcherPos).y << " " << '0' << "\n";//Dereferncing the pointer in two syntaxes 
		
		for (unsigned int i = 0; i < keysPointer->size(); i++) {
			levelFile << '3' << " " << keysPointer->at(i).pos.x << " " << keysPointer->at(i).pos.y << " " << '0' << "\n";//Second way
		}
		levelFile << '4' << " " << endpointPos->x << " " << endpointPos->y << " " << '0' << "\n";
	}
}

void Level::deleteLevel(std::string& deleteLevelName) {
	std::string deleteLevelPath = "levels/" + deleteLevelName + ".txt";
	if (std::remove(deleteLevelPath.c_str()) != 0) {//Delete the level.txt file
		std::cout << "Error Deleting " + deleteLevelPath + " " << std::endl;
	}
}