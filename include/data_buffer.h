#pragma once

#include <cstdint>


template <typename T>
class GL_DataBuffer
{
private:
    uint32_t m_gl_id;
    uint32_t m_buffer_type;
    uint32_t m_data_count;

public:
    GL_DataBuffer(uint32_t buffer_type, uint32_t data_count, const T* buffer_data, uint32_t buffer_usage);

    ~GL_DataBuffer();

    void gl_bind() const;

    void gl_unbind() const;

    inline uint32_t get_id() { return m_gl_id;  }
    inline uint32_t get_type() { return m_buffer_type; }
    inline uint32_t get_count() { return m_data_count; }
};
