#include "Project3-texture.h"

using namespace std; // allow console printouts without std::

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
		will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ( (fp=fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width  = atoi(buf[1]);
	height = atoi(buf[2]);
	printf("texture w: %d  h: %d\n", width, height);

	// Read maxval:
	do
	{
	  retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

// load image file into texture object
void loadTexture(const char* filename)
{
  GLuint texture[1];     // storage for one texture
  int twidth, theight;   // texture width/height [pixels]
  unsigned char* tdata;  // texture pixel data
  
  // Load image file
  tdata = loadPPM(filename, twidth, theight);
  if (tdata==NULL) return;
  
  // Create ID for texture
  glGenTextures(1, &texture[0]);   

  // Set this texture to be the one we are working with
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  
  // Generate the texture
  glTexImage2D(GL_TEXTURE_2D, // type of texture
	  0, // mimap level
	  GL_RGB, // internal representation of the texture in GPU
	  twidth, theight, // texture width and height
	  0, // should always be 0
	  GL_RGB, // representation of the texture that we are sending
	  GL_UNSIGNED_BYTE, // type of individual values in our images
	  tdata); // memory location where the pixel information is stored
  
  // Set bi-linear filtering for both minification and magnification
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint loadCubeMap(vector<const char*> filenames)
{
	GLuint textureID;

	vector<unsigned char*> tdata;  // texture pixel data
	vector<int> twidth, theight;   // texture width/height [pixels]

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Load image file
	for (GLuint i = 0; i < filenames.size(); i++) {
		tdata.push_back(NULL);
		twidth.push_back(NULL);
		theight.push_back(NULL);
		tdata[i] = loadPPM(filenames[i], twidth[i], theight[i]);
		if (tdata[i] == NULL) {
			printf("invalid file\n");
			return NULL;
		}
	}

	for (GLuint i = 0; i < tdata.size(); i++)
	{
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, twidth[i], theight[i], 0, GL_RGB, GL_UNSIGNED_BYTE, tdata[i]
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}