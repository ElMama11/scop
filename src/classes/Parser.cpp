#include "Parser.hpp"

Mesh Parser::parseOBJ(const std::string &filePath) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	std::vector<Vec3> tempPositions;
	std::vector<Vec3> tempNormals;
	std::vector<Vec2> tempTexCoords;

	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return Mesh(vertices, indices, textures);
	}

	std::string line;
	bool hasTexCoords = false;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string prefix;
		ss >> prefix;

		if (prefix == "v") {
			Vec3 position;
			ss >> position.x >> position.y >> position.z;
			tempPositions.push_back(position);
		} else if (prefix == "vt") {
			Vec2 texCoord;
			ss >> texCoord.x >> texCoord.y;
			tempTexCoords.push_back(texCoord);
			hasTexCoords = true;
		} else if (prefix == "vn") {
			Vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			tempNormals.push_back(normal);
		} else if (prefix == "f") {
			processVertex(ss, vertices, indices, tempPositions, tempNormals, tempTexCoords, hasTexCoords);
		}
	}

	file.close();
	return Mesh(vertices, indices, textures);
}

void Parser::processVertex(std::stringstream &ss, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Vec3> &tempPositions, std::vector<Vec3> &tempNormals, std::vector<Vec2> &tempTexCoords, bool hasTexCoords) {
	std::string vertexData;
	while (ss >> vertexData) {
		std::stringstream vertexSS(vertexData);
		std::string indexStr;
		std::vector<unsigned int> positionIndex, texCoordIndex, normalIndex;

		int i = 0;
		while (std::getline(vertexSS, indexStr, '/')) {
			if (!indexStr.empty()) {
				unsigned int index = std::stoi(indexStr);
				if (index < 0) {
					index = tempPositions.size() + index + 1;
				}
				if (i == 0) {
					positionIndex.push_back(index);
				} else if (i == 1) {
					texCoordIndex.push_back(index);
				} else if (i == 2) {
					normalIndex.push_back(index);
				}
			}
			i++;
		}

		Vertex vertex;
		vertex.position = tempPositions[positionIndex[0] - 1];
		if (hasTexCoords && !texCoordIndex.empty()) {
			vertex.texCoords = tempTexCoords[texCoordIndex[0] - 1];
		} else {
			// Generate planar texture coordinates
			vertex.texCoords.x = (vertex.position.x + 1.0f) / 2.0f;
			vertex.texCoords.y = (vertex.position.y + 1.0f) / 2.0f;
		}
		if (!normalIndex.empty()) {
			vertex.normal = tempNormals[normalIndex[0] - 1];
		}

		vertices.push_back(vertex);
		indices.push_back(vertices.size() - 1);
	}
}