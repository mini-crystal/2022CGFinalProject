#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../base/model.h"
#include "ground.h"


Ground::Ground() {
    std::string groundTexturePath="../../media/ground.png";
    GLfloat vertices[] = {
        -50.0f,  50.0f, -50.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         50.0f,  50.0f, -50.0f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         50.0f,  50.0f,  50.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         50.0f,  50.0f,  50.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -50.0f,  50.0f,  50.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -50.0f,  50.0f, -50.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    };

    // create vao and vbo
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    std::shared_ptr<Texture2D> groundTexture = std::make_shared<Texture2D>(groundTexturePath);
    glActiveTexture(GL_TEXTURE0);
    groundTexture->bind();

    try {
        const char* vsCode =
            "#version 330 core\n"
            
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "layout (location = 2) in vec2 aTexCoord;\n"
        
            "uniform mat4 model;\n"
            "uniform mat4 projection;\n"
            "uniform mat4 view;\n"
        
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"

            "void main()\n"
            "{\n"
            "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
            "    ourColor = aColor;\n"
            "    TexCoord = vec2(aTexCoord.x,aTexCoord.y);\n"
            "}\n";

        const char* fsCode =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
        
            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"
            "uniform sampler2D ourTexture;\n"
            "void main()\n"
            "{\n"
            "    FragColor = texture(ourTexture, TexCoord);\n"
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

Ground::Ground(Ground&& rhs) noexcept
    : _vao(rhs._vao),
    _vbo(rhs._vbo),
    _texture(std::move(rhs._texture)),
    _shader(std::move(rhs._shader)) {
    rhs._vao = 0;
    rhs._vbo = 0;
}


Ground::~Ground() {
    cleanup();
}


void Ground::draw(const glm::mat4& projection, const glm::mat4& view) {

    _shader->use();
    glm::mat4 model=glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -50.19f, 0.0f));
    _shader->setMat4("model", model);
    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}


void Ground::cleanup() {
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}
