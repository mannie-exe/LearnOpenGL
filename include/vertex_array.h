#pragma once

#include <cstdint>
#include "data_buffer.h"


template <typename T>
class GL_VertexArray
{
private:
    uint32_t m_gl_id;
    uint32_t m_gl_type;
    uint32_t m_component_count;
    uint32_t m_vertex_count;
public:
    GL_VertexArray(uint32_t component_count, uint32_t vertex_count, GL_DataBuffer<T>* data_buffer);

    ~GL_VertexArray();

    void gl_bind() const;

    void gl_unbind() const;

    inline uint32_t get_id() const { return m_gl_id; }
    inline uint32_t get_type() const { return m_gl_type; }
    inline uint32_t get_component_count() const { return m_component_count; }
    inline uint32_t get_vertex_count() const { return m_vertex_count; }
};
