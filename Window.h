#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "shader.h"
#include <vector>
#include "Project3-texture.h"
#include "Transform.h"
#include "CubeMap.h"
#include "Geometry.h"
#include "OBJObject.h"
#include "LightSource.h"
#include "Terrain.h"
#include "Player.h"
#include "PlayerBody.h"
#include "ParticleManager.h"
#include "Splash.h"
#include "Totem.h"
#include "TotemPart.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "LSystem.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static bool playerView;
	static bool toon_shade;
	static unsigned int seed;
	static bool endGame, endFinished;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static glm::vec3 trackBallMapping(GLFWwindow* window, double x, double y);
	static void initializeTerrain();
	static void initializeTotem();
private:
	const static int BODY_NOWING_NOMOUTH = 0;
	const static int BODY_NOWING_MOUTH = 1;
	const static int BODY_WING_MOUTH = 2;
	const static int BODY_NOWING_NOMOUTH_NOBOTTOM = 3;
	const static int BODY_WING_NOMOUTH = 8;
	const static int MOUTH = 4;
	const static int EARS = 5;
	const static int LEFT_WING = 6;
	const static int RIGHT_WING = 7;
};

#endif
