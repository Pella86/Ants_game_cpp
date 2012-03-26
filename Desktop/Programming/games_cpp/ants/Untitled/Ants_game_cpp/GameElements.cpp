/*
 *  GameElements.cpp
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 13.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */

#include "GameElements.h"

Random_number rand_num;


/************
 *TIME CONTROLLER CLASS
 ************/

bool TimeController::can_run(){
	// check if is passed enough time
	if ((std::clock() - last_move) > interval) {
		//reset the state
		last_move = std::clock();
		return true;
	}
	else {
		return false;
	}
}

/************
 *BASE OBJECT CLASS
 ************/

//initialize the static member
int BaseObject::_id_count = 0;



void BaseObject::increment_id(){
	_id_count++;
}

void BaseObject::decrement_id(){
	_id_count--;
}

BaseObject::BaseObject(){
	position = V2();
	obj_type = obj_none;
	size = 1.;
	color = Color();
	increment_id(); //each time a object is created increase the id_count
	unique_id = _id_count;	
}

BaseObject::BaseObject(BaseObject const& copy_of_me){
	this->operator=( copy_of_me );
	increment_id(); //each time a object is copied increase the id_count
}

/************
 *TRAIL CLASS
 ************/

Trail::Trail(){
	//baseObject settings
	obj_type = obj_trail;
	size = 0.2;
	color = Color(1.,1.,1.,0.);
	
	//class specific settings
	intensity_decrease_t = TimeController(50000);
	
	intensity = 10.;
	state = s_search;
	invalid_trail = 0;
	player = NULL;
}

void Trail::update_trail(){
	if (intensity_decrease_t.can_run()) {
		intensity -= 1;
	}
}

/************
 *ANT CLASS
 ************/

Ant::Ant(){
	//baseObject settings
	obj_type = obj_ant;
	size = 0.75;
	
	direction = V2(1., 0.);
	
	attack = 5.;
	defence = 30.;
	life = 100.;

	cost = 100.;
	sight_r = 10.;
	
	speed = size + 0.1;
	
	move_t = TimeController(5000.);
	
	state = s_search;
	
	harvest_t = TimeController(100.);
	
	reservoir = 0.;
	reservoir_limit = 50.;
	
	color = Color(1.,0.,0.,0.);
	
	trail_placing_t = TimeController(50000);
	
	player = NULL;
	nest = NULL;
	g_eng = NULL;
}

bool Ant::reservoir_full() const{
	if (reservoir >= reservoir_limit) {
		return true;
	}
	else {
		return false;
	}
}

bool Ant::reservoir_empty() const {
	if (reservoir == 0.) {
		return true;
	}
	else{
		return false;
	}
}

void Ant::harvest(Food& food){
	if (!(reservoir_full())) {
		if (harvest_t.can_run()) {
			reservoir += 1;
			food.set_resources() -= 1;
		}
	}
	else {
		state = s_bring_food_nest;
	}
}


void Ant::attack_ant(Ant& vs_ant){
	//calculate the damage
	//random number 30% higher or lower than the actual attack
	double cdamage = rand_num.get(attack - 0.3*(attack), attack + 0.3*(attack));
	//calculate the defense is a percentage of the attack
	double cdefence = rand_num.get(0., defence);
	//calculate the actual damage: damage - %defence
	cdamage = cdamage-(cdamage*(cdefence/100.));
	vs_ant.set_life() -= cdamage;
}

Trail Ant::put_trail(){
	Trail new_trail;
	if (state == s_bring_food_nest and trail_placing_t.can_run() ) {
		new_trail.set_state() = state;
		new_trail.set_position() = position;
		new_trail.set_player() = player;
		return new_trail;
	}
	/* //when attacking set red trails
	else if (state == s_attacking){
		new_trail.set_state() = state;
		new_trail.set_position() = position;
		new_trail.set_player() = player;
		new_trail.set_color() = Color(1., 0., 0., 0.);
		return new_trail;
	}
	*/

	else {
		new_trail.set_invalid_trail() = 1;
		return new_trail;
	}
}

std::vector<BaseObject *> Ant::look(std::vector < BaseObject *> obj_list) const {
	//Task: take an object vector and return a new vector containing the objects
	//		in range of sight_r
	
	std::vector<BaseObject *> return_objects;
	
	for (IT_bObjptr it = obj_list.begin(); it != obj_list.end(); ++it) {
		if ( this->get_id() != (*it)->get_id()) { //avoid checking the same obj
			if (intersect_circle(position, (*it)->get_position(), sight_r)) {
				return_objects.push_back(*it);
			}	
		}
	}
	return return_objects;
}


void Ant::make_move(std::vector<BaseObject * > obj_list){
	//Task: decide what the ant do and perform the action
	//		refactor when possible
	
	std::vector<BaseObject * > obj_seen = look(obj_list); // the objects in range
	
	//declaration of pointers to be passed finished the decision loop
	BaseObject* target_obj = NULL;
	Ant* ant_to_attack = NULL;
	Food* food_target = NULL;
	Trail* trail_target = NULL;
	
	//scan through the objects in range
	for (IT_bObjptr obj_ptr = obj_seen.begin(); obj_ptr != obj_seen.end(); ++obj_ptr) {
		target_obj = (*obj_ptr); //actual object
		
		//if collide with  the object
		if(intersect_circle(get_position(), (*obj_ptr)->get_position(), (*obj_ptr)->get_size())){
			Ant& ant_tmp = static_cast<Ant&> (*target_obj);

			switch ((*obj_ptr)->obj_type) {
				case obj_ant:
					//if the object is an ant and is from the different player ATTACK!
					if (ant_tmp.player != player) {
						state = s_attacking;
						ant_to_attack = &ant_tmp;
					}
					else {
						state = s_search;
					}
					
					break;
				case obj_food:
					if (state != s_bring_food_nest) {
						//if is a food type and we are not bringing back things to the  nest
						Food& food_tmp = static_cast<Food&> (*(*obj_ptr));
						if (food_tmp.get_resources() > 0) {
							state = s_harvest;
							food_target = &food_tmp;
						}
						else {
							state = s_search;
						}						
					}
					else {
						s_search;
					}

					break;
				case obj_nest:
					if (!reservoir_empty()) {
						Nest& tmp_nest = static_cast<Nest&> (*(*obj_ptr));
						//if the ant cross the nest where the ant come from
						if (tmp_nest.player == player) {
							state = s_discharge;
						}
						else {
							state = s_search;
						}
						
					}
					else {
						//if the reservoir is emptied ant can leave the nest
						state = s_search;
					}

					break;
				case obj_trail:
					//do nothing
					break;
				default:
					s_search;
					break;
			}
			//break the loop if you find something that intersects
			break;

		}
		// if sees the object
		else{
			if ((*obj_ptr)->obj_type == obj_food and reservoir < (reservoir_limit - 5)) {
				state = s_goto_food;
				target_obj = (*obj_ptr);
			}
			else if(reservoir > reservoir_limit - 5){
				state = s_bring_food_nest;
			}
			else if((*obj_ptr)->obj_type == obj_ant){
				Ant& ant_tmp = static_cast<Ant&> (*(*obj_ptr));
				if (ant_tmp.state == s_bring_food_nest and ant_tmp.player != player) {
					state = s_attack;
					ant_to_attack = &ant_tmp;
				}
			}
			else if((*obj_ptr)->obj_type == obj_trail) {
				Trail& trail_tmp = static_cast<Trail&> (*(*obj_ptr));
				if (trail_tmp.player == player) {
					state = s_follow_trail;
					trail_target = &trail_tmp;
				}
			}
			else {
				state = s_search;
			}
		}
	}
	
	// action to do:
	// if the ant sees objects
	if (obj_seen.size() > 0) {
		//switch variables
		V2 original_position;
		int counter;
		std::vector< Trail* > trails;
		
		switch (state) {
			case s_goto_food:
				go_to(target_obj->get_position());
				break;
			case s_harvest:
				//why some times is null?
				if (food_target != NULL) {
					harvest(*food_target);
				}
				else {
					move_random();
				}				
				break;
			case s_bring_food_nest:
				go_to(nest->get_position());
				break;
			case s_search:
				original_position = position; //save original position
				move_random();
				// if the ant is moved and it intersect an other object
				counter = 0; //cycle limit
				while (intersect_circle(position, target_obj->get_position(), target_obj->get_size())) {
					position = original_position;
					speed += counter; 
					move_dir_without_limit();					
					counter++;
				}
				//reset speed
				speed = size + 0.1;
				break;
			case s_discharge:
				nest->set_resources() += reservoir;
				reservoir = 0;
				state = s_search;				
				break;
			case s_follow_trail:
				
				//recurse over object seen and build a trail vector
				for (IT_bObjptr obj_ptr = obj_seen.begin(); obj_ptr != obj_seen.end(); ++obj_ptr) {
					if ((*obj_ptr)->obj_type == obj_trail){
						trails.push_back(static_cast<Trail*> (*obj_ptr) );
					}
				}
				//if we have more than 1 trail than choose the more distant from the nest
				if (trails.size() > 1) {
					Trail* best_trail = trails[0];
					double best_d = nest->get_position().distance(best_trail->get_position());
					
					for (it_trailptr trailit = trails.begin(); trailit != trails.end(); ++trailit) {
						double d = nest->get_position().distance((*trailit)->get_position());
						if (d > best_d) {
							best_trail = *trailit;
							best_d = d;
						}
					}
					//go to the calculated best trail
					go_to(best_trail->get_position());
				}
				else if(trails.size() == 1){
					go_to(trails[0]->get_position());
				}
				else {
					move_random();
				}
				break;
			case s_attacking:
				//it is crossing the ant
				if (ant_to_attack->obj_type == obj_ant) {
					attack_ant(*ant_to_attack);
					state = s_search;
				}
				break;
			case s_attack:
				if (ant_to_attack != NULL) {
					go_to(ant_to_attack->get_position());
				}
				break;
			default:
				move_random();
				break;
		}
	}
	else {
		//action to do if they see no objects
		move_random();
	}
}

void Ant::go_to_food(Food& food){
	go_to(food.get_position());
}

void Ant::rotate_dir(double alpha){
	double radalpha = deg2rad(alpha);
	direction = direction.rotate(radalpha);
}

void Ant::move_dir(){
	if (move_t.can_run()) {	
		position += direction*speed;
	}
}

void Ant::move_dir_without_limit(){
	//function used when the ants get stuck on an object
	double randdir = rand_num.get(-180,180);
	rotate_dir(randdir);	
	position += direction*speed;
}

void Ant::move_random(){
	double randdir = rand_num.get(-90,90);
	rotate_dir(randdir);
	move_dir();
}

void Ant::go_to(V2 const& target_pos){
	V2 v2traspose = target_pos - position;
	if (!v2traspose.is_null()) {
		v2traspose.normalize();
		V2 v2polar = v2traspose.convert_in_polar();
		V2 dirpolar = direction.convert_in_polar();
		dirpolar.y() = v2polar.y();
		direction = dirpolar.convert_in_cartesian();
		
	}
	move_dir();
}

bool Ant::is_dead() const {
	if (life <= 0) {return true;}
	else {return false;}
}

/************
 *NEST CLASS
 ************/

Nest::Nest(){
	obj_type = obj_nest;
	resources = 1000.;
	spawn_t = TimeController(200000.);
	spawning_point = position + V2(0.,size + 1.);
	size = 5.;
}

void Nest::spawn_ant(){
	Ant spawned_ant; //dinamic memory allocation heeeelps
	if (spawn_t.can_run()) {
		if (resources > spawned_ant.get_cost()) {
			spawned_ant.set_position() = spawning_point;
			spawned_ant.nest = this;
			spawned_ant.player = player;
			spawned_ant.set_color() = player->get_color() + Color(0.5, 0.5, 0.5, 0.);
			ants.push_back(spawned_ant);			
			resources -= spawned_ant.get_cost();
		}
	}
}

void Nest::update_spawning_point(){
	spawning_point = position + V2(0., size+ 1.);
}

/************
 *FOOD CLASS
 ************/
Food::Food(){
	obj_type = obj_food;
	resources = 500.;
	position = V2(0,0);
	color = Color(0.,0.,1.,0.);
	size = 2.;
}

/************
 *BBOX CLASS
 ************/

BBox::BBox(){
	position = V2();
	size = 100.;
}

BBox::BBox(V2 pos, double s){
	position = pos;
	size = s;
}


/*****************
 * CLASS PLAYER
 ****************/



Player::Player(){
	nome = "Hallo";
	color = Color( 0., 1., 0., 0.);
	nest = Nest();
}


Player::Player(std::string n){
	nome = n;
	color = Color( 0., 1., 0., 0.);
	nest = Nest();
}


//auxiliary function to determine if a trail is consumed or not

bool is_consumed(Trail const & trail){
	if (trail.get_intensity() < 0) {
		return true;
	}
	else {
		return false;
	}

}

void Player::update_trails(){
	for (IT_Trail it_trailptr = trails.begin(); it_trailptr != trails.end(); ++it_trailptr) {
		it_trailptr->update_trail();
	}
	trails.erase(std::remove_if(trails.begin(),trails.end(),is_consumed),trails.end());
}


/*****************
 * CLASS OSTREAM
 ****************/

std::ostream& operator<<(std::ostream &outs, Trail const& t){
	outs << "position: " << t.get_position() << "int: " 
	<< t.get_intensity() << " state: " << t.get_state();
	return outs;
}


std::ostream& operator<<(std::ostream& outs, Player const& t){
	outs << "Name: " << t.get_name() << " Nest: " << t.get_nest();
	return outs;
}



std::ostream& operator<<(std::ostream &outs, Ant const& t){
	outs << "|position: " << t.get_position() << " state: "<< t.get_state() << "|";
	return outs;
}

std::ostream& operator<<(std::ostream &outs, Nest const& t){
	outs << "|position: " << t.get_position() << " resources: " 
	<< t.get_resources() <<" ants: " << t.get_ants().size() << "|";
	return outs;
}



std::ostream& operator<<(std::ostream& outs, Food const& t){
	outs << "|position: " << t.get_position() << " resources: " 
	<< t.get_resources() << "|";
	return outs;
}
