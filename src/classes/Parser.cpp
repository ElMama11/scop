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
		}
		else if (prefix == "vt") {
			Vec2 texCoord;
			ss >> texCoord.x >> texCoord.y;
			tempTexCoords.push_back(texCoord);
			hasTexCoords = true;
		}
		else if (prefix == "vn") {
			Vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			tempNormals.push_back(normal);
		} 
		else if (prefix == "f")
			processVertex(ss, vertices, indices, tempPositions, tempNormals, tempTexCoords, hasTexCoords);
	}

	file.close();
	return Mesh(vertices, indices, textures);
}

void Parser::processVertex(std::stringstream &ss, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, const std::vector<Vec3> &tempPositions, const std::vector<Vec3> &tempNormals, const std::vector<Vec2> &tempTexCoords, bool hasTexCoords) {
    std::string vertexData;
    std::vector<unsigned int> vertexIndices;

    while (ss >> vertexData) {
        std::stringstream vertexSS(vertexData);
        std::string indexStr;
        unsigned int positionIndex = 0, texCoordIndex = 0, normalIndex = 0;

        int i = 0;
        while (std::getline(vertexSS, indexStr, '/')) {
            if (!indexStr.empty()) {
                unsigned int index = std::stoi(indexStr);
                if (index < 0) {
                    index = tempPositions.size() + index + 1;
                }
                if (i == 0) {
                    positionIndex = index;
                } else if (i == 1) {
                    texCoordIndex = index;
                } else if (i == 2) {
                    normalIndex = index;
                }
            }
            i++;
        }

        if (positionIndex > 0 && positionIndex <= tempPositions.size()) {
            Vertex vertex;
            vertex.position = tempPositions[positionIndex - 1];
            if (hasTexCoords && texCoordIndex > 0 && texCoordIndex <= tempTexCoords.size()) {
                vertex.texCoords = tempTexCoords[texCoordIndex - 1];
            } else {
                vertex.texCoords.x = (vertex.position.x + 1.0f) / 2.0f;
                vertex.texCoords.y = (vertex.position.y + 1.0f) / 2.0f;
            }
            if (normalIndex > 0 && normalIndex <= tempNormals.size()) {
                vertex.normal = tempNormals[normalIndex - 1];
            }

            // Add vertex to list and get index
            vertices.push_back(vertex);
            vertexIndices.push_back(vertices.size() - 1);
        } else {
            std::cerr << "Error: Invalid vertex index in face definition." << std::endl;
        }
    }

    // Triangulate the face if it has four vertices
    if (vertexIndices.size() == 4) {
        // Split the quad into two triangles: (0, 1, 2) and (0, 2, 3)
        indices.push_back(vertexIndices[0]);
        indices.push_back(vertexIndices[1]);
        indices.push_back(vertexIndices[2]);

        indices.push_back(vertexIndices[0]);
        indices.push_back(vertexIndices[2]);
        indices.push_back(vertexIndices[3]);
    } else if (vertexIndices.size() == 3) {
        // If it's already a triangle, just add it
        indices.push_back(vertexIndices[0]);
        indices.push_back(vertexIndices[1]);
        indices.push_back(vertexIndices[2]);
    } else {
        std::cerr << "Error: Unsupported face with " << vertexIndices.size() << " vertices." << std::endl;
    }
}
