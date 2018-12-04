#ifndef PJ3_TEXTURE    // To make sure you don't declare the function more than once by including the header multiple times.
#define PJ3_TEXTURE
#include <stdlib.h>
#include <stdio.h>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include <GL/glew.h>
#include <vector>

unsigned char* loadPPM(const char* filename, int& width, int& height);
void loadTexture(const char* filename);
GLuint loadCubeMap(std::vector<const char*> filenames);

#endif