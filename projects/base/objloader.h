#pragma once
#include <string>
#include <vector>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <utility>

#include <fstream>
#include <sstream>
#include <iostream>

typedef struct {
	std::vector<float> vertices;   // 'v'
	std::vector<float> normals;    // 'vn'
	std::vector<float> texcoords;  // 'vt'
} attrib_t;
typedef struct {
	int vertex_index;
	int normal_index;
	int texcoord_index;
} index_t;

bool Objloader(attrib_t* attrib, std::vector<index_t>* indices,
	std::string* err, const char* filename);
void Objoutput(attrib_t* attrib, std::vector<index_t>* indices,
	const char* filename);
