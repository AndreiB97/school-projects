#include "PowerUp.h"


PowerUp::PowerUp(float _x, float _y, float _length, string _name) : 
	x(_x), y(_y), length(_length), name(_name)
{
	// constructor

	// set corners based on center coords and length
	corners[0] = glm::vec3(-length / 2, length / 2, 0);
	corners[1] = glm::vec3(length / 2, length / 2, 0);
	corners[2] = glm::vec3(length / 2, -length / 2, 0);
	corners[3] = glm::vec3(-length / 2, -length / 2, 0);

	// set color
	color = glm::vec3(0, 1, 0);

	// init the mesh
	mesh = new Mesh(name);
	createMesh();

	// set rotate
	rotateStep = 0;
}

PowerUp::~PowerUp()
{
}

void PowerUp::createMesh()
{
	// init mesh based on corners
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corners[0], color),
		VertexFormat(corners[1], color),
		VertexFormat(corners[2], color),
		VertexFormat(corners[3], color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	mesh->InitFromData(vertices, indices);
}

void PowerUp::setRotate(float _rotateStep)
{
	// increase rotation
	rotateStep += _rotateStep;
}

void PowerUp::setMove(float move)
{
	// move down
	y -= move;
}
