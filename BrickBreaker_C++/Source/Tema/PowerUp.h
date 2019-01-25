#pragma once

#include <string>
#include <Core/Engine.h>
#include <iostream>

using namespace std;

enum PowerUpType { NONE, BIGBALL, STICKY };

class PowerUp
{
public:
	
	// corner coords
	glm::vec3 corners[4];
	// center coords
	float x;
	float y;
	// side length
	float length;
	// mesh props
	string name;
	Mesh * mesh;
	glm::vec3 color;
	// props
	float rotateStep;
	PowerUpType type;

	PowerUp(float _x, float _y, float _length, string _name);
	~PowerUp();
	void createMesh();
	void setRotate(float _rotateStep);
	void setMove(float move);
};

