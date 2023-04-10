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

     Texture(const std::string_view& filepath);
     Texture(const glm::vec4& color);
public:
     ~Texture();

     static std::shared_ptr<Texture> CreateTexture(const std::string_view& filepath);
     static std::shared_ptr<Texture> CreateColoredTexture(const glm::vec4& color);

     void Bind(uint32_t slot) const;
     void Unbind() const;

     uint32_t GetID() { return this->id; }
};