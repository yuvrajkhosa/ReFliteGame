#include "Vector.h"
#include <iostream>



Vector::Vector(float _x, float _y){
	//std::cout << "Vector Created\n";
	x = _x;
	y = _y;
}

void Vector::print() {
	printf("X: %0.2f Y: %0.2f\n", x, y);
}

Vector Vector::add(Vector v1, Vector v2) {
	
	return(Vector(v1.x + v2.x, v1.y + v2.y));

}

Vector Vector::sub(Vector v1, Vector v2) {

	return(Vector(v1.x - v2.x, v1.y - v2.y));

}

float Vector::getMag(Vector v) {
	return(sqrt((v.x * v.x) + (v.y * v.y)));
}

Vector Vector::mult(Vector v1,float f) {
	return(Vector(v1.x * f, v1.y * f));
}

Vector Vector::normalize(Vector v) {
	
	return(Vector(v.x / Vector::getMag(v), v.y / Vector::getMag(v)));
}

float Vector::dot(Vector v1, Vector v2) {
	return(float(v1.x * v2.x + v1.y * v2.y));
}
