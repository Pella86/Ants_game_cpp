/*
 *  main.cpp
 *  AntsGame
 *
 *  Created by Mauro Pellanda on 12.03.12.
 *  Copyright 2012 UZH. All rights reserved.
 *
 */


#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <iostream>

#include "timer.h"
#include "Geometry.h"
#include "Color.h"
#include "Random_numbers.h"
#include "GameElements.h"
#include "GameEngine.h"

#define NODRAW_SIGHT //draw the circle representing the sight of the ant if is DRAWSIGHT

Timer timer; //timer that determines the global action

GameEngine g_eng; //the game engine

Random_number randn; //random number generator

GLfloat X = 0.0f; //global screen center
GLfloat Y = 0.0f;

GLfloat Zoom = 100.f; //global field of view



void draw_square(Square square){
	double x1 = square.center().x() - square.size();
	double x2 = square.center().x() + square.size();
	double y1 = square.center().y() - square.size();
	double y2 = square.center().y() + square.size();
	
	glColor3f(square.color().r(), square.color().g(), square.color().b());
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
}

void draw_circle(Circle const& circle){
	double x1 = circle.center().x();
	double y1 = circle.center().y();
	
	glColor3f(circle.color().r(), circle.color().g(), circle.color().b());
	for (int angle = 0; angle < 360; ++angle) {
		glVertex2f(x1+sin(angle)*circle.radius(), y1 + cos(angle)*circle.radius());
	}
	
}
	

void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(-Zoom, Zoom, -Zoom, Zoom);
	
	glTranslatef(X, Y, 0.0f); //translate the screen using global X or Y
	glutPostRedisplay();
	glColor3f(1., 1., 1.);

	//draw the axis
	glBegin(GL_LINES);
		glVertex2f(-100.0, 0.0);
		glVertex2f(100, 0.0);
		glVertex2f(0.0, -100.0);
		glVertex2f(0.0, 100.0);
    glEnd();
	
	//Game Engine action every 500ms (?) not sure if is really 500 ms
	if (timer.elasped(500)) {
		g_eng.action();
		
	}
	
	//draw the objects
	std::vector< BaseObject * > const& obj_list = g_eng.get_obj_list(); //get objects form the g_eng
	std::vector< Circle > circle_array; //if is a circle
	std::vector< Square > square_array;	//if is a square
	
	// bounding box (! is not bound with the g_eng bbox)
	circle_array.push_back(Circle(V2(),100.,Color(1.,1.,1.,0.)));
	
	//scroll the item of the obj array
	for (IT_cbObjptr it_obj = obj_list.begin(); it_obj != obj_list.end(); ++it_obj) {
		if ((*it_obj)->obj_type == obj_ant or (*it_obj)->obj_type == obj_trail) {
			Square sq = Square((*it_obj)->get_size(), (*it_obj)->get_position(), (*it_obj)->get_color());
			square_array.push_back(sq);
#ifdef DRAW_SIGHT
			if ((*it_obj)->obj_type == obj_ant) {
				Ant& tmp_ant = static_cast<Ant&> (*(*it_obj));
				circle_array.push_back(Circle(tmp_ant.get_position(), tmp_ant.get_sight_r(), Color(1.,1.,1.,0.)));
			}
#endif			
		}

		else {
			Circle sq = Circle((*it_obj)->get_position(), (*it_obj)->get_size(), (*it_obj)->get_color());
			circle_array.push_back(sq);
		}

	}
	
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < circle_array.size(); ++i) {
		draw_circle(circle_array[i]);
	}
	glEnd();
	
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < square_array.size(); ++i) {
		draw_square(square_array[i]);
	}
	glEnd();
	
	
		
	glFlush();
}

// Function to determine which key is pressed 
// Will be passed  to glutKeyboardFunc(keyCB);
void keyCB(unsigned char key, int x, int y){   /* called on key press */
    // if 'q' is pressed exit the program
    if( key == 'q' || key == 'Q') {exit(0);}
	switch (key) {
		case 'z': // zoom out
			Zoom += 10.;
			break;
		case 'u': // zoom in
			Zoom -= 10.;
			break;
		case 'c': //reset center and zoom
			Zoom = 100.;
			X = 0.;
			Y = 0.;
			break;

		default:
			break;
	}
}

// Similar function like keyCB; determines which special key is pressed
// Will be passed to glutSpecialFunc(specialKey);
void specialKey(int key, int x, int y) { // called on special key pressed
	
    // Check which (arrow) key is pressed
    switch(key) {
        case GLUT_KEY_LEFT : 	// Arrow key left is pressed
            X += 1.f;
			break;
        case GLUT_KEY_RIGHT :    	// Arrow key right is pressed
            X -=  1.f;
			break;
        case GLUT_KEY_UP :        	// Arrow key up is pressed
            Y -= 1.f;
			break;
        case GLUT_KEY_DOWN :    	// Arrow key down is pressed
            Y += 1.f;
			break;
    }
}


int main(int argc, char* argv[]){
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("My new window");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyCB);
	glutSpecialFunc(specialKey);
	glutMainLoop();
}

