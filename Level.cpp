#include "Level.h"
#include <vector>
#include "Surface.h"
#include "Launcher.h"
#include "Key.h"
#include "Endpoint.h"
Level::Level(std::string levelPath){
	std::ifstream level(levelPath);
	enum TYPEENUM { surfaceE, surfaceFixedE, launcherE,keyE, endPointE };
	int type;
	float x;
	float y;
	float angle;
	extern std::vector<Surface> planes;
	extern Laser Beam;
	extern Launcher launcher;
	extern Key key;
	extern Endpoint endPoint;
	while (level >> type >> x >> y >> angle) {
		//std::cout << type << ", " << x << ", " << y << ", " << angle << ", " << std::endl;
		//SurfaceF = 0, SurfaceFixed = 1,Launcher = 2, Key = 3, Endpoint = 4
		switch (type) {
			case(surfaceE):
				planes.push_back(Surface(x, y, angle, true));//Boolean tells whether it is rotatable
				break;
			case(surfaceFixedE):
				planes.push_back(Surface(x, y, angle, false));
				break;
			case(launcherE):
				launcher = Launcher(x, y);
				break;
			case(keyE):
				key = Key(x, y);
				break;
			case(endPointE):
				endPoint = Endpoint(x, y);
				break;
		}
	}
}