#pragma once
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "glsl_program.h"
#include "texture.h"

class Cube {
public:
	Cube();
	Cube(Cube&& rhs) noexcept;
	~Cube();

    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	void draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view);
    glm::mat4 getModelMatrix()const;


private:
	GLuint _vao = 0;
	GLuint _vbo = 0;


	std::unique_ptr<TextureCubemap> _texture;

	std::unique_ptr<GLSLProgram> _shader;

	void cleanup();
    

};
