/*
 *  Geometry.h
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 12.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include "Color.h"
#include "MathVecLibrary.h"

double rad2deg(double rad);
double deg2rad(double deg);

bool intersect_circle(V2 v1, V2 v2, double r);

class Square {
private:
	double _size;
	V2 _center;
	Color _color;
public:
	Square() : _size(1.), _center(V2()), _color(Color()){}
	Square(double s, V2 c, Color col);
	
	double& size() {return _size;}
	double const& size() const {return _size;}
	V2& center() {return _center;}
	V2 const& center() const {return _center;}
	Color& color() {return _color;}
	Color const& color() const {return _color;}
	
};


class Circle {
private:
	V2 _center;
	double _radius;
	Color _color;
public:
	Circle() : _center(V2()), _radius(1.), _color(Color()){}
	Circle(V2 center, double radius, Color color);
	
	double& radius() {return _radius;}
	double const& radius() const {return _radius;}
	V2& center() {return _center;}
	V2 const& center() const {return _center;}
	Color& color() {return _color;}
	Color const& color() const {return _color;}	
	
};

std::ostream& operator<<(std::ostream &outs, Square const &square);

#endif
