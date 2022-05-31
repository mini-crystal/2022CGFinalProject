#pragma once
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "glsl_program.h"
#include "texture.h"

class Prismatictable {
public:
	Prismatictable();

	Prismatictable(Prismatictable&& rhs) noexcept;

	//释放借来的所有资源
	~Prismatictable();

	void draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view);



private:
	GLuint _vao = 0;
	GLuint _vbo = 0;


	std::unique_ptr<TextureCubemap> _texture;

	std::unique_ptr<GLSLProgram> _shader;

	void cleanup();

	void generatevertices(GLfloat *vertices);

	glm::vec3 getpoint(GLfloat v, GLfloat r, GLfloat u);
};
