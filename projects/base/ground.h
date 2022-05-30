#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "glsl_program.h"
#include "texture.h"

class Ground {
public:
    Ground();

    Ground(Ground&& rhs) noexcept;

    // Õ∑≈ΩË¿¥µƒÀ˘”–◊ ‘¥
    ~Ground();

    void draw(const glm::mat4& projection, const glm::mat4& view);



private:
    const std::string groundTexturePath="../../media/ground.jpg";
    std::shared_ptr<Texture2D> groundTexture = std::make_shared<Texture2D>(groundTexturePath);
    
    GLuint _vao = 0;
    GLuint _vbo = 0;


    std::unique_ptr<TextureCubemap> _texture;

    std::unique_ptr<GLSLProgram> _shader;

    void cleanup();
};

