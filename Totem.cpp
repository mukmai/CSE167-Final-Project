#include "Totem.h"
#include "Window.h"

#include <time.h>

Totem::Totem(std::vector<TotemPart*> totemParts, int counterVal) {
	srand(Window::seed + counterVal);

	// categorize all parts before using them
	createCategories(totemParts);

	float initRotation = rand() % 360;
	float initRotation2 = 15 - rand() % 30;
	std::cout << "initRotation: " << initRotation << std::endl;

	// set the totem's main transformation matrix
	this->transform = new Transform(glm::mat4(1.0f));
	this->transform->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(initRotation), glm::vec3(0.0f, 1.0f, 0.0f)));
	this->transform->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(initRotation2), glm::vec3(1.0f, 0.0f, 0.0f)));

	this->runningCount = 0;
	this->rotateVal = 25.0f;

	// build a random totem
	generateTotem(counterVal);

	this->transform->translate(glm::vec3(0, -.1, 0.0));
}

Totem::~Totem() 
{
	delete(transform);
	for (int i = 0; i < this->parts.size(); i++) {
		delete(parts[i]);
	}
	//being triply sure
	for (int i = 0; i < top.size(); i++) {
		delete(top[i]);
	}
	for (int i = 0; i < bottom.size(); i++) {
		delete(bottom[i]);
	}
	for (int i = 0; i < left.size(); i++) {
		delete(left[i]);
	}
	for (int i = 0; i < right.size(); i++) {
		delete(right[i]);
	}
	for (int i = 0; i < mouth.size(); i++) {
		delete(mouth[i]);
	}
}

void Totem::generateTotem(int counterVal) {
	srand(time(NULL) + counterVal);

	// for containing reference to the bottommost piece, set it as the first (and only) child of the Totem
	TotemPart* basePiece = new TotemPart(BODY_NOWING_NOMOUTH_NOBOTTOM, this->bodies[0]);
	this->transform->addChild(basePiece);

	std::vector<TotemPart*> queue;
	queue.push_back(basePiece);
	parts.push_back(basePiece);

	// prevent building infinite structures
	int counter = 0;
	int maxCounter = 40;

	while (queue.size() > 0 && counter <= maxCounter) {
		std::cout << "counter: " << counter << std::endl;
		counter++;
		runningCount++;

		TotemPart* currentPiece = queue[0];
		std::cout << "current piece's id: " << currentPiece->getId() << std::endl;

		// pieces that will be randomly assigned
		// there are more optimal ways to do this, but I am lazy and reaping the fruits of my actions :(
		int assignedTop, assignedBottom, assignedLeft, assignedRight, assignedMouth;
		TotemPart* randomTop = getRandomTop();
		TotemPart* randomBottom = getRandomBottom();
		TotemPart* randomLeft = getRandomLeft();
		TotemPart* randomRight = getRandomRight();
		TotemPart* randomMouth = getRandomMouth();
		
		parts.push_back(randomTop);
		parts.push_back(randomBottom);
		parts.push_back(randomLeft);
		parts.push_back(randomRight);
		parts.push_back(randomMouth);

		// assignment status. this will prevent us from attempting to reassign pointers
		assignedTop = currentPiece->setTop(randomTop);
		assignedBottom = currentPiece->setBottom(randomBottom);
		assignedLeft = currentPiece->setLeft(randomLeft);
		assignedRight = currentPiece->setRight(randomRight);
		assignedMouth = currentPiece->setMouth(randomMouth);
	
		float val = 1.25;
		this->rotateVal -= 5.5;

		// if there were any pieces that were successfully assigned, assign the reverse pointer 
		// in the child and push the child element into the queue. Also make sure it's assigned as a child to its
		// previous piece, and update the child's transform appropriately so that it's a little offset from the 
		// current piece
		if (assignedTop == 0) {
			currentPiece->getTransform()->addChild(randomTop);
				
			randomTop->getTransform()->translate(glm::vec3(0, val, 0));
			//randomTop->getTransform()->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(this->rotateVal), glm::vec3(0.0f, 1.0f, 0.0f)));
			randomTop->setBottom(currentPiece);
			queue.push_back(randomTop);
			std::cout << "assigned a top to the current part" << std::endl;
		}
		if (assignedBottom == 0) { // according to current pieces, this should NEVER be called
			currentPiece->getTransform()->addChild(randomBottom);
			randomBottom->getTransform()->translate(glm::vec3(0, -1 * val, 0));
			randomBottom->setTop(currentPiece);
			//queue.push_back(randomBottom);
			std::cout << "assigned a bottom to the current part" << std::endl;
		}
		if (assignedLeft == 0) {
			currentPiece->getTransform()->addChild(randomLeft);
			randomLeft->getTransform()->translate(glm::vec3(-1 * val, 0, 0));
			//randomLeft->getTransform()->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(this->rotateVal), glm::vec3(0.0f, 1.0f, 0.0f)));
			//randomLeft->setRight(currentPiece);
			//queue.push_back(randomLeft);
			std::cout << "assigned a left to the current part" << std::endl;
		}
		if (assignedRight == 0) {
			currentPiece->getTransform()->addChild(randomRight);
			randomRight->getTransform()->translate(glm::vec3(val, 0, 0));
			//randomRight->getTransform()->rotate(glm::rotate(glm::mat4(1.0f), glm::radians(this->rotateVal), glm::vec3(0.0f, 1.0f, 0.0f)));
			//randomRight->setLeft(currentPiece);
			//queue.push_back(randomRight);
			std::cout << "assigned a right to the current part" << std::endl;
		}
		if (assignedMouth == 0) {
			currentPiece->getTransform()->addChild(randomMouth);
			randomMouth->getTransform()->translate(glm::vec3(0, 0, val / 2.0));
			randomMouth->getTransform()->scale(glm::vec3(.8, 1, .8));
			//randomMouth->setMouth(currentPiece);
			//queue.push_back(randomMouth);
			std::cout << "assigned a mouth to the current part" << std::endl;
		}

		// save the current piece so it doesn't get lost during destruction
		this->parts.push_back(queue[0]);
		// remove the current piece from the queue
		queue.erase(queue.begin());

	}
}

void Totem::createCategories(std::vector<TotemPart*> totemParts) {
	// need to categorize each of the totemParts into positional and body part categories
	for (int i = 0; i < totemParts.size(); i++) {
		int id = totemParts[i]->getId();
		TotemPart* curr = totemParts[i];

		if (id == BODY_NOWING_MOUTH) {
			this->top.push_back(curr);
			this->bottom.push_back(curr);

			this->bodies.push_back(curr);
		}
		else if (id == BODY_NOWING_NOMOUTH) {
			this->top.push_back(curr);
			this->bottom.push_back(curr);

			this->bodies.push_back(curr);
		}
		else if (id == BODY_NOWING_NOMOUTH_NOBOTTOM) {
			this->top.push_back(curr);

			this->bodies.push_back(curr);
		}
		else if (id == BODY_WING_MOUTH) {
			this->top.push_back(curr);
			this->bottom.push_back(curr);

			// rigging the game
			this->top.push_back(curr);
			this->top.push_back(curr);
			this->top.push_back(curr);
			this->top.push_back(curr);
			this->top.push_back(curr);
			this->top.push_back(curr);

			this->bodies.push_back(curr);
		}
		else if (id == BODY_WING_NOMOUTH) {
			this->top.push_back(curr);
			this->bottom.push_back(curr);
			
			this->bodies.push_back(curr);
		}
		else if (id == EARS) { // ensure totem is at least 4 pieces high
			this->top.push_back(curr);

			this->ears.push_back(curr);
		}
		else if (id == MOUTH) {
			this->mouth.push_back(curr);

			this->mouths.push_back(curr);
		}
		else if (id == RIGHT_WING) {
			this->right.push_back(curr);

			this->rightWings.push_back(curr);
		}
		else if (id == LEFT_WING) {
			this->left.push_back(curr);

			this->leftWings.push_back(curr);
		}
	}
}

TotemPart* Totem::getRandomTop() {
	std::cout << "finding a top" << std::endl;
	int r = rand() % this->top.size();
	std::cout << "rand is: " << r << std::endl;
	return new TotemPart(this->top[r]);
}

TotemPart* Totem::getRandomBottom() {
	std::cout << "finding a bottom" << std::endl;
	int r = rand() % this->bottom.size();
	std::cout << "rand is: " << r << std::endl;
	return new TotemPart(this->bottom[r]);
}

TotemPart* Totem::getRandomLeft() {
	std::cout << "finding a left" << std::endl;
	int r = rand() % this->left.size();
	std::cout << "rand is: " << r << std::endl;
	return new TotemPart(this->left[r]);
}

TotemPart* Totem::getRandomRight() {
	std::cout << "finding a right" << std::endl;
	int r = rand() % this->right.size();
	std::cout << "rand is: " << r << std::endl;
	return new TotemPart(this->right[r]);
}

TotemPart* Totem::getRandomMouth() {
	std::cout << "finding a mouth" << std::endl;
	int r = rand() % this->mouth.size();
	std::cout << "rand is: " << r << std::endl;
	return new TotemPart(this->mouth[r]);
}

void Totem::draw(GLuint shaderProgram, glm::mat4 C) {
	this->transform->draw(shaderProgram, C);
}

void Totem::update() {

}

Transform* Totem::getTransform() {
	return this->transform;
}