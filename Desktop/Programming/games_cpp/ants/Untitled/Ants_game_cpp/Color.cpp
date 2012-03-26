/*
 *  Color.cpp
 *  Pendulum2
 *
 *  Created by Mauro Pellanda on 12.12.11.
 *  Copyright 2011 ETH. All rights reserved.
 *
 */

#include "Color.h"

Color::Color(){
	/*
	 *set the color to black
	 */
	rgba[0] = 0.;
	rgba[1] = 0.;
	rgba[2] = 0.;
	rgba[3] = 1.;
}

Color::Color(float r, float g, float b, float a){
	/*
	 *set the color to user value
	 */
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}


Color& Color::operator = (Color const& rhs){	
	rgba[0] = rhs[0];
	rgba[1] = rhs[1];
	rgba[2] = rhs[2];
	rgba[3] = rhs[3];
	return *this;
}

Color& Color::operator += (Color const& rhs){
	for (unsigned int i = 0; i < 4; ++i) {
		rgba[i] += rhs[i];
		if (rgba[i] > 2.f) {
			rgba[i] -= 1.;
		}		
	}
	return *this;
}

void Color::assign_rgba(float r, float g, float b, float a){
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
	
}


void Color::get_glColor() const{
	//calls the glColor() corresponding to the actual rgba values
	glColor4f(rgba[0], rgba[1], rgba[2], rgba[3]);
}


std::ostream & operator<<(std::ostream &outs, Color const& col){
	outs << "rgb(" << col.r() << ":" << col.g() << ":" << col.b()<< ":" << col.a() <<")";
	return outs;
}


/**************
 *Default Colors
 **************/

Colors::Colors(){
	red[0] = 1.;
	green[1] =1.;
	blue[2] = 1.;
	
	white.assign_rgba(1.,1.,1.,1.);
	yellow.assign_rgba(1., 1., 0., 1.);
}
