#pragma once

#include <string>
#include <Core/Engine.h>
#include <math.h>
#include <iostream>
#include "Constants.h"

using namespace std;

enum class CollisionType { SIDE, TOPBOT, CORNER, NONE };

class MyRectangle
{
public:
	
	// center coords
	float x;
	float y;
	// size of rectangle
	float width;
	float height;
	// props
	glm::vec3 color;
	std::string name;
	Mesh* mesh;
	bool render;
	// indicates if a rectangle is in the middle of an animation
	bool resizing;


	MyRectangle(float _width, float _height, string _name);
	~MyRectangle();
	void createMesh();
	void setCoord(float _x, float _y);
	CollisionType checkCollision(float _x, float _y, float radius);
	float distance(float x1, float y1, float x2, float y2);
	void move(float _x, float _y);
	void resize(float val);
};


