#pragma once

#include <cstdint>
#include <string>
#include "renderer.h"


class GL_Texture2D
{
private:
    uint32_t m_gl_id;
    std::string m_file_path;

    unsigned char* m_image_buffer;
    int32_t m_width, m_height, m_channels;

public:
    GL_Texture2D();
    GL_Texture2D(const std::string& image_file_path, bool flip_vertically = false, int32_t channels = 4);

    ~GL_Texture2D();

    void load_image(const std::string& image_file_path, bool flip_vertically = false, int32_t channels = 4);
    void load_image(uint32_t gl_texture_slot, const std::string& image_file_path, bool flip_vertically = false, int32_t channels = 4);

    void gl_bind(uint32_t gl_texture_slot = 0) const;
    void gl_unbind() const;

    inline int32_t get_width() const { return m_width; }
    inline int32_t get_height() const { return m_height; }
    inline int32_t get_channels() const { return m_channels; }
};
