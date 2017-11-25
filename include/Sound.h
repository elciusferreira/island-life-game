#include <SFML/Audio.hpp>
#include <thread>  
#include <iostream>

extern bool canSetMusic = false;
extern std::string path= "";

void controlPlayer(){
	sf::SoundBuffer buffer;
	sf::Sound sound;

	while(true){
		if(canSetMusic){
			if(buffer.loadFromFile(path)){
				sound.setBuffer(buffer);
				sound.setLoop(true);
				sound.play();
				canSetMusic = false;
			}
		}
	}
}

class Sound{
private:

public:
	const std::string begin = "resources/sounds/intro.wav";

	Sound(){
		std::thread run (controlPlayer);
		run.detach();
	}

	void setMusic(std::string newPath){
		if(path.compare(newPath)){
			path = newPath;
			canSetMusic = true;
		}
	}

};

