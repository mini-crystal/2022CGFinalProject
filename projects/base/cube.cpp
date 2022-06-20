#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "cube.h"


Cube::Cube() {
    GLfloat vertices[] = {
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f
    };




    // create vao and vbo
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

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
            "   color = vec4(1.0f, 0.870f, 0.0f, 1.0f);\n"
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
        ss << "Cube creation failure, (code " << error << ")";
        cleanup();
        throw std::runtime_error(ss.str());
    }
}

Cube::Cube(Cube&& rhs) noexcept
    : _vao(rhs._vao),
    _vbo(rhs._vbo),
    _texture(std::move(rhs._texture)),
    _shader(std::move(rhs._shader)) {
    rhs._vao = 0;
    rhs._vbo = 0;
}


Cube::~Cube() {
    cleanup();
}



void Cube::draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view) {


    _shader->use();
    _shader->setMat4("model", model);
    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);



}


void Cube::cleanup() {
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}

glm::mat4 Cube::getModelMatrix() const {
    return glm::translate(glm::mat4(1.0f), position) *
        glm::mat4_cast(rotation) *
        glm::scale(glm::mat4(1.0f), scale);
}
