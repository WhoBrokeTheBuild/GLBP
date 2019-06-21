#include <Texture.hpp>

#include <Log.hpp>
#include <stb/stb_image.h>

bool Texture::LoadFromFile(const std::string& filename, Options opts /*= Options()*/)
{
    int comp;
    glm::ivec2 size;

    uint8_t * buffer = stbi_load(filename.c_str(), &size.x, &size.y, &comp, STBI_rgb_alpha);
    if (!buffer) {
        LogError("Failed to load texture '%s'", filename);
        return false;
    }

    if (!LoadFromBuffer(buffer, size, comp, opts)) {
        LogError("Failed to load texture '%s'", filename);
        return false;
    }

    LogLoad("Loaded Texture from '%s", filename);

    stbi_image_free(buffer);

    return true;
}

bool Texture::LoadFromBuffer(const uint8_t * buffer, glm::ivec2 size, int comp /*= 4*/, Options opts /*= Options()*/)
{
    if (id_) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }

    glGenTextures(1, &id_);

    GLenum format = GL_RGBA;

    switch (comp) 
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, id_);

    LogVerbose("Binding texture to ID %u", id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.WrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opts.MagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opts.MinFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, buffer);

    if (opts.Mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}