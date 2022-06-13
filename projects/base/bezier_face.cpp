#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "bezier_face.h"

BezierFace::BezierFace() {
	GLfloat vertices[288];
	GLint indices[96];
	float controlPoints[] = {
	-1.5, -1.5, 2.0, -0.5, -1.5, 2.0, 0.5, -1.5, -1.0, 1.5, -1.5, 2.0,
	-1.5, -0.5, 1.0, -0.5, 1.5, 2.0, 0.5, 0.5, 1.0, 1.5, -0.5, -1.0,
	-1.5, 0.5, 2.0, -0.5, 0.5, 1.0, 0.5, 0.5, 3.0, 1.5, -1.5, 1.5,
	-1.5, 1.5, -2.0, -0.5, 1.5, -2.0, 0.5, 0.5, 1.0, 1.5, 1.5, -1.0
	};

	generatevertices(vertices, indices, controlPoints, 4);

	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error  << std::endl;
	}


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

//BezierFace::BezierFace() {}
//
//BezierFace::BezierFace(int step, float controlPoints[], int prec)
//{
//	this->step = step;
//	this->controlPoints = controlPoints;
//	init(prec);
//}
//
//int BezierFace::getNumVertices()
//{
//	return numVertices;
//}
//
//int BezierFace::getNumIndices()
//{
//	return numIndices;
//}
//
//vector<glm::vec3> BezierFace::getVertices()
//{
//	return vertices;
//}
//
//vector<glm::vec3> BezierFace::getNormals()
//{
//	return normals;
//}
//
//vector<glm::vec2> BezierFace::getTexCoords()
//{
//	return texCoords;
//}
//
//vector<int> BezierFace::getIndices()
//{
//	return indices;
//}


BezierFace::BezierFace(BezierFace&& rhs) noexcept
	: _vao(rhs._vao),
	_vbo(rhs._vbo),
	_texture(std::move(rhs._texture)),
	_shader(std::move(rhs._shader)) {
	rhs._vao = 0;
	rhs._vbo = 0;
}


BezierFace::~BezierFace() {
	cleanup();
}



void BezierFace::draw(const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view) {


	_shader->use();
	_shader->setMat4("model", model);
	_shader->setMat4("view", view);
	_shader->setMat4("projection", projection);
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, numIndices);



}


void BezierFace::cleanup() {
	if (_vbo != 0) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_vao != 0) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}


void BezierFace::generatevertices(GLfloat *vertices, GLint *indices, GLfloat *controlPoints, int prec) {

	float *controlPointsVector;
	int numVertices = (prec) * (prec);
	numIndices = (prec-1) * (prec-1) * 6;
	//int offset = 0;
	vector<glm::vec3> verticesVector;

	//for (int i = 0; i < 16 * 3; i += 3)
	//{
	//	//controlPointsVector.push_back(glm::vec3(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2]));
	//	memcpy(controlPointsVector + offset, glm::value_ptr(glm::vec3(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2])), 3 * sizeof(float));
	//	offset += 3;
	//}

	//offset = 0;

	for (int i = 0; i < prec; i++)
	{
		for (int j = 0; j < prec; j++)
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float u = (float)i / prec;
			float v = (float)j / prec;

			for (int q = 0; q <= 3; q++)
			{
				for (int p = 0; p <= 3; p++)
				{
					int index = (q * 4 + p) * 3;
					x += controlPoints[index] * Bernstein(u, q) * Bernstein(v, p);
					y += controlPoints[index+1] * Bernstein(u, q) * Bernstein(v, p);
					z += controlPoints[index+2] * Bernstein(u, q) * Bernstein(v, p);
				}
			}
			//memcpy(verticesVector + offset, glm::value_ptr(glm::vec3(x, y, z)), 3 * sizeof(float));
			verticesVector.push_back(glm::vec3(x, y, z));
			//memcpy(normals + offset, glm::value_ptr(glm::vec3(x, y, z)), 3 * sizeof(float));
			//normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
			//texCoords[i * (prec + 1) + j] = glm::vec2((float)j / prec, (float)i / prec);
			//offset += 3;
		}
	}

	for (int i = 0; i < prec-1; i++) {
		for (int j = 0; j < prec-1; j++) {
			indices[6 * (i * (prec - 1) + j) + 0] = i * (prec) + j;
			indices[6 * (i * (prec - 1) + j) + 1] = i * (prec) + j + 1;
			indices[6 * (i * (prec - 1) + j) + 2] = (i + 1) * (prec) + j;
			indices[6 * (i * (prec - 1) + j) + 3] = i * (prec) + j + 1;
			indices[6 * (i * (prec - 1) + j) + 4] = (i + 1) * (prec) + j + 1;
			indices[6 * (i * (prec - 1) + j) + 5] = (i + 1) * (prec) + j;
		}
	}

	for (int i = 0; i < numIndices; i++)
	{
		memcpy(vertices + i * 3, glm::value_ptr(verticesVector[indices[i]]), 3 * sizeof(float));
	}

}

//
//void BezierFace::init(int prec)
//{
//	numVertices = (prec + 1) * (prec + 1);
//	numIndices = prec * prec * 6;
//	for (int i = 0; i < numVertices; i++)
//	{
//		vertices.push_back(glm::vec3());
//		normals.push_back(glm::vec3());
//		//texCoords.push_back(glm::vec2());
//	}
//	for (int i = 0; i < numIndices; i++)
//	{
//		indices.push_back(0);
//	}
//	for (int i = 0; i < (step + 1) * (step + 1) * 3; i += 3)
//	{
//		controlPointsVector.push_back(glm::vec3(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2]));
//	}
//	for (int i = 0; i <= prec; i++)
//	{
//		for (int j = 0; j <= prec; j++)
//		{
//			float x = 0.0f;
//			float y = 0.0f;
//			float z = 0.0f;
//			float u = (float)i / prec;
//			float v = (float)j / prec;
//
//			for (int i = 0; i <= 3; i++)
//			{
//				for (int j = 0; j <= 3; j++)
//				{
//					int index = i * 4 + j;
//					x += controlPointsVector[index].x * Bernstein(u, i) * Bernstein(v, j);
//					y += controlPointsVector[index].y * Bernstein(u, i) * Bernstein(v, j);
//					z += controlPointsVector[index].z * Bernstein(u, i) * Bernstein(v, j);
//				}
//			}
//			vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
//			normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
//			//texCoords[i * (prec + 1) + j] = glm::vec2((float)j / prec, (float)i / prec);
//		}
//	}
//	//计算索引
//	for (int i = 0; i < prec; i++) {
//		for (int j = 0; j < prec; j++) {
//			indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
//			indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
//			indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
//			indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
//			indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
//			indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
//		}
//	}
//}
//float BezierFace::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

//根据index计算每个点（u，v）的
float BezierFace::Bernstein(float t, int index)
{
	switch (index)
	{
	default:
	case 0:
		return pow(1.0 - t, 3);
		break;
	case 1:
		return 3 * t * pow(1.0 - t, 2);
		break;
	case 2:
		return 3 * pow(t, 2) * (1 - t);
		break;
	case 3:
		return pow(t, 3);
		break;
	}

}