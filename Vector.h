#pragma once
class Vector {
	
public:
	float x = 0.0f;
	float y = 0.0f;
	Vector() = default;
	Vector(float _x, float _y);

	
	void print();

	static Vector add(Vector v1, Vector v2);
	static Vector sub(Vector v1, Vector v2);
	static float getMag(Vector v);
	static Vector mult(Vector v1, float f);
	static Vector normalize(Vector v);
	static float dot(Vector v1, Vector v2);
};

