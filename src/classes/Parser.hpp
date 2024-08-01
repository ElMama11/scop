#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Mesh.hpp"
#include "Vec2.hpp"

class Parser {
public:
    static Mesh parseOBJ(const std::string &filePath);

private:
    static void processVertex(std::stringstream &ss, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Vec3> &tempPositions, std::vector<Vec3> &tempNormals, std::vector<Vec2> &tempTexCoords, bool hasTexCoords);
};

#endif