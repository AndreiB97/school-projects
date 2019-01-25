#pragma once

#include <Core/Engine.h>
#include <string>
#include <vector>
#include <math.h>
#include <iostream>
#include "Constants.h"

using namespace std;

class Circle
{
public:

	// center coords
	float x;
	float y;
	// props
	string name;
	float radius;
	glm::vec3 color;
	Mesh * mesh;
	float moveSpeedX;
	float moveSpeedY;
	bool render;
	

	Circle(float _radius, string _name);
	~Circle();
	void createMesh();
	void setCoord(float _x, float _y);
	void move(float _x, float _y);
	void platformReflect(float _x, float platformWidth);
};

