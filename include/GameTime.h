#include <iostream>
#include <time.h>
#include <sstream>


class GameTime{
private:
	int MAXTIME;
	int elapsed_secs, remaining=200;
	time_t begin, end;
	bool endGame;
	std::ostringstream buff;

public:
	GameTime(){
		endGame = false;
	}

	void setTime(int max){
		MAXTIME= max;
	}

	void startTime(){
		time (&begin);
		endGame = false;
		elapsed_secs = 0;
	}

	std::string getTime(){
		 buff.str("");
		 time (&end);
		 elapsed_secs = int(difftime(end, begin));

		 remaining = MAXTIME - elapsed_secs;

		 buff << remaining;
		
		 return "Tempo restante " + buff.str();
	}

	bool isFinishTime(){
		if(elapsed_secs >= MAXTIME)
			return true;
		else
			return false;
	}

	bool isEndGame(){
		return endGame;
	}

	void setEndGame(bool status){
		endGame = status;
	}

};