/*
 *  Color.h
 *  Pendulum2
 *
 *  Created by Mauro Pellanda on 12.12.11.
 *  Copyright 2011 ETH. All rights reserved.
 *
 */

#ifndef COLOR_H
#define COLOR_H

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <iostream>


class Color  {
private:
	float rgba[4];
public:
	Color();
	Color(float r, float g, float b, float a);
	
	float& r(){return rgba[0];}
	float const& r() const {return rgba[0];}
	float& g(){return rgba[1];}
	float const& g() const {return rgba[1];}
	float& b(){return rgba[2];}
	float const& b() const {return rgba[2];}
	float& a(){return rgba[3];}
	float const& a() const {return rgba[3];}
	
	void assign_rgba(float r, float g, float b, float a);
	
	Color& operator = (Color const& rhs);
	
	Color& operator += (Color const& rhs);
	Color  operator + (Color const& rhs) const {Color result(*this); result += rhs; return result;}
	
	float&        operator[](unsigned int i)        { return rgba[i];} //assign
	float const&  operator[](unsigned int i) const  { return rgba[i];} //read only
	
	void get_glColor() const;
};

std::ostream & operator<<(std::ostream &outs, Color const& col);

class Colors{
public:
	Color red;
	Color green;
	Color blue;
	Color white;
	Color black;
	Color yellow;
	
	Colors();
};


#endif
