#pragma once
#pragma once


#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "glsl_program.h"
#include "texture.h"

class Tetrahedron {
public:
	Tetrahedron();

	Tetrahedron(Tetrahedron&& rhs) noexcept;

	//释放借来的所有资源
	~Tetrahedron();

	void draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view);



private:
	GLuint _vao = 0;
	GLuint _vbo = 0;


	std::unique_ptr<TextureCubemap> _texture;

	std::unique_ptr<GLSLProgram> _shader;

	void cleanup();
};
