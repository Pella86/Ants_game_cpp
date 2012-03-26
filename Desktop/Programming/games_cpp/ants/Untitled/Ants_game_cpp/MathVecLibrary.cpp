/*
 *  MathVecLibrary.cpp
 *  Pendulum2
 *
 *  Created by Mauro Pellanda on 17.12.11.
 *  Copyright 2011 EdoubleH. All rights reserved.
 *
 */

#include "MathVecLibrary.h"

/////////////////////////////////////////
/****************************************
 * 2D vector
 ****************************************/
////////////////////////////////////////
V2::V2(){
	coords[0] = 0.;
	coords[1] = 0.;	
}
V2::V2(double x, double y){
	coords[0] = x;
	coords[1] = y;	
}

V2::V2(double *v){
	for (int i = 0; i < 2; ++i) {
		coords[i] = *v;
		v++;
	}
}

V2& V2::operator  = (V2 const& rhs){
	coords[0] = rhs.x();
	coords[1] = rhs.y();
	return *this;
}

V2& V2::operator += (V2 const& rhs){
	x() += rhs.x();
	y() += rhs.y();
	return *this;
}

V2& V2::operator -= (V2 const& rhs){
	x() -= rhs.x();
	y() -= rhs.y();
	return *this;
}

V2& V2::operator *= (double const k){
	x() *= k;
	y() *= k;
	return *this;
}

V2& V2::operator /= (double const k){
	x() /= k;
	y() /= k;
	return *this;
}

V2& V2::operator - (){
	x() = -x();
	y() = -y();
	return *this;
}

V2 V2::operator +( V2 const& rhs)  const{
	V2 result(*this); result += rhs; return result;
}

V2 V2::operator -( V2 const& rhs)  const{
	V2 result(*this); result -= rhs; return result;
}

V2 V2::operator *( double rhs)     const{
	V2 result(*this); result *= rhs; return result;
}

V2 V2::operator /( double const k) const{
	V2 result(*this); result /= k; return result;
}


bool V2::operator == (V2 const& rhs) const{
	if (x() == rhs.x() and y() == rhs.y()) {
		return true;
	}
	else {
		return false;
	}
}

bool V2::is_null() const {
	if (module() == 0.) {
		return true;
	}
	else {
		return false;
	}
}


double V2::module() const{
	return sqrt(x()*x() + y()*y());
}

V2& V2::normalize(){
	double mod = module();
	for (int i = 0; i<2; ++i) {
		coords[i] /= mod;
	}
	return *this;
}
V2 V2::normalize() const{
	V2 result(*this); result.normalize(); return result;
}

double V2::distance(V2 const& v) const{
	double d = 0;
	for (int i = 0; i < 2; ++i) {
		d += (coords[i] - v[i])*(coords[i] - v[i]);
	}
	return std::sqrt(d);
}

V2 V2::convert_in_polar() const{
	double r = module();
	double theta = atan2(y(), x());
	return V2(r, theta);
}

V2 V2::convert_in_cartesian() const{
	double x = r()*cos(angle());
	double y = r()*sin(angle());
	return V2(x,y);
}

V2  V2::rotate(double theta) const{
	double tmpx = x()*cos(theta) - y()*sin(theta);
	double tmpy = x()*sin(theta) + y()*cos(theta);
	V2 result(tmpx,tmpy);
	result.normalize();
	return result;
}

V2& V2::rotate(double theta){
	double fixX = x();
	double fixY = y();
	double tmpx = fixX*cos(theta) - fixY*sin(theta);
	double tmpy = fixX*sin(theta) + fixY*cos(theta);
	x() = tmpx;
	y() = tmpy;
	normalize();
	return *this;
}	

std::ostream& operator<<(std::ostream &outs, V2 const &v){
	outs << "(" << v.x() << ":" << v.y() << ")";
	return outs;
}

/////////////////////////////////////////
/****************************************
 * 3D vector
 ****************************************/
////////////////////////////////////////



V3::V3(){
	//assign every coord to 0
	coords[0] = 0;
	coords[1] = 0;
	coords[2] = 0;
	
}


V3::V3(double x, double y, double z){
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}


V3::V3(double coord[3]){
	coords[0] = coord[0];
	coords[1] = coord[1];
	coords[2] = coord[2];
	
}

double V3::module() const{
	double result = coords[0]*coords[0];
	  result += coords[1]*coords[1];
	  result += coords[2]*coords[2];
	return std::sqrt(result);
}


V3& V3::assign3(double x, double y, double z){
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;	
}

V3& V3::assign3(double u_coords[3]){
	coords[0] = u_coords[0];
	coords[1] = u_coords[1];
	coords[2] = u_coords[2];
	return *this;
}

V3& V3::operator = (V3 const& rhs){
	coords[0] = rhs[0];
	coords[1] = rhs[1];
	coords[2] = rhs[2];
	return *this; //why dereferencing the pointer for a reference?
}


V3& V3::operator += (V3 const& rhs){
	coords[0] = coords[0]+rhs[0];
	coords[1] = coords[1]+rhs[1];
	coords[2] = coords[2]+rhs[2];
	//std::transform(coords,coords+3,rhs.coords, coords,std::plus);
	return *this;
}


V3& V3::operator -=(V3 const& rhs){
	coords[0] = coords[0] - rhs[0];
	coords[1] = coords[1] - rhs[1];
	coords[2] = coords[2] - rhs[2];
	//std::transform(coords,coords+3,rhs.coord,coords,std::minus);
	return *this;
}


V3& V3::operator *=(double k){
	coords[0] = coords[0] *k;
	coords[1] = coords[1] *k;
	coords[2] = coords[2] *k;
	//std::transform(coords, coords+3,coords,std::bind1st(std::multiplies,k));
	return *this;
}

V3& V3::operator *=(V3 const& rhs){
	coords[0] = coords[0]*rhs[0];
	coords[1] = coords[1]*rhs[1];
	coords[2] = coords[2]*rhs[2];
	return *this;
}

V3& V3::operator /= (double const k){
	coords[0] = coords[0]/k;
	coords[1] = coords[1]/k;
	coords[2] = coords[2]/k;
	return *this;
	
}

V3 V3::operator+(V3 const& rhs) const{
	V3 result(*this); result += rhs; return result;
}

V3 V3::operator-(V3 const& rhs) const{
	V3 result(*this); result -= rhs; return result;
}
V3& V3::operator-(){
	coords[0] = -coords[0];
	coords[1] = -coords[1];
	coords[2] = -coords[2];
	return *this;
}

V3 V3::operator*(double rhs) const{
	V3 result(*this); result *= rhs; return result;
}

V3 V3::operator*(V3 const& rhs) const {
	V3 result(*this); result *= rhs; return result;
}

V3 V3::operator /( double const k) const{
	V3 result(*this); result /= k; return result;
}

void V3::normalize(){
	double module = this->module();
	*this *= (1/module); //compiles fine but not sure if does what I mean
}


double V3::distance(V3 const& rhs) const {
	// d = sqrt((a1+b1)^2+...)
	double result = 0;
	for (int i = 0; i<3; ++i) {
		result += std::pow(coords[i]*rhs[i],2);
	}
	return result;
}


double V3::dot3(V3 const& rhs) const {
	double result = 0;
	for (int i = 0; i < 3; ++i) {
		result += coords[i]*rhs[i];
	}
	return result;
}

double V3::angle(V3 const& rhs) const {
	    //theta = arccos((a.dot3(b))/(a.module()*b.module())
	return acos(dot3(rhs)/(module()*rhs.module()));	
}

V3& V3::cross3(V3 const& rhs){
	V3 res;
	//a2b3-a3b2
	//a3b1-a1b3
	//a1b2-a2b1
	//y*rhs.z-z*rhs.y
	res[0] = coords[1]*rhs[2]-coords[2]*rhs[1];
	res[1] = coords[2]*rhs[0]-coords[0]*rhs[2];
	res[2] = coords[0]*rhs[1]-coords[1]*rhs[0];
	
	this->operator=(res);
	return *this;
}

V3 V3::cross3( V3 const& rhs) const {
	V3 res;
	//a2b3-a3b2
	//a3b1-a1b3
	//a1b2-a2b1
	//y*rhs.z-z*rhs.y
	res[0] = coords[1]*rhs[2]-coords[2]*rhs[1];
	res[1] = coords[2]*rhs[0]-coords[0]*rhs[2];
	res[2] = coords[0]*rhs[1]-coords[1]*rhs[0];
	return res;
}


std::string V3::print_vector() const {
	std::stringstream ss;
	std::string s;
	ss<< "("<< coords[0] << ":"<< coords[1] << ":"<< coords[2] << ")";
	s = ss.str();
	return s;
}

/////////////////////////////////////////
/****************************************
 * 4D vector
 ****************************************/
////////////////////////////////////////


V4::V4(){
	//assign every coord to 0
	coords[0] = 0;
	coords[1] = 0;
	coords[2] = 0;
	coords[3] = 0;
}


V4::V4(double x, double y, double z, double w){
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	coords[3] = w;
}


V4::V4(double coord[4]){
	coords[0] = coord[0];
	coords[1] = coord[1];
	coords[2] = coord[2];
	coords[3] = coord[3];
}

double V4::module() const{
	double result = coords[0]*coords[0];
	result += coords[1]*coords[1];
	result += coords[2]*coords[2];
	result += coords[3]*coords[3];
	return std::sqrt(result);
}


V4& V4::assign4(double x, double y, double z, double w){
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	coords[3] = w;
}

V4& V4::assign4(double u_coords[4]){
	coords[0] = u_coords[0];
	coords[1] = u_coords[1];
	coords[2] = u_coords[2];
	coords[3] = u_coords[3];
	return *this;
}

V4& V4::operator = (V4 const& rhs){
	coords[0] = rhs[0];
	coords[1] = rhs[1];
	coords[2] = rhs[2];
	coords[3] = rhs[3];
	return *this; //why dereferencing the pointer for a reference?
}


V4& V4::operator += (V4 const& rhs){
	coords[0] = coords[0]+rhs[0];
	coords[1] = coords[1]+rhs[1];
	coords[2] = coords[2]+rhs[2];
	coords[3] = coords[3]+rhs[3];
//std::transform(coords,coords+3,rhs.coords, coords,std::plus);
	return *this;
}


V4& V4::operator -=(V4 const& rhs){
	coords[0] = coords[0] - rhs[0];
	coords[1] = coords[1] - rhs[1];
	coords[2] = coords[2] - rhs[2];
	coords[3] = coords[3] - rhs[3];
	//std::transform(coords,coords+3,rhs.coord,coords,std::minus);
	return *this;
}


V4& V4::operator *=(double k){
	coords[0] = coords[0] *k;
	coords[1] = coords[1] *k;
	coords[2] = coords[2] *k;
	coords[3] = coords[3] *k;
	//std::transform(coords, coords+3,coords,std::bind1st(std::multiplies,k));
	return *this;
}

V4& V4::operator *=(V4 const& rhs){
	coords[0] = coords[0]*rhs[0];
	coords[1] = coords[1]*rhs[1];
	coords[2] = coords[2]*rhs[2];
	coords[3] = coords[3]*rhs[3];
	return *this;
}

V4& V4::operator /= (double const k){
	coords[0] = coords[0]/k;
	coords[1] = coords[1]/k;
	coords[2] = coords[2]/k;
	coords[3] = coords[3]/k;
	return *this;
	
}

V4 V4::operator+(V4 const& rhs) const{
	V4 result(*this); result += rhs; return result;
}

V4 V4::operator-(V4 const& rhs) const{
	V4 result(*this); result -= rhs; return result;
}
V4& V4::operator-(){
	coords[0] = -coords[0];
	coords[1] = -coords[1];
	coords[2] = -coords[2];
	coords[3] = -coords[3];
	return *this;
}

V4 V4::operator*(double rhs) const{
	V4 result(*this); result *= rhs; return result;
}

V4 V4::operator*(V4 const& rhs) const {
	V4 result(*this); result *= rhs; return result;
}

V4 V4::operator /( double const k) const{
	V4 result(*this); result /= k; return result;
}

void V4::normalize(){
	double module = this->module();
	*this *= (1/module); //compiles fine but not sure if does what I mean
}


double V4::distance(V4 const& rhs) const {
	// d = sqrt((a1+b1)^2+...)
	double result = 0;
	for (int i = 0; i<4; ++i) {
		result += std::pow(coords[i]*rhs[i],2);
	}
	return result;
}


double V4::dot4(V4 const& rhs) const {
	double result = 0;
	for (int i = 0; i < 4; ++i) {
		result += coords[i]*rhs[i];
	}
	return result;
}

double V4::angle(V4 const& rhs) const{
	//theta = arccos((a.dot3(b))/(a.module()*b.module())
	return acos(dot4(rhs)/(module()*rhs.module()));	
}


std::string V4::print_vector() const {
	std::stringstream ss;
	std::string s;
	ss<< "("<< coords[0] << ":"<< coords[1] << ":"<< coords[2]<<":"<<coords[3] << ")";
	s = ss.str();
	return s;
}

