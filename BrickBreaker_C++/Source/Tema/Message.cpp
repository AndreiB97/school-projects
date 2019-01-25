#include "Message.h"



Message::Message(float _width, float _height, string _name) : width(_width), height(_height), name(_name)
{
	// constructor
	mesh = new Mesh(name);
	size = height / 6;
	color = glm::vec3(0, 1, 0);
	createMesh();
}


Message::~Message()
{
}

void Message::createMesh()
{
	// create the mesh
	glm::vec3 point = glm::vec3(-width / 2, height / 2, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(point, color),
		VertexFormat(point + glm::vec3(0, -height, 0), color),
		VertexFormat(point + glm::vec3(width, -height, 0), color),
		VertexFormat(point + glm::vec3(width, -(height - 3 * size), 0), color),
		VertexFormat(point + glm::vec3(width - 2 * size, -(height - 3 * size), 0), color),
		VertexFormat(point + glm::vec3(width - 2 * size, -(height - 2 * size), 0), color),
		VertexFormat(point + glm::vec3(width - size, -(height - 2 * size), 0), color),
		VertexFormat(point + glm::vec3(width - size, -(height - size), 0), color),
		VertexFormat(point + glm::vec3(size, -(height - size), 0), color),
		VertexFormat(point + glm::vec3(size, -size, 0), color),
		VertexFormat(point + glm::vec3(width, -size, 0), color),
		VertexFormat(point + glm::vec3(width, 0, 0), color),
	};

	std::vector<unsigned short> indices = { 
		0, 9, 1,
		1, 8, 9,
		1, 7, 8,
		1, 2, 7,
		2, 3, 7,
		7, 3, 6,
		3, 4, 6,
		4, 5, 6,
		9, 10, 11,
		9, 11, 0
	};

	mesh->InitFromData(vertices, indices);
}

void Message::setCoord(float _x, float _y)
{
	// set the center's coords
	x = _x;
	y = _y;
}
