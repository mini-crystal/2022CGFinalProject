#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "glsl_program.h"
#include "texture.h"

using namespace std;

class BezierFace
{
	////顶点个数
	//int numVertices;
	////索引个数
	int numIndices;
	//顶点
	//vector<glm::vec3> vertices;
	//法线
	//vector<glm::vec3> normals;
	//纹理
	//vector<glm::vec2> texCoords;
	//索引
	//vector<int> indices;
	//计算数据
	//void init(int prec);
	//控制点
	//float* controlPoints;
	//曲线阶数
	//int step;
	//float toRadians(float degrees);
	float Bernstein(float u, int index);
public:
	BezierFace();

	BezierFace(BezierFace&& rhs) noexcept;

	//释放借来的所有资源
	~BezierFace();

	void draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view);

	////BezierFace(int step, float controlPoints[], int prec);
	//int getNumVertices();
	//int getNumIndices();
	//vector<glm::vec3> getVertices();
	//vector<glm::vec3> getNormals();
	//vector<glm::vec2> getTexCoords();
	//vector<int> getIndices();

private:

	GLuint _vao = 0;
	GLuint _vbo = 0;


	std::unique_ptr<TextureCubemap> _texture;

	std::unique_ptr<GLSLProgram> _shader;

	void generatevertices(GLfloat *vertices, GLint *indices, GLfloat *contorlPoints, int prec);

	void cleanup();
};
