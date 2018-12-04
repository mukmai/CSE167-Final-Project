#include "window.h"

const char* window_title = "GLFW Starter Project";

CubeMap* cubemap;
Transform* world;
Transform* cubemapS;
Transform* trackS;
Geometry* sphere;
Transform* controlPtS;
Transform* sphereS;
Track* track;
std::vector<AnchorPoint*> anchorList;
std::vector<ControlHandle*> controlHandleList;

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
glm::vec3 lastPoint;
glm::vec3 curPoint;
glm::mat4 VM = glm::mat4(1.0f);
glm::mat4 VOrig = glm::mat4(1.0f);

AnchorPoint* curAnchor;
ControlHandle* curHandle;
int currentPoint = 0;
float moveScalar = 0.1;

float trackScalar = 8.0f;

glm::mat4 regularV;
glm::mat4 riderV;
bool riderView = false;

void Window::initialize_objects()
{
	textureFiles.push_back("Skybox_Water222_right.ppm"); // right
	textureFiles.push_back("Skybox_Water222_left.ppm"); // left
	textureFiles.push_back("Skybox_Water222_top.ppm"); // top
	textureFiles.push_back("Skybox_Water222_base.ppm"); // bottom
	textureFiles.push_back("Skybox_Water222_front.ppm"); // front
	textureFiles.push_back("Skybox_Water222_back.ppm"); // back

	sphere = new Geometry("sphere.obj");

	world = new Transform(glm::mat4(1.0f));
	cubemapS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(500.0f)));
	trackS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(trackScalar)));
	cubemap = new CubeMap(textureFiles);

	sphereS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(1/ trackScalar)));
	controlPtS = new Transform(glm::scale(glm::mat4(1.0f), glm::vec3(0.02f)));
	controlHandleList.push_back(new ControlHandle(glm::vec3(0.5, 0, 0.2), controlPtS));
	controlHandleList.push_back(new ControlHandle(glm::vec3(0.1, 0, 0.5), controlPtS));
	controlHandleList.push_back(new ControlHandle(glm::vec3(0.5, 0, -0.2), controlPtS));
	controlHandleList.push_back(new ControlHandle(glm::vec3(-0.5, 0, -0.2), controlPtS));
	controlHandleList.push_back(new ControlHandle(glm::vec3(-0.5, 0, -0.2), controlPtS));
	controlHandleList.push_back(new ControlHandle(glm::vec3(0.5, 0, -0.2), controlPtS));
	controlHandleList.push_back(new ControlHandle(glm::vec3(0.5, 0, 0.2), controlPtS));
	controlHandleList.push_back(new ControlHandle(glm::vec3(0.5, 0, 0.2), controlPtS));
	anchorList.push_back(new AnchorPoint(glm::vec3(2, 0, 2), controlPtS, controlHandleList[0]));
	anchorList.push_back(new AnchorPoint(glm::vec3(3, 1, 1), controlPtS, controlHandleList[1]));
	anchorList.push_back(new AnchorPoint(glm::vec3(3, 2, -1), controlPtS, controlHandleList[2]));
	anchorList.push_back(new AnchorPoint(glm::vec3(1, 1, -1), controlPtS, controlHandleList[3]));
	anchorList.push_back(new AnchorPoint(glm::vec3(-1, 0, -2), controlPtS, controlHandleList[4]));
	anchorList.push_back(new AnchorPoint(glm::vec3(-2, -2, 0), controlPtS, controlHandleList[5]));
	anchorList.push_back(new AnchorPoint(glm::vec3(-2, 1, 2), controlPtS, controlHandleList[6]));
	anchorList.push_back(new AnchorPoint(glm::vec3(0, 3, 3), controlPtS, controlHandleList[7]));

	track = new Track(anchorList, sphereS);
	track->minVelocity = 0.4;
	track->maxVelocity = 4;
	sphereS->addChild(sphere);

	world->addChild(cubemapS);
	cubemapS->addChild(cubemap);
	world->addChild(trackS);
	trackS->addChild(track);
	controlPtS->addChild(sphere);
	
	curAnchor = anchorList[0];
	curAnchor->chosen = true;
	curHandle = controlHandleList[0];

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
	delete(world);
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
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

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
}

void Window::display_callback(GLFWwindow* window)
{
	if (riderView) {
		V = track->getRiderView();
	}
	else {
		V = regularV;
	}
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	glLineWidth(2);
	world->draw(shaderProgram, glm::mat4(1.0f));

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
	else if (key == GLFW_KEY_W) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(-2.0f), glm::vec3(1, 0, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(0, 0, 0.8f));
		}
			
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_S) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(2.0f), glm::vec3(1, 0, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(0, 0, -0.8f));
		}
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_A) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(-2.0f), glm::vec3(0, 1, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(0.8f, 0, 0));
		}
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_D) {
		if (mods == GLFW_MOD_SHIFT) {
			VM = glm::rotate(VM, glm::radians(2.0f), glm::vec3(0, 1, 0));
		}
		else {
			VM = glm::translate(VM, glm::vec3(-0.8f, 0, 0));
		}
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_E) {
		VM = glm::translate(VM, glm::vec3(0, -0.8f, 0));
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_Q) {
		VM = glm::translate(VM, glm::vec3(0, 0.8f, 0));
		regularV = VM * VOrig;
	}
	else if (key == GLFW_KEY_X) {
		if (mods == GLFW_MOD_SHIFT) {
			if (currentPoint % 2 == 0) curAnchor->pos += glm::vec3(moveScalar, 0, 0);
			else {
				curHandle->offset += glm::vec3(moveScalar, 0, 0);
				curHandle->update();
			}
		}
		else {
			if (currentPoint % 2 == 0) curAnchor->pos += glm::vec3(-moveScalar, 0, 0);
			else {
				curHandle->offset += glm::vec3(-moveScalar, 0, 0);
				curHandle->update();
			}
		}
		track->update();
	}
	else if (key == GLFW_KEY_Y) {
		if (mods == GLFW_MOD_SHIFT) {
			if (currentPoint % 2 == 0) curAnchor->pos += glm::vec3(0, moveScalar, 0);
			else {
				curHandle->offset += glm::vec3(0, moveScalar, 0);
				curHandle->update();
			}
		}
		else {
			if (currentPoint % 2 == 0) curAnchor->pos += glm::vec3(0, -moveScalar, 0);
			else {
				curHandle->offset += glm::vec3(0, -moveScalar, 0);
				curHandle->update();
			}
		}
		track->update();
	}
	else if (key == GLFW_KEY_Z) {
		if (mods == GLFW_MOD_SHIFT) {
			if (currentPoint % 2 == 0) curAnchor->pos += glm::vec3(0, 0, moveScalar);
			else {
				curHandle->offset += glm::vec3(0, 0, moveScalar);
				curHandle->update();
			}
		}
		else {
			if (currentPoint % 2 == 0) curAnchor->pos += glm::vec3(0, 0, -moveScalar);
			else {
				curHandle->offset += glm::vec3(0, 0, -moveScalar);
				curHandle->update();
			}
		}
		track->update();
	}
	else if (key == GLFW_KEY_0) {
		curAnchor->chosen = false;
		curHandle->chosen = false;
		currentPoint += 1;
		if (currentPoint == (anchorList.size() + controlHandleList.size())) currentPoint = 0;
		if (currentPoint % 2 == 0) {
			curAnchor = anchorList[currentPoint / 2];
			curAnchor->chosen = true;
		}
		else {
			curHandle = controlHandleList[currentPoint / 2];
			curHandle->chosen = true;
		}
	}
	else if (key == GLFW_KEY_P) {
		track->moveSphere = !(track->moveSphere);
	}
	else if (key == GLFW_KEY_C) {
		riderView = !riderView;
		if (riderView) {
			cubemapS->scale(glm::vec3(0.1, 0.1, 0.1));
		}
		else {
			cubemapS->scale(glm::vec3(10, 10, 10));
		}
		
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
			lastPoint = trackBallMapping(window, x, y);
		}
		else if (GLFW_RELEASE == action) {
			lbutton_down = false;
		}
	}
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	if (lbutton_down) {
		curPoint = trackBallMapping(window, xpos, ypos);
		float velocity = glm::sqrt(glm::dot((curPoint - lastPoint), (curPoint - lastPoint)));
		if (velocity > 0.0001) {
			glm::vec3 rotAxis = glm::cross(lastPoint, curPoint);
			float rotAngle = glm::acos(glm::dot(lastPoint, curPoint) / (length(lastPoint) * length(curPoint)));
			VM = glm::rotate(VM, rotAngle / 3, rotAxis);
			regularV = VM * VOrig;
		}

		lastPoint = curPoint;
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

void Window::drawLine(GLuint shaderProgram, glm::mat4 M) {

}