#include "Tema.h"


Tema::Tema()
{
}

Tema::~Tema()
{
}

void Tema::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// var inits

	// random number generator seed used for powerups
	srand(time(NULL));
	activePower = PowerUpType::NONE;
	powerUpTime = 0;
	started = false;
	won = false;
	livesNum = 3;

	// scene setup

	// victory message
	Message * message = new Message(150, 200, "G");
	message->setCoord(600, 360);
	meshes[message->name] = message->mesh;
	
	// walls
	leftWall = new MyRectangle(wallWidth, wallHeight, "left border wall");
	leftWall->setCoord(0, 720);
	meshes[leftWall->name] = leftWall->mesh;

	rightWall = new MyRectangle(wallWidth, wallHeight, "right border wall");
	rightWall->setCoord(1280, 720);
	meshes[rightWall->name] = rightWall->mesh;

	upperWall = new MyRectangle(wallHeight, wallWidth, "upper border wall");
	upperWall->setCoord(640, 720);
	meshes[upperWall->name] = upperWall->mesh;

	// platform
	platform = new MyRectangle(platformWidth, platformHeight, "platform");
	platform->setCoord(640, 25);
	meshes[platform->name] = platform->mesh;

	// bricks
	MyRectangle * brick;

	for (int i = 0; i < bricksPerCol; i++) {
		for (int j = 0; j < bricksPerLine; j++) {
			brick = new MyRectangle(brickWidth, brickHeight, "brick " + to_string(i * bricksPerLine + j));
			brick->setCoord((float)110 + j * (1135 / bricksPerLine), (float)600 - i * 50);
			blocks.push_back(brick);
			meshes[brick->name] = brick->mesh;
		}
	}

	// ball
	ball = new Circle(ballRadius, "ball");
	ball->setCoord(platform->x, platform->y + 25);
	meshes[ball->name] = ball->mesh;

	// lives display
	Circle * life;

	for (int i = 0; i < livesNum; i++) {
		life = new Circle(10, "life " + to_string(i));
		life->setCoord((float)25 + i * 35, (float)40);
		lives.push_back(life);
		meshes[life->name] = life->mesh;
	}
	
}

void Tema::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema::Update(float deltaTimeSeconds)
{
	// draw the walls
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(leftWall->x, leftWall->y);
	RenderMesh2D(meshes[leftWall->name], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(rightWall->x, rightWall->y);
	RenderMesh2D(meshes[rightWall->name], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(upperWall->x, upperWall->y);
	RenderMesh2D(meshes[upperWall->name], shaders["VertexColor"], modelMatrix);

	// draw the power ups and check for collisions with the platform
	list<PowerUp *> ::iterator powerUpIterator = powerUps.begin();
	float res = sqrt(2);

	while (powerUpIterator != powerUps.end()) {
		// animate the power up
		(*powerUpIterator)->setRotate(rotateSpeed * deltaTimeSeconds);
		(*powerUpIterator)->setMove(powerUpSpeed * deltaTimeSeconds);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate((*powerUpIterator)->x, (*powerUpIterator)->y);
		modelMatrix *= Transform2D::Rotate((*powerUpIterator)->rotateStep);

		// check for collisions
		CollisionType collision = platform->checkCollision((*powerUpIterator)->x,
			(*powerUpIterator)->y, (*powerUpIterator)->length / res);

		if (collision != CollisionType::NONE) {
			// activate power up

			// clear other power ups effects
			if (activePower != PowerUpType::NONE) {
				switch (activePower) {
				case BIGBALL:
					ball->radius /= bigBallScale;
					break;
				}
			}

			// apply new power up
			activePower = (*powerUpIterator)->type;
			switch (activePower) {
			case BIGBALL:
				ball->radius *= bigBallScale;
				break;
			}
			powerUpTime = 30;

			// remove it from the scene
			PowerUp* p = (*powerUpIterator);
			powerUpIterator++;
			powerUps.remove(p);
		}
		else {
			RenderMesh2D(meshes[(*powerUpIterator)->name], shaders["VertexColor"], modelMatrix);

			// if the power up is too low remove it from the scene
			PowerUp* p = (*powerUpIterator);
			powerUpIterator++;
			if (p->y < 0) {
				powerUps.remove(p);
			}
		}
	}

	// decrease remaining power up time
	if (powerUpTime > 0) {
		powerUpTime -= deltaTimeSeconds;
	}
	else {
		// power up expired
		// remove it's effect
		switch (activePower) {
		case BIGBALL:
			ball->radius /= bigBallScale;
			break;
		}
		activePower = PowerUpType::NONE;
	}
	
	if (ball->render) {
		modelMatrix = glm::mat3(1);
		// if the ball is offscreen then lose a life and reset
		if (ball->y < 0 || ball->x < 0 || ball->x > 1280) {
			started = false;

			livesNum--;
			lives.pop_back();

			ball->moveSpeedX = 0;
			ball->moveSpeedY = maxMoveVal;

			powerUpTime = 0;
			activePower = PowerUpType::NONE;

			if (livesNum > 0) {
				ball->setCoord(platform->x, platform->y + 25);
			}
			else {
				ball->render = false;
			}
		}

		if (started) {
			// check for collisions
			CollisionType collision;

			// walls
			collision = leftWall->checkCollision(ball->x, ball->y, ball->radius);

			if (collision != CollisionType::NONE) {
				switch (collision) {
				case CollisionType::SIDE:
					ball->moveSpeedX = -ball->moveSpeedX;
					break;
				case CollisionType::TOPBOT:
					ball->moveSpeedY = -ball->moveSpeedY;
					break;

				case CollisionType::CORNER:
					ball->moveSpeedX = -ball->moveSpeedX;
					ball->moveSpeedY = -ball->moveSpeedY;
					break;
				}

			}

			collision = rightWall->checkCollision(ball->x, ball->y, ball->radius);

			if (collision != CollisionType::NONE) {
				switch (collision) {
				case CollisionType::SIDE:
					ball->moveSpeedX = -ball->moveSpeedX;
					break;
				case CollisionType::TOPBOT:
					ball->moveSpeedY = -ball->moveSpeedY;
					break;

				case CollisionType::CORNER:
					ball->moveSpeedX = -ball->moveSpeedX;
					ball->moveSpeedY = -ball->moveSpeedY;
					break;
				}
			}

			collision = upperWall->checkCollision(ball->x, ball->y, ball->radius);

			if (collision != CollisionType::NONE) {
				// only one possible type of collision with upper wall
				ball->moveSpeedY = -ball->moveSpeedY;
			}

			// platform
			collision = platform->checkCollision(ball->x, ball->y, ball->radius);

			if (collision != CollisionType::NONE) {
				// only one possible type of collision with the platform

				if (collision == CollisionType::TOPBOT) {
					// check for power up
					ball->platformReflect(platform->x, platform->width);
					if (activePower == PowerUpType::STICKY) {
						started = false;
					}
				}
			}

			// bricks
			// check if the game is over
			won = true;

			for (int i = 0; i < blocks.size(); i++) {
				if (blocks[i]->render) {
					// found a brick that wasn't destroyed
					// game is not over yet
					won = false;

					// check if the brick is in the middle of an animation
					if (blocks[i]->resizing) {
						blocks[i]->resize(resizeSpeed * deltaTimeSeconds);
					}
					else {
						// check for collision
						collision = blocks[i]->checkCollision(ball->x, ball->y, ball->radius);

						if (collision != CollisionType::NONE) {
							// reflect ball based on collision type
							switch (collision) {
							case CollisionType::SIDE:
								ball->moveSpeedX = -ball->moveSpeedX;
								break;
							case CollisionType::TOPBOT:
								ball->moveSpeedY = -ball->moveSpeedY;
								break;
							case CollisionType::CORNER:
								ball->moveSpeedX = -ball->moveSpeedX;
								ball->moveSpeedY = -ball->moveSpeedY;
								break;
							}

							// enable the animation
							blocks[i]->resizing = true;

							// check for power up drop
							if (rand() % 10 == 0) {
								// 10% chance of a powerup
								// create power up
								PowerUp * powerUp = new PowerUp(blocks[i]->x, blocks[i]->y,
									powerUpLength, "PowerUp " + to_string(i));
								powerUps.push_back(powerUp);
								meshes[powerUp->name] = powerUp->mesh;
								
								// random power up type

								switch (rand() % 2) {
								case 0:
									powerUp->type = PowerUpType::BIGBALL;
									break;
								case 1:
									powerUp->type = PowerUpType::STICKY;
									break;
								default:
									powerUp->type = PowerUpType::NONE;
								}
							}

							break;
						}

					}
				}
			}

			// move the ball
			ball->move(ball->moveSpeedX * deltaTimeSeconds, ball->moveSpeedY * deltaTimeSeconds);
			
			// if the game is over reset the ball
			if (won) {
				started = false;
				ball->setCoord(platform->x, platform->y + 25);
			}
		} else if (won) {
			// display game over message
			glm::mat3 matrix = glm::mat3(1);
			matrix *= Transform2D::Translate(500, 360);
			RenderMesh2D(meshes["G"], shaders["VertexColor"], matrix);

			matrix *= Transform2D::Translate(200, 0);
			RenderMesh2D(meshes["G"], shaders["VertexColor"], matrix);
		}
 
		modelMatrix *= Transform2D::Translate(ball->x, ball->y);

		// apply power up effect
		if (activePower == PowerUpType::BIGBALL) {			
			modelMatrix *= Transform2D::Scale(bigBallScale, bigBallScale);
		}

		RenderMesh2D(meshes[ball->name], shaders["VertexColor"], modelMatrix);
	}

	// platform
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platform->x, platform->y);
	RenderMesh2D(meshes[platform->name], shaders["VertexColor"], modelMatrix);

	// bricks
	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i]->render) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(blocks[i]->x, blocks[i]->y);
			RenderMesh2D(meshes[blocks[i]->name], shaders["VertexColor"], modelMatrix);
		}
	}

	// lives counter
	list<Circle *> ::iterator circleIterator = lives.begin();
	while(circleIterator != lives.end()) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate((*circleIterator)->x, (*circleIterator)->y);
		RenderMesh2D(meshes[(*circleIterator)->name], shaders["VertexColor"], modelMatrix);
		circleIterator++;
	}
}

void Tema::FrameEnd()
{

}

void Tema::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Tema::OnKeyPress(int key, int mods)
{
	
}

void Tema::OnKeyRelease(int key, int mods)
{
	
}

void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// if the ball has not been launched then move it too
	if (!started) {
		ball->move((float)deltaX, 0);
	}

	// move the platform
	platform->move((float)deltaX, 0);
}

void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	
}

void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// launch the ball
	if (button == GLFW_MOUSE_BUTTON_2) {
		started = true;
	}
}

void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema::OnWindowResize(int width, int height)
{
}
