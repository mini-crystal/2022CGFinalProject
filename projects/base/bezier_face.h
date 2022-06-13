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
	////�������
	//int numVertices;
	////��������
	int numIndices;
	//����
	//vector<glm::vec3> vertices;
	//����
	//vector<glm::vec3> normals;
	//����
	//vector<glm::vec2> texCoords;
	//����
	//vector<int> indices;
	//��������
	//void init(int prec);
	//���Ƶ�
	//float* controlPoints;
	//���߽���
	//int step;
	//float toRadians(float degrees);
	float Bernstein(float u, int index);
public:
	BezierFace();

	BezierFace(BezierFace&& rhs) noexcept;

	//�ͷŽ�����������Դ
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
