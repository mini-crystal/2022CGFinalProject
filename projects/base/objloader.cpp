#include "../base/objloader.h"

bool Objloader(attrib_t* attrib, std::vector<index_t>* indices,
	std::string* err, const char* filename)
{


	float x = 0.0, y = 0.0, z = 0.0;




	std::ifstream fileStream(filename);
	std::string line = "";

	while (!fileStream.eof()) {

		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0) { // vertex position ("v" case)

			std::stringstream ss(line.erase(0, 1));
			ss >> x; ss >> y; ss >> z; // extract the vertex position values

			attrib->vertices.push_back(x);
			attrib->vertices.push_back(y);
			attrib->vertices.push_back(z);


		}
		if (line.compare(0, 2, "vt") == 0) { // texture coordinates ("vt" case)
			std::stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; // extract texture coordinate values

			attrib->texcoords.push_back(x);
			attrib->texcoords.push_back(y);

		}
		if (line.compare(0, 2, "vn") == 0) { // vertex normals ("vn" case)
			std::stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z; // extract the normal vector values
			attrib->normals.push_back(x);
			attrib->normals.push_back(y);
			attrib->normals.push_back(z);

		}


		if (line.compare(0, 2, "f ") == 0) {
			// 注意，这里的 f 后面必须加一个空格，因为line.compare(0, 2, "f ")中表示从索引0开始的2个字符，而f不加空格只有一个字符

			std::string oneCorner, v, t, n;
			std::stringstream ss(line.erase(0, 2));

			index_t tempindex;

			for (int i = 0; i < 3; i++) {
				getline(ss, oneCorner, ' '); // extract triangle face references
				std::stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');
				tempindex.vertex_index = stoi(v); // "stoi" converts string to integer
				tempindex.texcoord_index = stoi(t);
				tempindex.normal_index = stoi(n);
				indices->push_back(tempindex);
			}

		}
	}
	return true;
}
void Objoutput(attrib_t* attrib, std::vector<index_t>* indices,
	const char* filename) {
	std::ofstream fout(filename);
	std::int16_t ver = 0;
	std::int16_t nor = 0;
	std::int16_t tex = 0;
	std::int16_t ind = 0;
	while (true) {
		//if()
		//in << "0" << i << "\t" << a << "\n";

		if (ver < attrib->vertices.size()) {

			fout << "v " << std::to_string(attrib->vertices[ver]) << " " << std::to_string(attrib->vertices[ver + 1]) << " " << std::to_string(attrib->vertices[ver + 2]) << "\n";
			ver = ver + 3;
			continue;
		}
		if (nor < attrib->normals.size())
		{
			fout << "vn " << std::to_string(attrib->normals[nor]) << " " << std::to_string(attrib->normals[nor + 1]) << " " << std::to_string(attrib->normals[nor + 2]) << "\n";
			nor = nor + 3;
			continue;
		}
		if (tex < attrib->texcoords.size())
		{
			fout << "vt " << std::to_string(attrib->texcoords[tex]) << " " << std::to_string(attrib->texcoords[tex + 1]) << "\n";
			tex = tex + 2;
			continue;
		}


		if (ind < indices->size())
		{

			fout << "f " << std::to_string(indices->at(ind).vertex_index) << '/' << std::to_string(indices->at(ind).texcoord_index) << '/' << std::to_string(indices->at(ind).normal_index) << ' ';
			fout << std::to_string(indices->at(ind + 1).vertex_index) << '/' << std::to_string(indices->at(ind + 1).texcoord_index) << '/' << std::to_string(indices->at(ind + 1).normal_index) << ' ';
			fout << std::to_string(indices->at(ind + 2).vertex_index) << '/' << std::to_string(indices->at(ind + 2).texcoord_index) << '/' << std::to_string(indices->at(ind + 2).normal_index) << "\n";
			ind = ind + 3;
			continue;
		}
		else {
			break;
		}


	}
	fout.close();
}