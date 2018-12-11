#include "LSystem.h"
#include "Window.h"
#include <cstdlib>

LSystem::LSystem()
{
	lineWidth = 5;
	initializeTrees();
}

// Generate some number of trees and put them into a vector of string,int pairs
// where string is the fully expanded tree to draw, and int is the type of tree
void LSystem::initializeTrees()
{
	// Generate 10 trees
	for (int i = 0; i < 10; i++) {
		// Generate random type
		//srand(time(NULL));
		type = (rand() % 3) + 1;
		depth = (rand() % 3) + 4;
		axiom = "X";
		for (int i = 0; i <= depth; i++) {
			axiom = expand(type, axiom);
		}
		trees.push_back(make_pair(axiom, type));
	}
}

// '[' constant
void LSystem::push()
{
	glPushMatrix();
	// Branches are thicker at bottom
	if (lineWidth > 0) {
		lineWidth -= 1;
	}
}

// ']' constant
void LSystem::pop()
{
	glPopMatrix();
	// Branches are thinner at top
	lineWidth += 1;
}

// Rotate left by some angle theta '-'
void LSystem::rotateLeft(float theta)
{
	glRotatef(theta, 1, 0, 0);
	glRotatef(theta * 4, 0, 1, 0);
	//glRotatef(theta, 0, 0, 1); To keep or not to keep
}

// Rotate right by some angle theta '+'
void LSystem::rotateRight(float theta)
{
	glRotatef(-theta, 1, 0, 0);
	glRotatef(theta * 4, 0, 1, 0);
	//glRotatef(-theta, 0, 0, 1); To keep or not to keep
}

// Add a leaf to the plants 'L'
void LSystem::addLeaf()
{
	glPushAttrib(GL_LIGHTING_BIT); // Save current lighting
	// glColor3f(0.50, 1.0, 0.0);
	GLfloat ambient[4] = { 0.50, 1.0, 0.0 }; // ambient reflection
	GLfloat specular[4] = { 0.55, 1.0, 0.0 }; // specular reflection
	GLfloat diffuse[4] = { 0.50, 0.9, 0.0 }; // diffuse reflection

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient); // set the ambient reflection for the object

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse); // set the diffuse reflection for the object

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular); // set the specular reflection for the object      

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0); // set the size of the specular highlights

	// Draw leaf with triangles
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5, 0, 0.7);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(-0.5, 0, -0.7);
	glVertex3f(0, 1, 0);
	glEnd();
	glPopAttrib();
}

// Draw the branches with lines (turtle moving forward) 'F'
void LSystem::moveForward(float length)
{
	glPushAttrib(GL_LIGHTING_BIT); // Save current lighting

	//glColor3f(0.55, 0.27, 0.07);
	GLfloat ambient[4] = { 0.55, 0.27, 0.07 }; // ambient reflection
	GLfloat diffuse[4] = { 0.55, 0.27, 0.07 }; // diffuse reflection

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient); // set the ambient reflection for the object

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse); // set the diffuse reflection for the object

	// Draw the branch in the y-direction
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);
	glEnd();

	// Move turtle to end of line
	glTranslatef(0, length, 0);
	glPopAttrib();
}

// Draw the entire tree after the production rule has been applied 'depth' number of times
void LSystem::draw()
{
	string nextChar = ""; // Hold the value of the next char in the production string
	// Draw all the trees in the vector
	for (int x = 0; x < trees.size(); x++) {
		for (int i = 0; i < trees[x].first.length(); i++) {
			nextChar = trees[x].first.at(i); // Read the next character in the string

			// Move forward by some length
			if (nextChar.compare("F") == 0 || nextChar.compare("X") == 0) {
				if (trees[x].second == 1) {
					moveForward(0.001);
				}

				else if (trees[x].second == 2) {
					moveForward(0.001);
				}

				else if (trees[x].second == 3) {
					moveForward(0.39);
				}
			}

			// Push current variables onto stack
			else if (nextChar.compare("[") == 0) {
				push();
			}

			// Restore stack variables to previous "["
			else if (nextChar.compare("]") == 0) {
				pop();
			}

			// Draw a leaf
			else if (nextChar.compare("L") == 0) {
				addLeaf();
			}

			// Rotate turtle right by some theta
			else if (nextChar.compare("+") == 0) {
				if (trees[x].second == 1) {
					rotateRight(25.0);
				}

				else if (trees[x].second == 2) {
					rotateRight(15.0);
				}

				else if (trees[x].second == 3) {
					rotateRight(35.7);
				}
			}

			// Rotate turtle left by some theta
			else if (nextChar.compare("-") == 0) {
				if (trees[x].second == 1) {
					rotateLeft(25.0);
				}

				else if (trees[x].second == 2) {
					rotateLeft(15.0);
				}

				else if (trees[x].second == 3) {
					rotateLeft(35.7);
				}
			}
		}
	}
}

// Apply production rule, rule depends on type
string LSystem::expand(int type, string startString)
{
	string nextChar = ""; // Hold the value of the next char in the L-System string
	// One type of a tree
	if (type == 1) {
		for (int i = 0; i < startString.length(); i++) {
			nextChar = startString.at(i);

			// Replace variable F with production rule
			if (nextChar.compare("F") == 0) {
				startString.replace(i, 1, "FF");
				i = i + 1; // Move to next char in string based on length of production rule
			}

			// Replace variable X with production rule
			else if (nextChar.compare("X") == 0) {
				// Do a seed random here to either make branches left or right?
				if (1) {
					// startString.replace(i, 1, "F[-X]F[+X]-X"); // no leaves
					startString.replace(i, 1, "F[-XL]F[+XL]-X"); // leaves
				}

				else {
					//LSystem.replace(i, 1, "F[+X]F[-X]+X"); no leaves
					startString.replace(i, 1, "F[+XL]F[-XL]+X"); // leaves
				}
				i = i + 13;	// Move to next char in string based on length of production rule
			}
			//cout << "String: " << startString << " after " << i << " iteration." << endl;
		}
	}

	else if (type == 2) {
		for (int i = 0; i < startString.length(); i++) {
			nextChar = startString.at(i);

			// Replace variable F with production rule
			if (nextChar.compare("F") == 0) {
				startString.replace(i, 1, "FF");
				i = i + 1; // Move to next char in string based on length of production rule
			}

			// Replace variable X with production rule
			else if (nextChar.compare("X") == 0) {
				// Do a seed random here to either make branches left or right?
				if (1) {
					// startString.replace(i, 1, "F+[[X]-X]-F[-FX]+X"); // no leaves
					startString.replace(i, 1, "F+[[XL]-X]-F[-FXL]+X"); // leaves
				}

				else {
					//LSystem.replace(i, 1, "F-[[X]+X]+F[+FX]-X"); no leaves
					startString.replace(i, 1, "F-[[XL]+X]+F[+FXL]-X"); // leaves
				}
				i = i + 19;	// Move to next char in string based on length of production rule
			}
			//cout << "String: " << startString << " after " << i << " iteration." << endl;
		}
	}

	else if (type == 3) {
		for (int i = 0; i < startString.length(); i++) {
			nextChar = startString.at(i);

			// Replace variable F with production rule
			if (nextChar.compare("F") == 0) {
				startString.replace(i, 1, "FF");
				i = i + 1; // Move to next char in string based on length of production rule
			}

			// Replace variable X with production rule
			else if (nextChar.compare("X") == 0) {
				// Do a seed random here to either make branches left or right?
				if (1) {
					// startString.replace(i, 1, "F[+X][-X]FX"); // no leaves
					startString.replace(i, 1, "FL[+XL][-XL]FXL"); // leaves
				}

				else {
					//LSystem.replace(i, 1, "F[-X][+X]FX"); no leaves
					startString.replace(i, 1, "FL[-XL][+XL]FXL"); // leaves
				}
				i = i + 14;	// Move to next char in string based on length of production rule
			}
			//cout << "String: " << startString << " after " << i << " iteration." << endl;
		}
	}
	//tree->push_back(startString);
	return startString;
}

// Make the plants blow in the wind
void LSystem::update()
{
}
