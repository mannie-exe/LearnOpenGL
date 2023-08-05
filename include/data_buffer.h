#pragma once

#include <cstdint>


template<typename T>
class GL_DataBuffer
{
private:
    uint32_t m_gl_id;
    uint32_t m_gl_buffer_type;
    uint32_t m_data_count;
    uint32_t m_data_size;
    uint32_t m_buffer_size;

public:
    GL_DataBuffer(uint32_t gl_buffer_type, uint32_t data_count, const T* buffer_data, uint32_t gl_buffer_usage);
    ~GL_DataBuffer();

    inline void gl_bind() const;
    inline void gl_unbind() const;

    inline uint32_t get_id() const { return m_gl_id;  }
    inline uint32_t get_type() const { return m_gl_buffer_type; }
    inline uint32_t get_count() const { return m_data_count; }
    inline uint32_t get_data_size() const { return m_data_size; }
    inline uint32_t get_buffer_size() const { return m_buffer_size; }
};
