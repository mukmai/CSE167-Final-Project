#include "window.h"

const char* window_title = "GLFW Starter Project";
bool fullScreen = true;
bool Window::playerView = true;
bool Window::toon_shade = false;
LSystem* mainTree;

OBJObject* sphere;
OBJObject* cylinder;
OBJObject* limb;

CubeMap* cubemap;
Transform* world;
Transform* cubemapS;
Transform* stageOffset;
Transform* stageS;
Transform* orbOffset;
Transform* particlePlacement;
Transform* mainTreeS;
Transform* orbS;

Geometry* orb;
Geometry* stage;

std::vector<TotemPart*> totemParts;

LightSource* sunLight;
Terrain* terrain;
int terrainSize = 256;
unsigned int Window::seed = 4;

ParticleManager* particles;

std::vector<std::pair<Node*, int>> objects;

bool spreadMode = false;
float treeGrowRatio = 0;
bool Window::endGame = false;
bool Window::endFinished = false;
float orbShrinkRatio = 1;
int spinRound = 1080;
int nStepsBack = 10;
int nCamBackOff = 280;
int standTime = 300;

Player* player;
PlayerBody* playerBody;

GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

std::vector<const char*> textureFiles;

// Trackball variables
int lbutton_down = false;
glm::vec3 lastTrackBallPoint;
glm::vec3 curTrackBallPoint;
glm::mat4 VM = glm::mat4(1.0f);
glm::mat4 VOrig = glm::mat4(1.0f);
glm::mat4 regularV;

glm::vec2 lastMousePos;
glm::vec2 curMousePos;

bool LSystem::leafSwitch;

void Window::initialize_objects()
{
	/*
	textureFiles.push_back("Skybox_Water222_right.ppm"); // right
	textureFiles.push_back("Skybox_Water222_left.ppm"); // left
	textureFiles.push_back("Skybox_Water222_top.ppm"); // top
	textureFiles.push_back("Skybox_Water222_base.ppm"); // bottom
	textureFiles.push_back("Skybox_Water222_front.ppm"); // front
	textureFiles.push_back("Skybox_Water222_back.ppm"); // back
	*/

	textureFiles.push_back("back.ppm"); // back
	textureFiles.push_back("front.ppm"); // front
	textureFiles.push_back("top.ppm"); // top
	textureFiles.push_back("bottom.ppm"); // bottom
	textureFiles.push_back("left.ppm"); // left
	textureFiles.push_back("right.ppm"); // right

	sphere = new OBJObject("sphere.obj", true);
	cylinder = new OBJObject("body_s.obj", false);
	limb = new OBJObject("limb_s.obj", false);

	orb = new Geometry(sphere, glm::vec3(0, 0.80, 0.91));
	stage = new Geometry(cylinder, glm::vec3(0.5, 0.5, 0.5));

	world = new Transform(glm::mat4(1.0f));
	cubemapS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(500.0f)));
	stageOffset = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0,-0.7,0)));
	stageS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(5,1,5)));
	orbOffset = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0)));
	orbS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(orbShrinkRatio)));
	
	// where the particle system begins generating particles
	particlePlacement = new Transform(glm::mat4(1.0f));

	initializeTotem();

	cubemap = new CubeMap(textureFiles);

	initializeTerrain();

	sunLight = new LightSource(glm::vec3(0.5, 0.47, 0.35), glm::vec3(0, -1, 2));

	Geometry* bodyPart = new Geometry(cylinder, glm::vec3(0.54, 0.42, 0.12));
	Geometry* headPart = new Geometry(sphere, glm::vec3(0.99, 0.89, 0.72));
	Geometry* limbPart = new Geometry(limb, glm::vec3(0.54, 0.42, 0.12));
	playerBody = new PlayerBody(headPart, bodyPart, limbPart);
	player = new Player(playerBody, terrain);

	// actual particle generation system
	Geometry* particleShape = new Geometry(sphere, glm::vec3(0.95f, 0.75f, 0.38f));

	particles = new ParticleManager(10000);
	//particles->transform->translate(glm::vec3(0, 10, 0));

	world->addChild(cubemapS);
	cubemapS->addChild(cubemap);
	world->addChild(stageOffset);
	stageOffset->addChild(stageS);
	stageOffset->addChild(orbOffset);
	orbOffset->addChild(orbS);
	orbS->addChild(orb);

	mainTree = new LSystem(2, 7);
	mainTreeS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(treeGrowRatio)));
	Transform* treeR1 = new Transform(glm::rotate(glm::mat4(1.0f), -glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	Transform* treeR2 = new Transform(glm::rotate(glm::mat4(1.0f), -glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	treeR1->addChild(mainTree);
	treeR2->addChild(mainTree);
	stageOffset->addChild(mainTreeS);
	mainTreeS->addChild(mainTree);
	mainTreeS->addChild(treeR1);
	mainTreeS->addChild(treeR2);
	

	stageS->addChild(stage);

	world->addChild(particles);

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	printf("completed initialize\n");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
	delete(world);
	delete(sunLight);
	delete(player);
	delete(terrain);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window;
	if (fullScreen) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);
	}
	else {
		window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	}

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		VOrig = glm::lookAt(cam_pos, cam_look_at, cam_up);
		regularV = VM * VOrig;
	}
}

void Window::idle_callback()
{
	particles->update();
	playerBody->update();
	if (spreadMode && treeGrowRatio < 1) {
		treeGrowRatio += 0.01;
		mainTreeS->set(glm::scale(glm::mat4(1.0f), glm::vec3(treeGrowRatio)));
	}
	else if (!endGame && !spreadMode && treeGrowRatio > 0) {
		treeGrowRatio -= 0.01;
		mainTreeS->set(glm::scale(glm::mat4(1.0f), glm::vec3(treeGrowRatio)));
	}
	if (endGame && !endFinished) {
		if (orbShrinkRatio > 0) {
			playerBody->moving = false;
			player->cam_look_at = glm::vec3(0, 4, 0) + (player->position + glm::vec3(0, 3, 0) - glm::vec3(0, 4, 0)) * orbShrinkRatio;
			orbShrinkRatio -= 0.01;
			orbS->set(glm::scale(glm::mat4(1.0f), glm::vec3(orbShrinkRatio)));
		} else {
			if (nStepsBack > 0 && (standTime % 2 == 0)) {
				standTime--;
				nStepsBack--;
				player->move(2);
			}
			else if (nStepsBack > 0 && (standTime % 2 == 1)){
				standTime--;
			}
			else {
				if (standTime > 0) {
					// drop particle here
					playerBody->moving = false;
					standTime--;
				}
				else {
					particles->turnParticlesOff();
					terrain->switchSpreading(true);
					if (treeGrowRatio < 1) {
						treeGrowRatio += 0.004;
						mainTreeS->set(glm::scale(glm::mat4(1.0f), glm::vec3(treeGrowRatio)));
					}
					if (nCamBackOff > 0) {
						nCamBackOff--;
						player->cam_backOff += glm::vec3(-0.35, 0.25, 0);
					}
					if (spinRound > 360) {
						spinRound--;
						player->cam_rotateX = glm::rotate(player->cam_rotateX, -glm::radians(0.5f), glm::vec3(0, 1, 0));
					}
					else {
						player->cam_look_at = player->position + glm::vec3(0, 3, 0);
						if (nCamBackOff > -140) {
							nCamBackOff--;
							player->cam_backOff += glm::vec3(0.7, -0.5, 0);
							player->cam_rotateX = glm::rotate(player->cam_rotateX, -glm::radians(0.5f), glm::vec3(0, 1, 0));
						}
						else {
							endFinished = true;
						}
					}
				}
			}

		}
		player->update();
	}
}

void Window::display_callback(GLFWwindow* window)
{
	if (playerView) {
		V = player->thirdPersonV;
	}
	else {
		V = regularV;
	}
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	sunLight->draw(shaderProgram, glm::mat4(1.0f));
	terrain->draw(shaderProgram, glm::mat4(1.0f));
	LSystem::leafSwitch = true;
	world->draw(shaderProgram, glm::mat4(1.0f));
	LSystem::leafSwitch = false;
	player->draw(shaderProgram, glm::mat4(1.0f));

	particles->draw(shaderProgram, glm::mat4(1.0f));

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	else if (key == GLFW_KEY_T) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(-2.0f), glm::vec3(1, 0, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(0, 0, 0.8f));
		}

		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_G) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(2.0f), glm::vec3(1, 0, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(0, 0, -0.8f));
		}
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_F) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(-2.0f), glm::vec3(0, 1, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(0.8f, 0, 0));
		}
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_H) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(2.0f), glm::vec3(0, 1, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(-0.8f, 0, 0));
		}
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_R) {
		VM = glm::translate(VM, glm::vec3(0, -0.8f, 0));
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_Y) {
		VM = glm::translate(VM, glm::vec3(0, 0.8f, 0));
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_1) {
		if (mods == GLFW_MOD_SHIFT) {
			seed--;
			delete(terrain);
			initializeTerrain();
			player->terrain = terrain;
			player->update();
		}
		else {
			seed++;
			delete(terrain);
			initializeTerrain();
			player->terrain = terrain;
			player->update();
		}
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_2) {
		spreadMode = !spreadMode;
		terrain->switchSpreading(spreadMode);
		
		if (spreadMode) {
			particles->turnParticlesOff();
		}
	}
	else if (key == GLFW_KEY_3) {
		particles->toggleParticles();
	}
	else if (key == GLFW_KEY_4) {
		toon_shade = !toon_shade;
	}
	else if (!endGame || endFinished) {
		if (key == GLFW_KEY_W) {
			if (action == GLFW_RELEASE) {
				playerBody->moving = false;
			}
			else {
				player->move(0);
			}
		}
		else if (key == GLFW_KEY_D) {
			if (action == GLFW_RELEASE) {
				playerBody->moving = false;
			}
			else {
				player->move(1);
			}
			player->move(1);
		}
		else if (key == GLFW_KEY_S) {
			if (action == GLFW_RELEASE) {
				playerBody->moving = false;
			}
			else {
				player->move(2);
			}
		}
		else if (key == GLFW_KEY_A) {
			if (action == GLFW_RELEASE) {
				playerBody->moving = false;
			}
			else {
				player->move(3);
			}
		}
	}

	if (key == GLFW_KEY_X) {
		playerView = !playerView;
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action) {
			double x;
			double y;
			lbutton_down = true;
			glfwGetCursorPos(window, &x, &y);
			lastTrackBallPoint = trackBallMapping(window, x, y);
		}
		else if (GLFW_RELEASE == action) {
			lbutton_down = false;
		}
	}
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!playerView && lbutton_down) {
		curTrackBallPoint = trackBallMapping(window, xpos, ypos);
		float velocity = glm::sqrt(glm::dot((curTrackBallPoint - lastTrackBallPoint), (curTrackBallPoint - lastTrackBallPoint)));
		if (velocity > 0.0001) {
			glm::vec3 rotAxis = glm::cross(lastTrackBallPoint, curTrackBallPoint);
			float rotAngle = glm::acos(glm::dot(lastTrackBallPoint, curTrackBallPoint) / (length(lastTrackBallPoint) * length(curTrackBallPoint)));
			VM = glm::rotate(VM, rotAngle / 3, rotAxis);
			regularV = VM * VOrig;
		}

		lastTrackBallPoint = curTrackBallPoint;
	}
	else if (playerView && (!endGame || endFinished)) {
		curMousePos.x = xpos;
		curMousePos.y = ypos;
		float xdiff = curMousePos.x - lastMousePos.x;
		float ydiff = curMousePos.y - lastMousePos.y;
		player->cam_rotateX = glm::rotate(player->cam_rotateX, -xdiff / 200, glm::vec3(0, 1, 0));
		player->cam_rotateY = glm::rotate(player->cam_rotateY, -ydiff / 400, glm::vec3(0, 0, 1));
		player->update();
		lastMousePos = curMousePos;
	}
}

glm::vec3 Window::trackBallMapping(GLFWwindow* window, double x, double y)
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	v.x = (2.0f * x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (height - 2.0f * y) / height;   // this does the equivalent to the above for the mouse Y position
	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0f) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001f - d * d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	return glm::normalize(v);  // Still need to normalize, since we only capped d, not v. Return the mouse location on the surface of the trackball
}

void Window::initializeTerrain() {
	objects.clear();
	// totems and their constituent parts
	Totem* totemA = new Totem(totemParts, 0);
	Totem* totemB = new Totem(totemParts, 1);
	Totem* totemC = new Totem(totemParts, 2);

	objects.push_back(std::make_pair(totemA, 1));
	objects.push_back(std::make_pair(totemB, 1));
	objects.push_back(std::make_pair(totemC, 1));

	LSystem* object1 = new LSystem(0, 4);
	LSystem* object2 = new LSystem(1, 3);
	LSystem* object3 = new LSystem(0, 5);
	objects.push_back(std::make_pair(object1, 5));
	objects.push_back(std::make_pair(object2, 10));
	objects.push_back(std::make_pair(object3, 4));

	terrain = new Terrain(terrainSize, terrainSize, 1, objects);
}

void Window::initializeTotem()
{
	// a collection of totem parts to be used in the totem constructor
	// currently just uses cylindars and spheres, will produce more geometries later
	// These do not have to actually be moved, just keeping them as identity matrices should be enough 
	// unless there should be some squishing or flipping
	// geometries (color coded for convenience)
	Geometry* blackSphere = new Geometry(sphere, glm::vec3(38 / 255.0, 38 / 255.0, 38 / 255.0));
	Geometry* whiteSphere = new Geometry(sphere, glm::vec3(242 / 255.0, 242 / 255.0, 242 / 255.0));
	Geometry* redSphere = new Geometry(sphere, glm::vec3(255 / 255.0, 51 / 255.0, 0 / 255.0));

	Geometry* blackCylinder = new Geometry(cylinder, glm::vec3(38 / 255.0, 38 / 255.0, 38 / 255.0));
	Geometry* whiteCylinder = new Geometry(cylinder, glm::vec3(242 / 255.0, 242 / 255.0, 242 / 255.0));
	Geometry* redCylinder = new Geometry(cylinder, glm::vec3(255 / 255.0, 51 / 255.0, 0 / 255.0));

	// generic eye
	Transform* eye = new Transform(glm::mat4(1.0f));
	Transform* eyeOutline = new Transform(glm::mat4(1.0f));
	Transform* eyeIris = new Transform(glm::mat4(1.0f));
	Transform* eyePupil = new Transform(glm::mat4(1.0f));
	eyeOutline->addChild(redSphere);
	eyeOutline->scale(glm::vec3(1, 1, .7));
	eyeIris->addChild(whiteSphere);
	eyeIris->scale(glm::vec3(.5, .5, .3));
	eyeIris->translate(glm::vec3(0, 0, 2.5));
	eyePupil->addChild(blackSphere);
	eyePupil->scale(glm::vec3(.3, .3, .4));
	eyePupil->translate(glm::vec3(0, 0, 2.2));
	eye->addChild(eyeOutline);
	eye->addChild(eyeIris);
	eye->addChild(eyePupil);

	// building complex transforms from geometry
	// lizard mouth
	Transform* lizardMouthTop = new Transform(glm::mat4(1.0f));
	lizardMouthTop->addChild(whiteSphere);
	lizardMouthTop->scale(glm::vec3(.8, .3, 1.5));
	lizardMouthTop->translate(glm::vec3(0, 1, .3));
	Transform* lizardMouthBottom = new Transform(glm::mat4(1.0f));
	lizardMouthBottom->addChild(whiteSphere);
	lizardMouthBottom->scale(glm::vec3(.8, .2, 1.3));
	lizardMouthBottom->translate(glm::vec3(0, -0.8, .3));
	//lizardMouthBottom->rotate(15);

	// pair of arms (???)
	Transform* arms = new Transform(glm::mat4(1.0f));
	arms->translate(glm::vec3(0, 0, -.1));
	Transform* leftArm = new Transform(glm::mat4(1.0f));
	arms->addChild(leftArm);
	Transform* rightArm = new Transform(glm::mat4(1.0f));
	arms->addChild(rightArm);
	Transform* leftBicep = new Transform(glm::mat4(1.0f));
	leftBicep->addChild(whiteSphere);
	leftBicep->scale(glm::vec3(.5, 1, .5));
	leftBicep->translate(glm::vec3(-2.1, 0, -.8));
	leftBicep->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	leftArm->addChild(leftBicep);
	Transform* rightBicep = new Transform(glm::mat4(1.0f));
	rightBicep->addChild(whiteSphere);
	rightBicep->scale(glm::vec3(.5, 1, .5));
	rightBicep->translate(glm::vec3(2.1, 0, -.8));
	rightBicep->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	rightArm->addChild(rightBicep);
	Transform* leftForearm = new Transform(glm::mat4(1.0f));
	leftForearm->addChild(redSphere);
	leftForearm->scale(glm::vec3(.3, 1.5, .3));
	leftForearm->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	leftForearm->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	leftForearm->translate(glm::vec3(-2.1, -.7, -0.75));
	leftBicep->addChild(leftForearm);
	Transform* rightForearm = new Transform(glm::mat4(1.0f));
	rightForearm->addChild(redSphere);
	rightForearm->scale(glm::vec3(.3, 1.5, .3));
	rightForearm->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	rightForearm->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	rightForearm->translate(glm::vec3(2.1, -.7, -0.75));
	rightBicep->addChild(rightForearm);
	Transform* rightOtherPair = new Transform(glm::mat4(1.0f));
	Transform* leftOtherPair = new Transform(glm::mat4(1.0f));
	arms->addChild(rightOtherPair);
	arms->addChild(leftOtherPair);
	rightOtherPair->addChild(rightBicep);
	leftOtherPair->addChild(leftBicep);
	rightOtherPair->translate(glm::vec3(0, 0, .5));
	leftOtherPair->translate(glm::vec3(0, 0, .5));
	rightOtherPair->scale(glm::vec3(.5, .5, .5));
	leftOtherPair->scale(glm::vec3(.5, .5, .5));
	// cat ears/horns
	Transform* horns = new Transform(glm::mat4(1.0f));
	Transform* hornBase = new Transform(glm::mat4(1.0f));
	hornBase->addChild(whiteSphere);
	horns->addChild(hornBase);
	Transform* leftHorn = new Transform(glm::mat4(1.0f));
	leftHorn->addChild(whiteSphere);
	leftHorn->scale(glm::vec3(.3, 2, .3));
	leftHorn->translate(glm::vec3(-1, .5, 0));
	hornBase->addChild(leftHorn);
	Transform* rightHorn = new Transform(glm::mat4(1.0f));
	rightHorn->addChild(whiteSphere);
	rightHorn->scale(glm::vec3(.3, 2, .3));
	rightHorn->translate(glm::vec3(1, .5, 0));
	hornBase->addChild(rightHorn);
	// a hat
	Transform* hat = new Transform(glm::mat4(1.0f));
	Transform* whiteHat = new Transform(glm::mat4(1.0f));
	Transform* blackHat = new Transform(glm::mat4(1.0f));
	whiteHat->addChild(whiteCylinder);
	blackHat->translate(glm::vec3(0, .5, 0));
	hat->addChild(whiteHat);
	hat->addChild(blackHat);
	Transform* brim = new Transform(glm::mat4(1.0f));
	brim->addChild(blackCylinder);
	brim->scale(glm::vec3(1.5, .3, 1.5));
	brim->translate(glm::vec3(0, 2.3, 0));
	blackHat->addChild(brim);
	Transform* crown = new Transform(glm::mat4(1.0f));
	crown->addChild(blackCylinder);
	crown->scale(glm::vec3(.6, 2.5, .6));
	crown->translate(glm::vec3(0, 1.2, 0));
	blackHat->addChild(crown);


	// transforms
	Transform* totemBodyT = new Transform(glm::mat4(1.0f));
	totemBodyT->scale(glm::vec3(1, 1, 1));
	totemBodyT->addChild(whiteCylinder);
	Transform* eagleMouthT = new Transform(glm::mat4(1.0f));
	Transform* mainEye = new Transform(glm::mat4(1.0));
	mainEye->addChild(eye);
	eagleMouthT->addChild(mainEye);
	eagleMouthT->scale(glm::vec3(.7, .7, .7));
	eagleMouthT->translate(glm::vec3(0, 0, 0.6));
	Transform* leftEye = new Transform(glm::mat4(1.0));
	leftEye->addChild(eye);
	leftEye->scale(glm::vec3(.5, .5, .5));
	leftEye->translate(glm::vec3(-2.5, 0, 0));
	leftEye->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	Transform* rightEye = new Transform(glm::mat4(1.0));
	rightEye->addChild(eye);
	rightEye->scale(glm::vec3(.5, .5, .5));
	rightEye->translate(glm::vec3(2.5, 0, 0));
	rightEye->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	eagleMouthT->addChild(leftEye);
	eagleMouthT->addChild(rightEye);

	Transform* lizardMouthT = new Transform(glm::mat4(1.0f));
	lizardMouthT->addChild(lizardMouthTop);
	lizardMouthT->addChild(lizardMouthBottom);

	Transform* whaleMouthT = new Transform(glm::mat4(1.0f));
	whaleMouthT->addChild(arms);
	Transform* eagleWingLT = new Transform(glm::mat4(1.0f));
	eagleWingLT->addChild(whiteSphere);
	eagleWingLT->scale(glm::vec3(3, .5, .01));
	Transform* eagleWingRT = new Transform(glm::mat4(1.0f));
	eagleWingRT->addChild(whiteSphere);
	eagleWingRT->scale(glm::vec3(3, .5, .01));
	Transform* albatrossWingLT = new Transform(glm::mat4(1.0f));
	albatrossWingLT->addChild(blackSphere);
	albatrossWingLT->scale(glm::vec3(3, .5, .01));
	Transform* albatrossWingRT = new Transform(glm::mat4(1.0f));
	albatrossWingRT->addChild(blackSphere);
	albatrossWingRT->scale(glm::vec3(3, .5, .01));
	Transform* catEarsT = new Transform(glm::mat4(1.0f));
	catEarsT->addChild(horns);
	Transform* hatEarsT = new Transform(glm::mat4(1.0f));
	hatEarsT->addChild(hat);

	// just construct all of the totem parts here and create another degree of indirection 
	// when applying transforms. This will make categorization a little easier.
	totemParts.push_back(new TotemPart(BODY_NOWING_MOUTH, totemBodyT));
	totemParts.push_back(new TotemPart(BODY_NOWING_NOMOUTH, totemBodyT));
	totemParts.push_back(new TotemPart(BODY_NOWING_NOMOUTH_NOBOTTOM, totemBodyT));
	totemParts.push_back(new TotemPart(BODY_WING_MOUTH, totemBodyT));
	totemParts.push_back(new TotemPart(BODY_WING_NOMOUTH, totemBodyT));
	totemParts.push_back(new TotemPart(MOUTH, lizardMouthT));
	totemParts.push_back(new TotemPart(MOUTH, eagleMouthT));
	totemParts.push_back(new TotemPart(MOUTH, whaleMouthT));
	totemParts.push_back(new TotemPart(LEFT_WING, eagleWingLT));
	totemParts.push_back(new TotemPart(LEFT_WING, albatrossWingLT));
	totemParts.push_back(new TotemPart(RIGHT_WING, eagleWingRT));
	totemParts.push_back(new TotemPart(RIGHT_WING, albatrossWingRT));
	totemParts.push_back(new TotemPart(EARS, catEarsT));
	totemParts.push_back(new TotemPart(EARS, hatEarsT));




}