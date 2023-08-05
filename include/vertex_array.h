#pragma once

#include <cstdint>
#include "data_buffer.h"
#include "attrib_array.h"


template<typename T>
class GL_VertexArray
{
private:
    uint32_t m_gl_id;

public:
    GL_VertexArray(GL_AttribArray* attrib_array, GL_DataBuffer<T>* data_buffer);
    ~GL_VertexArray();

    inline void gl_bind() const;
    inline void gl_unbind() const;

    inline uint32_t get_id() const { return m_gl_id; }
};
