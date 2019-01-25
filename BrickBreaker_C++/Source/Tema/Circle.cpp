#include "Circle.h"



Circle::Circle(float _radius, string _name) : radius(_radius), name(_name)
{
	// constructor
	color = glm::vec3(1, 1, 1);
	mesh = new Mesh(name);
	x = 0;
	y = 0;
	createMesh();
	moveSpeedY = maxMoveVal;
	moveSpeedX = 0;
	render = true;
}

Circle::~Circle()
{
}

void Circle::createMesh() {
	// create the circle mesh

	vector<VertexFormat> vertices;
	vector<unsigned short> indices;

	vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));

	for (int i = 0; i < 100; i++) {
		float angle = RADIANS((float)i * 3.6f);		
		vertices.push_back(VertexFormat(glm::vec3(sin(angle) * radius, cos(angle) * radius, 0), color));
	}

	for (int i = 0; i < 99; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(99);

	mesh->InitFromData(vertices, indices);
}

void Circle::setCoord(float _x, float _y) {
	// set center coords
	x = _x;
	y = _y;
}

void Circle::move(float _x, float _y) {
	// move the circle
	x += _x;
	y += _y;
}

void Circle::platformReflect(float platformX, float platformWidth)
{
	// calculate trajectory after a collision with the platform
	// distance to center = sin of the angle * (platform width / 2)
	float sinVal = (x - platformX) / (platformWidth / 2);

	moveSpeedX = sinVal * maxMoveVal;
	moveSpeedY = sqrt(1 - sinVal * sinVal) * maxMoveVal;
}
