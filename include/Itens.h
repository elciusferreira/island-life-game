#include <glm/glm.hpp>
#include <iostream>


class Itens{
private:
	glm::vec3 Position;
	bool visited;
	float boxrange;

public:

	Itens(glm::vec3 pos){
		this->Position = pos;
		this->visited = false;
		this->boxrange = 4;
	}

	bool isInside(glm::vec3 camera){
		if((Position.x - boxrange) <= camera.x && (Position.x + boxrange) >= camera.x
                && (Position.z  - boxrange) <= camera.z && (Position.z + boxrange) >= camera.z ){
            std::cout<<"TA NO RANGE!!!\n";
            return true;     
        }
        std::cout<<"NAO TA NO RANGE!!!\n";
        return false;
	}

	bool isLooking(float look){
		if(-1 <= look && -0.75 >= look ){
            std::cout<<"TA OLHANDO!!!\n";
            return true;     
        }
        std::cout<<"NAO TA OLHANDO!!!\n";
        return false;
	}

	void setVisited(bool v){
		visited = v;
	}

	bool getVisited(){
		return visited;
	}

};