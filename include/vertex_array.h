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
    GL_VertexArray();
    GL_VertexArray(GL_AttribArray* attrib_array, GL_DataBuffer<T>* data_buffer);

    ~GL_VertexArray();

    void set_buffer(GL_AttribArray* attrib_array, GL_DataBuffer<T>* data_buffer);

    void bind() const;
    void unbind() const;

    inline uint32_t get_id() const { return m_gl_id; }
};
