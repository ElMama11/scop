#include "Parser.hpp"

Mesh Parser::parseOBJ(const std::string &filePath) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures; // This can be filled based on the material file (.mtl)

    std::vector<Vec3> tempPositions;
    std::vector<Vec3> tempNormals;
    std::vector<Vec2> tempTexCoords;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return Mesh(vertices, indices, textures);
    }

    std::string line;
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
        } else if (prefix == "vn") {
            Vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (prefix == "f") {
            processVertex(ss, vertices, indices, tempPositions, tempNormals, tempTexCoords);
        }
    }

    file.close();
    return Mesh(vertices, indices, textures);
}

void Parser::processVertex(std::stringstream &ss, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Vec3> &tempPositions, std::vector<Vec3> &tempNormals, std::vector<Vec2> &tempTexCoords) {
    std::string vertexData;
    while (ss >> vertexData) {
        std::stringstream vertexSS(vertexData);
        std::string indexStr;
        std::vector<unsigned int> indicesTemp;

        while (std::getline(vertexSS, indexStr, '/')) {
            indicesTemp.push_back(std::stoi(indexStr));
        }

        Vertex vertex;
        vertex.position = tempPositions[indicesTemp[0] - 1];
        if (indicesTemp.size() > 1 && !indexStr.empty()) {
            vertex.texCoords = tempTexCoords[indicesTemp[1] - 1];
        }
        if (indicesTemp.size() > 2 && !indexStr.empty()) {
            vertex.normal = tempNormals[indicesTemp[2] - 1];
        }

        vertices.push_back(vertex);
        indices.push_back(vertices.size() - 1);
    }
}
