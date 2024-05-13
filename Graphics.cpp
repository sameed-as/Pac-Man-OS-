#include"SFML/Graphics.hpp"
#include"SFML/Audio.hpp"
#include"SFML/Window.hpp"
#include"SFML/System.hpp"
#include<iostream>
#include<fstream>
#include<stack>
#include<sstream>
using namespace std;
using namespace sf;
class Graphics
{
private:

public:
	string wind_name;
	VideoMode res;//resolution
	//bool fullscreen;
	//bool vsync;
	unsigned frameRateLimit;
	//ContextSettings contextSettings;
	//vector<VideoMode> videoModes;

	Graphics() {
        this->wind_name = "DEFAULT";
        this->res = VideoMode::getDesktopMode();
        //this->fullscreen = false;
        //this->vsync = false;
        this->frameRateLimit = 120;
        //this->contextSettings.antialiasingLevel = 0;
        //this->videoModes = VideoMode::getFullscreenModes();
    }
    void saveToFile(const string path) {
        ofstream ofs(path);
        if (ofs.is_open()) {
            ofs << this->wind_name;
            ofs << this->res.width << " " << this->res.height;
            //ofs << this->fullscreen;
            ofs << this->frameRateLimit;
            //ofs << this->vsync;
            //ofs << this->contextSettings.antialiasingLevel;
        }
        ofs.close();

    }

    void loadFromFile(const string path) {
        ifstream ifs(path);

        if (ifs.is_open()) {
            getline(ifs, this->wind_name);
            ifs >> this->res.width >> this->res.height;
            //ifs >> this->fullscreen;
            ifs >> this->frameRateLimit;
            //ifs >> this->vsync;
            //ifs >> this->contextSettings.antialiasingLevel;
        }
        ifs.close();

    }

};


