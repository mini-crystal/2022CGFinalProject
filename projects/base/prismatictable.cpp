#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "prismatictable.h"

using namespace std;

Prismatictable::Prismatictable() {
	GLfloat vertices[216] = { 0.0f };

	generatevertices(vertices);

	// create vao and vbo
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindVertexArray(0);

	try {


		const char* vsCode =
			"#version 330 core\n"
			"layout(location = 0) in vec3 aPosition;\n"
			"uniform mat4 model;\n"
			"uniform mat4 projection;\n"
			"uniform mat4 view;\n"
			"void main(){\n"
			"	gl_Position = projection * view * model * vec4(aPosition, 1.0);\n"
			"}\n";

		const char* fsCode =
			"#version 330 core\n"
			"out vec4 color;\n"
			"void main() {\n"
			"   color = vec4(1.0f, 0.4f, 0.4f, 1.0f);\n"
			"}\n";

		_shader.reset(new GLSLProgram);
		_shader->attachVertexShader(vsCode);
		_shader->attachFragmentShader(fsCode);
		_shader->link();
	}
	catch (const std::exception&) {
		cleanup();
		throw;
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::stringstream ss;
		ss << "Ball creation failure, (code " << error << ")";
		cleanup();
		throw std::runtime_error(ss.str());
	}
}

Prismatictable::Prismatictable(Prismatictable&& rhs) noexcept
	: _vao(rhs._vao),
	_vbo(rhs._vbo),
	_texture(std::move(rhs._texture)),
	_shader(std::move(rhs._shader)) {
	rhs._vao = 0;
	rhs._vbo = 0;
}


Prismatictable::~Prismatictable() {
	cleanup();
}



void Prismatictable::draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view) {


	_shader->use();
	_shader->setMat4("model", model);
	_shader->setMat4("view", view);
	_shader->setMat4("projection", projection);
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 216);



}


void Prismatictable::cleanup() {
	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_vao != 0) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}

void Prismatictable::generatevertices(GLfloat *vertices) {
	GLfloat v_step = 1.0f / 6;
	GLuint offset = 0;
	for (GLint v = 0; v < 6; v++) {
		glm::vec3 point1 = glm::vec3(0.0f, -0.5f, 0.0f);
		glm::vec3 point2 = getpoint((GLfloat)v * v_step, 0.5f, -0.5f);
		glm::vec3 point3 = getpoint((GLfloat)(v + 1) * v_step, 0.5f, -0.5f);

		glm::vec3 point4 = glm::vec3(0.0f, 0.5f, 0.0f);
		glm::vec3 point5 = getpoint((GLfloat)v * v_step, 0.2f, 0.5f);
		glm::vec3 point6 = getpoint((GLfloat)(v + 1) * v_step, 0.2f, 0.5f);

		memcpy(vertices + offset, glm::value_ptr(point1), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point3), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point2), 3 * sizeof(GLfloat));
		offset += 3;

		memcpy(vertices + offset, glm::value_ptr(point3), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point2), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point5), 3 * sizeof(GLfloat));
		offset += 3;

		memcpy(vertices + offset, glm::value_ptr(point3), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point6), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point5), 3 * sizeof(GLfloat));
		offset += 3;

		memcpy(vertices + offset, glm::value_ptr(point4), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point6), 3 * sizeof(GLfloat));
		offset += 3;
		memcpy(vertices + offset, glm::value_ptr(point5), 3 * sizeof(GLfloat));
		offset += 3;
	}
}


glm::vec3 Prismatictable::getpoint(GLfloat v, GLfloat r, GLfloat y) {
	glm::vec3 point;
	GLfloat pi = 3.1415f;

	point.x = r * glm::sin(2 * pi * v);
	point.z = r * glm::cos(2 * pi * v);
	point.y = y;

	return point;
}