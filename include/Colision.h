#include <glm/glm.hpp>
#include <iostream>
#include <vector>


class Colision{
private:
	std::vector<glm::vec3> objects;
	float boxrange;

public:
	Colision(){
		boxrange = 2;
	}

	void addObject(glm::vec3 obj){
		objects.push_back(obj);
	}

	void start(){
		// update algoritm
	}

	bool isColision(glm::vec3 camera){
		for(int i=0; i< objects.size(); i++){
			if((objects.at(i).x - boxrange) <= camera.x && (objects.at(i).x + boxrange) >= camera.x
                && (objects.at(i).z  - boxrange) <= camera.z && (objects.at(i).z + boxrange) >= camera.z ){
	            return true; 
	            std::cout<<"-----------------> BATEU!!!\n";	         
       		}
       		else{
       			std::cout<< "----------------->  N BATEU\n";       			
       		}
		}
		std::cout<<"NAO BATEU!!!\n";
        return false;
	}
};