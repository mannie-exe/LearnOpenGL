#include <iostream>
#include "renderer.h"
#include "data_buffer.h"


template <typename T>
GL_DataBuffer<T>::GL_DataBuffer(uint32_t buffer_type, uint32_t data_count, const T* buffer_data, uint32_t buffer_usage) :
    m_buffer_type(buffer_type), m_data_count(data_count)
{
    switch (m_buffer_type) {
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
        break;

    default:
        fprintf(stderr, "ERROR | Invalid OpenGL buffer type: 0x%04x", m_buffer_type);
        exit(-1);
    }

    GL_CALL(glGenBuffers(1, &m_gl_id));
    GL_CALL(glBindBuffer(m_buffer_type, m_gl_id));
    GL_CALL(glBufferData(m_buffer_type, m_data_count * sizeof(T), &buffer_data, buffer_usage));
}


template <typename T>
GL_DataBuffer<T>::~GL_DataBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_gl_id));
}


template <typename T>
void GL_DataBuffer<T>::gl_bind() const
{
    GL_CALL(glBindBuffer(m_buffer_type, m_gl_id));
}


template <typename T>
void GL_DataBuffer<T>::gl_unbind() const
{
    GL_CALL(glBindBuffer(m_buffer_type, 0));
}


template class GL_DataBuffer<float>;
template class GL_DataBuffer<uint32_t>;
