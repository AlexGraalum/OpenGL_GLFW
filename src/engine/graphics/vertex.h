#pragma once

#include <glm\glm.hpp>

struct Vertex {
     glm::vec3 position;
     glm::vec3 normal;
     glm::vec2 texCoords;

     Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) {
          this->position = p;
          this->normal = n;
          this->texCoords = t;
     }
};