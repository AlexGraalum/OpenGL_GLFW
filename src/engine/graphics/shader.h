#pragma once

#include <stdint.h>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
private:
     uint32_t id;

     static std::string readFile(const std::string_view& filepath);

     void compileShader(uint32_t id, const char* const source, size_t length);
     void checkCompilerErrors(unsigned int shader, std::string type);
public:
     Shader(const std::string_view& vertexPath, const std::string_view& fragmentPath);

     ~Shader();

     static std::shared_ptr<Shader> Create(const std::string_view& vertexPath, const std::string_view& fragmentPath);
     static std::shared_ptr<Shader> CreateFromSource(const std::string_view& vertexSource, const std::string_view& fragmentSource);

     void use() { glUseProgram(this->id); }

     void setBool(const std::string& name, bool value) const;
     void setInt(const std::string& name, int value) const;
     void setFloat(const std::string& name, float value) const;
     void setVec2(const std::string& name, glm::vec2& value) const;
     void setVec2(const std::string& name, float x, float y) const;
     void setVec3(const std::string& name, glm::vec3& value) const;
     void setVec3(const std::string& name, float x, float y, float z) const;
     void setVec4(const std::string& name, glm::vec4& value) const;
     void setVec4(const std::string& name, float x, float y, float z, float w) const;
     void setMat2(const std::string& name, glm::mat2& mat) const;
     void setMat3(const std::string& name, glm::mat3& mat) const;
     void setMat4(const std::string& name, glm::mat4& mat) const;
};