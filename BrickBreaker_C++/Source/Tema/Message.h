#include <string>
#include <Core/Engine.h>


using namespace std;

#pragma once
class Message
{
public:

	// center coords
	float x;
	float y;
	// thickness of the letter
	float size;
	// width and height of the character
	float width;
	float height;
	// props
	glm::vec3 color;
	std::string name;
	Mesh* mesh;


	Message(float _width, float _height, string _name);
	~Message();
	void createMesh();
	void setCoord(float _x, float _y);
};

