#include "texture_2d.h"
#include <stb_image.h>


GL_Texture2D::GL_Texture2D() :
    m_gl_id(0), m_file_path(""), m_image_buffer(nullptr), m_width(0), m_height(0), m_channels(0)
{
    GL_CALL(glGenTextures(1, &m_gl_id));
    ASSERT(m_gl_id);
}


GL_Texture2D::GL_Texture2D(uint32_t gl_texture_slot, const std::string& image_file_path, bool flip_vertically, bool transparent, int32_t channels) :
    GL_Texture2D()
{
    load_image(gl_texture_slot, image_file_path, flip_vertically, transparent, channels);
}


GL_Texture2D::~GL_Texture2D()
{
    GL_CALL(glDeleteTextures(1, &m_gl_id));
}


void GL_Texture2D::load_image(uint32_t gl_texture_slot, const std::string& image_file_path, bool flip_vertically, bool transparent, int32_t channels)
{
    // Load image
    stbi_set_flip_vertically_on_load(flip_vertically);
    m_image_buffer = stbi_load(image_file_path.c_str(), &m_width, &m_height, &m_channels, channels);
    m_file_path = image_file_path;

    // Bind texture
    gl_bind(gl_texture_slot);

    // Configure texture
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

    // Load image into texture
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, transparent ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, m_image_buffer));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

    // Clear image buffer (which may be empty)
    if (m_image_buffer)
        stbi_image_free(m_image_buffer);
}


void GL_Texture2D::gl_bind(uint32_t gl_texture_slot) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + gl_texture_slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_gl_id));
}


void GL_Texture2D::gl_unbind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}
