#include "Rectangle.h"

MyRectangle::MyRectangle(float _width, float _height, string _name) : width(_width),
height(_height), name(_name)
{
	// constructor
	mesh = new Mesh(name);
	color = glm::vec3(1, 0, 0);
	x = 0;
	y = 0;

	createMesh();
	render = true;
	resizing = false;

}

MyRectangle::~MyRectangle()
{	
}

void MyRectangle::createMesh() {
	// create the rectangle's mesh
	
	// corner coords
	glm::vec3 bottomLeft = glm::vec3(- width / 2, - height / 2, 0);
	glm::vec3 upperRight = glm::vec3(width / 2, height / 2, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(bottomLeft, color),
		VertexFormat(bottomLeft + glm::vec3(width, 0, 0), color),
		VertexFormat(bottomLeft + glm::vec3(width, height, 0), color),
		VertexFormat(bottomLeft + glm::vec3(0, height, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	mesh->InitFromData(vertices, indices);
}

void MyRectangle::setCoord(float _x, float _y) {
	// set center coords
	x = _x;
	y = _y;
}

CollisionType MyRectangle::checkCollision(float _x, float _y, float radius) {
	// check for collisions with the ball

	if (_x >= x - width / 2 && _x <= x + width / 2) {	
		// can collide with bottom or top of rectangle
		if (_y >= y - height / 2 - radius && _y <= y + height / 2 + radius) {
			// hit the bottom or top
			return CollisionType::TOPBOT;
		}
	}
	else if (_y >= y - height / 2  && _y <= y + height / 2) {
		// can collide with the sides
		if (_x >= x - width / 2 - radius && _x <= x + width / 2 + radius) {
			// hit the side
			return CollisionType::SIDE;
		}
	}
	else {
		// can collide with corners

		// top left corner
		if (distance(x - width / 2, y + height / 2, _x, _y) <= radius) {
			return CollisionType::CORNER;
		}

		// top right corner
		if (distance(x + width / 2, y + height / 2, _x, _y) <= radius) {
			return CollisionType::CORNER;
		}

		// bottom right corner
		if (distance(x + width / 2, y - height / 2, _x, _y) <= radius) {
			return CollisionType::CORNER;
		}

		// bottom left corner
		if (distance(x - width / 2, y - height / 2, _x, _y) <= radius) {
			return CollisionType::CORNER;
		}
	}

	return CollisionType::NONE;
}

float MyRectangle::distance(float x1, float y1, float x2, float y2) {
	// get distance of two points using pythagora's

	float x = fabs(x1 - x2);
	float y = fabs(y1 - y2);

	return sqrt(x * x + y * y);
}

void MyRectangle::move(float _x, float _y) {
	// move the rectangle (used for the platform)
	x += _x;
	y += _y;
}

void MyRectangle::resize(float val) {
	// resize animation that is played after the brick is destroyed
	if (width > brickWidth / 5 && height > brickHeight / 5) {
		this->width -= val * brickWidth;
		this->height -= val * brickHeight;
		createMesh();
	}
	else {
		// when animation is over stop rendering the brick
		render = false;
	}
}