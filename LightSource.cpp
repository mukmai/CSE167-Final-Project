#include "LightSource.h"


LightSource::LightSource(glm::vec3 light_color, glm::vec3 light_dir)
{
	this->lightType = 0;
	this->on = 1;
	this->light_color = light_color;
	this->light_dir = light_dir;
}


LightSource::LightSource(glm::vec3 light_color, glm::vec3 light_pos, float cons_att)
{
	this->lightType = 1;
	this->on = 1;
	this->light_color = light_color;
	this->light_pos = light_pos;
	this->cons_att = cons_att;
}


LightSource::~LightSource()
{
}

void LightSource::draw(GLuint shaderProgram, glm::mat4 C) {
	if (lightType == 0) {
		glUniform1i(glGetUniformLocation(shaderProgram, "dirLight.on"), on);
		glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.light_color"), light_color.x, light_color.y, light_color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.light_dir"), light_dir.x, light_dir.y, light_dir.z);
	}
	else if (lightType == 1) {
		glUniform1i(glGetUniformLocation(shaderProgram, "pointLight.on"), on);
		glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.light_color"), light_color.x, light_color.y, light_color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.light_pos"), light_pos.x, light_pos.y, light_pos.z);
		glUniform1f(glGetUniformLocation(shaderProgram, "pointLight.cons_att"), cons_att);
	}
}

void LightSource::update() {

}