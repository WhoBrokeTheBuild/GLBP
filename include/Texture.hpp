#pragma once

#include <depend/OpenGL.hpp>
#include <depend/Math.hpp>

#include <string>

class Texture 
{
public:

    struct Options 
    {
        Options()
            : WrapS(GL_REPEAT)
            , WrapT(GL_REPEAT)
            , MagFilter(GL_NEAREST)
            , MinFilter(GL_NEAREST)
            , Mipmap(true)
        { }

        GLenum WrapS;
        GLenum WrapT;

        GLenum MagFilter;
        GLenum MinFilter;

        bool Mipmap;
    };

    inline Texture(const std::string& filename, Options opts = Options()) {
        LoadFromFile(filename, opts);
    }

    inline Texture(const uint8_t * buffer, glm::ivec2 size, int comp = 4, Options opts = Options()) {
        LoadFromBuffer(buffer, size, comp, opts);
    }

    inline virtual ~Texture() 
    {
        if (id_ > 0) {
            glDeleteTextures(1, &id_);
        }

        id_ = 0;
    }

    bool LoadFromFile(const std::string& filename, Options opts = Options());

    bool LoadFromBuffer(const uint8_t * buffer, glm::ivec2 size, int comp = 4, Options opts = Options());

    void Bind()
    {
        glBindTexture(GL_TEXTURE_2D, id_);
    }

private:

    GLuint id_ = 0;

};