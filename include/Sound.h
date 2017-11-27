#include <SFML/Audio.hpp>
#include <thread>  
#include <iostream>

extern bool canSetMusic = false;
extern std::string path= "";
extern int timeStart= 0;

void controlPlayer(){
	sf::SoundBuffer buffer;
	sf::Sound sound;

	while(true){
		if(canSetMusic){
			if(buffer.loadFromFile(path)){
				sound.setBuffer(buffer);
				sound.setLoop(true);
				sound.play();
				sound.setPlayingOffset(sf::seconds(timeStart));
				canSetMusic = false;
			}
		}
	}
}

class Sound{
private:

public:
	const std::string begin = "resources/sounds/begin.wav";
	const std::string fail = "resources/sounds/fail.wav";
	const std::string success = "resources/sounds/success.wav";
	const std::string game = "resources/sounds/game.wav";

	Sound(){
		std::thread run (controlPlayer);
		run.detach();
	}

	void setMusic(std::string newPath, int startTime){
		if(path.compare(newPath)){
			path = newPath;
			timeStart = startTime;
			canSetMusic = true;
		}
	}

};

