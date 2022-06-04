#pragma once

#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>

#include "vertex.h"
#include "object3d.h"
#include "bounding_box.h"
#include "../base/objloader.h"


class ObjModel : public Object3D {
public:
    ObjModel(const std::string& filepath);

    ObjModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

    ObjModel(ObjModel&& rhs) noexcept;

    virtual ~ObjModel();

    GLuint getVao() const;

    GLuint getBoundingBoxVao() const;

    size_t getVertexCount() const;

    size_t getFaceCount() const;

    BoundingBox getBoundingBox() const;

    virtual void draw() const;

    virtual void drawBoundingBox() const;

protected:
    // vertices of the table represented in model's own coordinate
    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;


    // bounding box
    BoundingBox _boundingBox;

    // opengl objects
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;

    GLuint _boxVao = 0;
    GLuint _boxVbo = 0;
    GLuint _boxEbo = 0;

    void computeBoundingBox();

    void initGLResources();

    void initBoxGLResources();

    void cleanup();
};