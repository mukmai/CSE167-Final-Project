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

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static unsigned int seed;
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
};

#endif
