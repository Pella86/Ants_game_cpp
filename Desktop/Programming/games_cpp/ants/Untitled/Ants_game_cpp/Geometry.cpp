/*
 *  Geometry.cpp
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 12.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */

#include "Geometry.h"

#define PI 3.1415926f

double rad2deg(double rad){
	double deg = rad*(180./PI);
	return deg;
}
double deg2rad(double deg){
	double rad = deg*(PI/180.);
	return rad;
}

bool intersect_circle(V2 v1, V2 v2, double r){
	double d = v1.distance(v2);
	if (d < r) {
		return true;
	}
	else {
		return false;
	}

}


Square::Square(double s, V2 c, Color col){
	_size = s;
	_center = c;
	_color = col;
}

std::ostream & operator << (std::ostream& outs, Square const &square){
	outs << "center: " << square.center() << " size: " << square.size() << " color: " << square.color(); 
	return outs;
}


Circle::Circle(V2 center, double radius, Color color){
	_center = center;
	_radius = radius;
	_color = color;
}

