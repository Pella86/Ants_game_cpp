/*
 *  GameEngine.h
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 21.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>
#include <algorithm>
#include "Color.h"
#include "GameElements.h"


class GameEngine{
private:
	std::vector< Player > players;			//players in game
	BBox bbox;								//bounding box of the game
	int food_n;								//how many foods
	std::vector < Food > food_list;			//food array
	std::vector < BaseObject* > obj_list;	//vector containing all the objects
	
public:
	GameEngine();
	
	//getters & setters
	std::vector < BaseObject* > const & get_obj_list() {return obj_list;}
	
	//actions
	Food spawn_food() const;
	void check_food();			//check if there is a food station exhausted
	void update_obj_list();
	void action();				//main loop of the game, called by the rendering loop
};


#endif