#include "texture.h"

#include <assert.h>

#include <glad\glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string_view& filepath) {
     int width, height, channels;
     stbi_set_flip_vertically_on_load(true);
     stbi_uc* data = nullptr;

     data = stbi_load(filepath.data(), &width, &height, &channels, 0);
     assert(data);

     this->width = width;
     this->height = height;

     internalFormat = 0;
     dataFormat = 0;
     if (channels == 4) {
          internalFormat = GL_RGBA8;
          dataFormat = GL_RGBA;
     } else if (channels == 3) {
          internalFormat = GL_RGB8;
          dataFormat = GL_RGB;
     }

     assert(internalFormat & dataFormat);

     glGenTextures(1, &this->id);
     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D, this->id);
     glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
     
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

     stbi_image_free(data);
}

Texture::Texture(const glm::vec4& color)
     : width(16), height(16), internalFormat(GL_RGBA8), dataFormat(GL_RGBA) {
     int iColor = (static_cast<int>(255 * color.a) << 24)
                    | (static_cast<int>(255 * color.b) << 16)
                    | (static_cast<int>(255 * color.g) << 8)
                    | static_cast<int>(255 * color.r);

     int data[256];
     for (int i = 0; i < 256; i++) data[i] = iColor;

     glGenTextures(1, &this->id);
     glBindTexture(GL_TEXTURE_2D, this->id);
     glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, 16, 16, 0, dataFormat, GL_UNSIGNED_BYTE, data);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::~Texture() {
     glDeleteTextures(1, &this->id);
}

std::shared_ptr<Texture> Texture::CreateTexture(const std::string_view& filepath) {
     return std::shared_ptr<Texture>(new Texture(filepath));
}

std::shared_ptr<Texture> Texture::CreateColoredTexture(const glm::vec4& color) {
     return std::shared_ptr<Texture>(new Texture(color));
}

void Texture::Bind(uint32_t id) const {
     glActiveTexture(GL_TEXTURE0 + id);
     glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::Unbind() const {
     glBindTexture(GL_TEXTURE_2D, 0);
}