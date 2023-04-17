#include "shader.h"

#include <fstream>
#include <vector>

Shader::Shader(const std::string_view& vertexPath, const std::string_view& fragmentPath) {
     std::string vertexSource = readFile(vertexPath);
     std::string fragmentSource = readFile(fragmentPath);

     this->id = glCreateProgram();

     uint32_t vertexID = glCreateShader(GL_VERTEX_SHADER);
     compileShader(vertexID, vertexSource.data(), vertexSource.size());
     glAttachShader(this->id, vertexID);

     uint32_t fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
     compileShader(fragmentID, fragmentSource.data(), fragmentSource.size());
     glAttachShader(this->id, fragmentID);

     glLinkProgram(this->id);

     int isLinked = 0;
     glGetProgramiv(this->id, GL_LINK_STATUS, &isLinked);
     if (!isLinked) {
          int maxLength = 0;
          glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &maxLength);

          std::vector<char> infoLog(maxLength);
          glGetProgramInfoLog(this->id, maxLength, &maxLength, &infoLog[0]);

          glDeleteProgram(this->id);
          glDeleteShader(vertexID);
          glDeleteShader(fragmentID);

          throw std::string(infoLog.data());
     }

     glDeleteShader(vertexID);
     glDeleteShader(fragmentID);
}

std::string Shader::readFile(const std::string_view& filepath) {
     std::string result;
     std::ifstream in(filepath.data(), std::ios::in | std::ios::binary);
     if (in) {
          in.seekg(0, std::ios::end);
          result.resize((uint64_t)in.tellg());
          in.seekg(0, std::ios::beg);
          in.read(&result[0], result.size());
          in.close();
     } else {
          throw "Could not open file " + std::string(filepath);
     }

     return result;
}

void Shader::compileShader(uint32_t id, const char* const source, int length) {
     glShaderSource(id, 1, &source, 0);
     glCompileShader(id);

     int status = 0;
     glGetShaderiv(id, GL_COMPILE_STATUS, &status);

     if (!status) {
          int maxLength = 0;
          glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

          std::vector<char> infoLog(maxLength);
          glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

          glDeleteShader(id);
          fprintf(stderr, "%s\n", infoLog.data());
          throw std::string(infoLog.data());
     }
}

void Shader::checkCompilerErrors(unsigned int shader, std::string type) {
     int success;
     char infoLog[1024];

     if (type != "PROGRAM") {
          glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

          if (!success) {
               glGetShaderInfoLog(shader, 1024, NULL, infoLog);
               fprintf(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n\n", type.c_str(), infoLog);
          }
     }
     else {
          glGetProgramiv(shader, GL_LINK_STATUS, &success);
          if (!success) {
               glGetProgramInfoLog(shader, 1024, NULL, infoLog);
               fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n\n", type.c_str(), infoLog);
          }
     }
}

Shader::~Shader() {
     glDeleteProgram(this->id);
}

std::shared_ptr<Shader> Shader::Create(const std::string_view& vertexPath, const std::string_view& fragmentPath) {
     return std::shared_ptr<Shader>(new Shader(vertexPath, fragmentPath));
}

std::shared_ptr<Shader> Shader::CreateFromSource(const std::string_view& vertexSource, const std::string_view& fragmentSource) {
     return std::shared_ptr<Shader>(new Shader(vertexSource, fragmentSource));
}

void Shader::setBool(const std::string& name, bool value) const {
     glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
     glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
     glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, glm::vec2& value) const{
     glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const{
     glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, glm::vec3& value) const {
     glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
     glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, glm::vec4& value) const {
     glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
     glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, glm::mat2& mat) const {
     glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, glm::mat3& mat) const {
     glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, glm::mat4& mat) const {
     glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}