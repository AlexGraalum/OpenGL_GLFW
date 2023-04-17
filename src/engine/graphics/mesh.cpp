#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
     this->vertices = vertices;
     this->indices = indices;

     PrepareMesh();
}

Mesh::Mesh(std::string filePath) {
     LoadFile(filePath);

     PrepareMesh();
}

void Mesh::LoadFile(std::string filePath) {
     fprintf(stderr, "Loading file %s into new mesh...\n", filePath.c_str());

     if (filePath.substr(filePath.length() - 4, filePath.length()) != ".obj") {
          fprintf(stderr, "File %s is not of type .obj\n", filePath.c_str());
          exit(-1);
     }

     std::ifstream inFile(filePath);

     if (!inFile) {
          fprintf(stderr, "Failure to open .obj file %s\n", filePath.c_str());
          exit(-1);
     }

     std::vector<glm::vec3> positions;
     std::vector<glm::vec3> normals;
     std::vector<glm::vec2> texture;

     for (std::string line; std::getline(inFile, line);) {
          if (line.substr(0, 2) == "v ") {
               std::istringstream iss{ line.substr(2, line.length()) };
               glm::vec3 v;

               iss >> v.x >> v.y >> v.z;
               positions.push_back(v);
          }
          else if (line.substr(0, 2) == "vn") {
               std::istringstream iss{ line.substr(3, line.length()) };
               glm::vec3 vn;

               iss >> vn.x >> vn.y >> vn.z;
               normals.push_back(vn);
          }
          else if (line.substr(0, 2) == "vt") {
               std::istringstream iss{ line.substr(3, line.length()) };
               glm::vec2 vt;

               iss >> vt.x >> vt.y;
               texture.push_back(vt);
          }
          else if (line.substr(0, 2) == "f ") {
               std::string l = line.substr(2, line.length());

               unsigned int f[3][3];
               //                                                v1.v      v1.vt     v1.n      v2.v      v2.vt     v2.n      v3.v      v3.vt     v3.n
               sscanf_s(l.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &f[0][0], &f[0][1], &f[0][2], &f[1][0], &f[1][1], &f[1][2], &f[2][0], &f[2][1], &f[2][2]);

               for (unsigned int i = 0; i < 3; i++) {
                    this->vertices.push_back(Vertex(positions[(*&f[i][0]) - 1], normals[(*&f[i][2]) - 1], texture[(*&f[i][1]) - 1]));
                    this->indices.push_back((unsigned int)this->vertices.size() - 1);
               }
          }
     }

     fprintf(stderr, ".obj File %s loaded successfully\n", filePath.c_str());
}

void Mesh::PrepareMesh() {
     vertexArray = new VertexArray();
     vertexArray->Bind();

     vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
     elementBuffer = new ElementBuffer(&indices[0], indices.size() * sizeof(unsigned int));

     //position
     vertexArray->LinkAttribute(*vertexBuffer, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
     //normal
     vertexArray->LinkAttribute(*vertexBuffer, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
     //texture
     vertexArray->LinkAttribute(*vertexBuffer, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

     vertexArray->Unbind();
     vertexBuffer->Unbind();
     elementBuffer->Unbind();

     fprintf(stderr, ".obj File has been prepared\n");
}

Mesh::~Mesh() {
     vertices.clear();
     indices.clear();

     vertexArray->Delete();
     vertexBuffer->Delete();
     elementBuffer->Delete();

     fprintf(stderr, ".obj File has been deleted");
}

std::shared_ptr<Mesh> Mesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
     return std::shared_ptr<Mesh>(new Mesh(vertices, indices));
}

std::shared_ptr<Mesh> Mesh::Create(std::string filePath) {
     return std::shared_ptr<Mesh>(new Mesh(filePath));
}

void Mesh::DrawMesh() {
     this->vertexArray->Bind();
     glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

     this->vertexArray->Unbind();
}