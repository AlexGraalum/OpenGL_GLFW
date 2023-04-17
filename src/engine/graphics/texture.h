#pragma once

#include <map>
#include <string>
#include <memory>

#include <glm\glm.hpp>

class Texture {
private:
     uint32_t id;
     uint64_t width, height;
     uint32_t internalFormat, dataFormat;
public:
     Texture(const std::string_view& filepath);
     Texture(const glm::vec4& color);

     ~Texture();

     static std::shared_ptr<Texture> Create(const std::string_view& filepath);
     static std::shared_ptr<Texture> Create(const glm::vec4& color);

     void Bind(uint32_t slot) const;
     void Unbind() const;

     uint32_t GetID() { return this->id; }
};