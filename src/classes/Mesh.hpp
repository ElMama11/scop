#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>
#include <cstddef>
#include "Vec3.hpp"
#include "Vec2.hpp"
#include "Shader.hpp"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
	public:
        // mesh data
        std::vector<Vertex>          vertices;
        std::vector<unsigned int>    indices;
        std::vector<Texture>         textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void draw(Shader &shader);

    private:
        // render data
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};

#endif