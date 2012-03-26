/*
 *  MathVecLibrary.h
 *  Pendulum2
 *
 *  Created by Mauro Pellanda on 17.12.11.
 *  Copyright 2011 ETH. All rights reserved.
 *
 */

#ifndef MATHVECLIBRARY_H
#define MATHVECLIBRARY_H

#include <algorithm>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

/////////////////////////////////////////
/****************************************
 * 2D Vector
 ****************************************/
////////////////////////////////////////

class V2 {
private:
	double coords[2];
public:
	
	V2();
	V2(double x, double y);
	V2(double* v);
	
	double& operator[](unsigned int i){return coords[i];}
	double const& operator[](unsigned int i) const {return coords[i];}
	
	double& x(){return coords[0];}
	double const& x() const {return coords[0];}
	double& y(){return coords[1];}
	double const& y() const {return coords[1];}
	
	double& r(){return coords[0];}
	double const& r() const {return coords[0];}
	double& angle(){return coords[1];}
	double const& angle() const {return coords[1];}
	
	
	V2& operator =  (V2 const& rhs); 
	V2& operator += (V2 const& rhs);
	V2& operator -= (V2 const& rhs);
	V2& operator *= (double const k);
	V2& operator /= (double const k);

	V2& operator -(); //to do x = -x
	
	V2 operator +( V2 const& rhs)  const;
	V2 operator -( V2 const& rhs)  const;
	V2 operator *( double rhs)     const;
	V2 operator *( V2 const& rhs)  const;
	V2 operator /( double const k) const;
	
	bool operator == (V2 const& rhs) const;
	bool is_null() const;
	
	double module() const;
	V2& normalize();
	V2 normalize() const;
	
	double distance(V2 const& v) const;
	
	V2 convert_in_polar() const;
	V2 convert_in_cartesian() const;
	
	V2 rotate(double theta) const;
	V2& rotate(double theta);
	
	
	V2 direction(const V2& a);
	void set_direction(const V2& d);
	void copy_direction(const V2& d);
	void set_magnitude(const double m);
	
	
};

inline
V2 V2::direction(const V2& a){
	V2 u = a;
	u.normalize();
	return u;
}


inline
void V2::set_direction(const V2& d){
	double m = module();
	V2 v = d;
	v.normalize();
	*this = v*m;
}


inline
void V2::copy_direction(const V2& d){
	set_direction(direction(d));
}


	
std::ostream& operator<<(std::ostream &outs, V2 const &v);
/////////////////////////////////////////
/****************************************
 * 3D Vector
 ****************************************/
////////////////////////////////////////



class V3{
private:
	double coords[3];
public:
	/************
	 Constructors
	 ************/
	V3();
	V3(double x, double y, double z);
	V3(double coord[3]);

	
	
	double& operator[](unsigned int i)             {return coords[i];} //assigning function
	double const& operator[](unsigned int i) const {return coords[i];} //read only function
	
	double const* get_vector() const{return coords;} //return the pointer to coords 
												     //used to pass the vector to glVertex3vd(v.get_vector)
	
	V3& assign3(double x,double y,double z); //assign x,y,z
	V3& assign3(double u_coords[3]); // assign an array of 3 coords
	

	V3& operator =  (V3 const& rhs); 
	V3& operator += (V3 const& rhs);
	V3& operator -= (V3 const& rhs);
	V3& operator *= (double const k);
	V3& operator *= (V3 const& rhs); //prodotto tra due vettori (non dot product)
	V3& operator /= (double const k);
	
	V3& operator -(); //to do x = -x
	
	V3 operator +( V3 const& rhs)  const;
	V3 operator -( V3 const& rhs)  const;
	V3 operator *( double rhs)     const;
	V3 operator *( V3 const& rhs)  const;
	V3 operator /( double const k) const;
	
	double module() const;
	void normalize();
	double distance(V3 const& rhs) const;
	
	double dot3(V3 const& rhs) const; //dot product
	
	double angle(V3 const& rhs) const; //angle
	
	V3& cross3(V3 const& rhs);
	V3 cross3( V3 const& rhs) const;
	
	std::string print_vector() const;
	/*
	 feature to add
	V3 cross3( V3 const& v);               // u = v.cross3(w)
	V3 cross3( V3 const& u, V3 const& v); // u = op.cross3(u,v)
	*/
	
};

/////////////////////////////////////////
/****************************************
 * 4D vectors
 ****************************************/
////////////////////////////////////////


class V4{
private:
	double coords[4];
public:
	/************
	 Constructors
	 ************/
	V4();
	V4(double x, double y, double z, double w);
	V4(double coord[4]);
	
	double& operator[](unsigned int i)             {return coords[i];} //assigning function
	double const& operator[](unsigned int i) const {return coords[i];} //read only function
	
	double const* get_vector() const{return coords;} //return the pointer to coords 
	//used to pass the vector to glVertex3vd(v.get_vector)
	
	V4& assign4(double x,double y,double z, double w); //assign x,y,z
	V4& assign4(double u_coords[4]); // assign an array of 3 coords
	
	
	V4& operator =  (V4 const& rhs); 
	V4& operator += (V4 const& rhs);
	V4& operator -= (V4 const& rhs);
	V4& operator *= (double const k);
	V4& operator *= (V4 const& rhs); //prodotto tra due vettori (non dot product)
	V4& operator /= (double const k);
	
	V4& operator -(); //to do x = -x
	
	V4 operator +( V4 const& rhs)  const;
	V4 operator -( V4 const& rhs)  const;
	V4 operator *( double rhs)     const;
	V4 operator *( V4 const& rhs)  const;
	V4 operator /( double const k) const;
	
	double module() const;
	void normalize();
	double distance(V4 const& rhs) const;
	
	double dot4(V4 const& rhs) const; //dot product
	
	double angle(V4 const& rhs) const; //angle
	

	
	std::string print_vector() const;
	/*
	 feature to add
	 V4 cross3( V4 const& v);               // u = v.cross3(w)
	 V4 cross3( V4 const& u, V4 const& v); // u = op.cross3(u,v)
	 */
	
};

#endif