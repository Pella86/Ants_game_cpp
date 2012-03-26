/*
 *  GameEngine.cpp
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 21.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */

#include "GameEngine.h"


Random_number randn_ge;


/*****************
 * CLASS GAME ENGINE
 ****************/



GameEngine::GameEngine(){
	//Initialize players
	players.push_back(Player("PGreen"));			//0

	players.push_back(Player("PRed"));				//1
	players[1].set_color() = Color(1., 0., 0., 0.);

	players.push_back(Player("PCyan"));				//2
	players[2].set_color() = Color(0., 1., 1., 0.);

	players.push_back(Player("PMagenta"));			//3
	players[3].set_color() = Color(1., 0., 1., 0.);

	players.push_back(Player("PYellow"));			//4
	players[4].set_color() = Color(1., 1., 0., 0.);

	//Initialize bounding box
	bbox = BBox(V2(0.0,0.0), 100.);
	
	//initialize the nest position
	for (IT_Player it_player = players.begin(); it_player != players.end(); ++it_player) {
		double boxsize = bbox.get_size() - 10;
		double randx = randn_ge.get(-boxsize,boxsize);
		double randy = randn_ge.get(-boxsize,boxsize);
		it_player->set_nest().player = &*it_player;
		it_player->set_nest().set_position() = V2(randx, randy);
		it_player->set_nest().update_spawning_point();
		it_player->set_nest().set_color() = it_player->get_color();
		while (!intersect_circle(it_player->set_nest().get_position(), bbox.get_position(), bbox.get_size())) {
			double randx = randn_ge.get(-boxsize,boxsize);
			double randy = randn_ge.get(-boxsize,boxsize);
			it_player->set_nest().set_position() = V2(randx, randy);
			it_player->set_nest().update_spawning_point();
		}
	}
	
	//initialize food
	food_n = 20;
	for (int i = 0; i < food_n; ++i) {
		Food spawned = spawn_food();
		food_list.push_back(spawned);
	}
}

Food GameEngine::spawn_food() const {
	Food food = Food();
	
	double boxsize = bbox.get_size() -10;
	double randx = randn_ge.get(-boxsize, boxsize);
	double randy = randn_ge.get(-boxsize, boxsize);
	
	food.set_position() = V2(randx, randy);
	bool intersect_obj = false;

	for (IT_cbObjptr it_objptr = obj_list.begin(); it_objptr != obj_list.end(); ++it_objptr) {
		if (intersect_circle(food.get_position(), (*it_objptr)->get_position(), (*it_objptr)->get_size())) {
			intersect_obj = true;
		}
	}
	
	while (!intersect_circle(food.get_position(), bbox.get_position(), bbox.get_size())
		   or intersect_obj) {
	repeat_food_place:
		randx = randn_ge.get(-boxsize, boxsize);
		randy = randn_ge.get(-boxsize, boxsize);
		food.set_position() = V2(randx, randy);
		
		intersect_obj = false;
		for (IT_cbObjptr it_objptr = obj_list.begin(); it_objptr != obj_list.end(); ++it_objptr) {
			if (intersect_circle(food.get_position(), (*it_objptr)->get_position(), (*it_objptr)->get_size())) {
				intersect_obj = true;
				goto repeat_food_place;
			}
		}
		
	}
	return food;
}


bool is_food_exhausted(Food const& food){
	if (food.get_resources() <= 0) {
		return true;
	}
	else {
		return false;
	}
}


void GameEngine::check_food(){
	//spawn a new food if find one exhausted
	for (IT_Food it_food = food_list.begin(); it_food != food_list.end(); ++it_food) {
		if (it_food->get_resources() <= 0) {
			food_list.push_back(spawn_food());
		}
	}
	
	//remove the one exhausted
	food_list.erase(std::remove_if(food_list.begin(), food_list.end(), is_food_exhausted), food_list.end());
}





void GameEngine::update_obj_list(){
	//it clears the previous object list and build an other one for all the obj in the scene
	obj_list.clear();
	//scroll through player
	for (IT_Player it_player = players.begin(); it_player != players.end(); ++it_player) {
		
		//append the nest
		BaseObject *bptr = &it_player->set_nest();
		obj_list.push_back(bptr);
		
		//append the ants
		for ( IT_Ant it_ant = it_player->set_nest().set_ants().begin() ; it_ant != it_player->set_nest().set_ants().end(); ++it_ant) {
			Ant & ant = *it_ant;
			BaseObject *bptr = &ant;
			obj_list.push_back(bptr);
		}
		
		//append the trails
		for (IT_Trail it = it_player->set_trails().begin(); it != it_player->set_trails().end(); ++ it) {
			Trail & trail = *it;
			BaseObject *bptr = &trail;
			obj_list.push_back(bptr);
		}
	}
	
	//append the food
	for (IT_Food it_food = food_list.begin(); it_food != food_list.end(); ++it_food) {
		Food & food = *it_food;
		BaseObject *bptr = &food;
		obj_list.push_back(bptr);
	}
}

//auxiliary function for remove_if in ant vector
bool is_dead(Ant const& ant){
	if (ant.get_life() < 0) {
		return true;
	}
	else {
		return false;
	}
}

void GameEngine::action(){
	//player indipendent updates	
	check_food();
	
	//player dependet updates
	for (IT_Player it_player = players.begin(); it_player != players.end(); ++it_player) {
		std::cout<< "---- Player: " << *it_player << std::endl;
		//spawn ants
		if (it_player->get_nest().get_resources() >= Ant().get_cost()) {
			it_player->set_nest().spawn_ant();
		}
		
		//update the trails
		if (it_player->get_trails().size() > 0) {
			it_player->update_trails();
		}
		
		//remove died ants
		std::vector<Ant>& ants = it_player->set_nest().set_ants(); //reference to have a shorter name
		ants.erase(remove_if(ants.begin(), ants.end(), is_dead ), ants.end());
	}
		
	update_obj_list();
	
	//real action
	//scroll the players
	for (IT_Player it_player = players.begin(); it_player != players.end(); ++it_player) {
		//scroll the ants
		for (IT_Ant it_ant = it_player->set_nest().set_ants().begin();it_ant != it_player->set_nest().set_ants().end(); ++it_ant) {
			V2 original_pos = it_ant->get_position(); //store the position in case is out of bounding box
			
			//ant action
			it_ant->make_move(obj_list);
			
			//Create a trail
			Trail new_trail = it_ant->put_trail();
			
			//   check if the trail is a valid trail
			if (new_trail.get_invalid_trail() != 1 ) {
				it_player->set_trails().push_back(new_trail);
			}
			
			//check if ant is inside the bounding box
			if (!intersect_circle(it_ant->get_position(), bbox.get_position(), bbox.get_size())) {
				it_ant->set_position() = original_pos;
			}
		}
	}
	update_obj_list();
	
}


