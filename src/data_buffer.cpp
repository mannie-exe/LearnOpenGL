#include "data_buffer.h"
#include "renderer.h"
#include <cstdio>


template<typename T>
GL_DataBuffer<T>::GL_DataBuffer() :
    m_gl_id(0), m_gl_buffer_type(0), m_data_count(0), m_data_size(0), m_buffer_size(0)
{
    GL_CALL(glGenBuffers(1, &m_gl_id));
    ASSERT(m_gl_id);
}


template<typename T>
GL_DataBuffer<T>::GL_DataBuffer(uint32_t gl_buffer_type, uint32_t data_count, const T* buffer_data, uint32_t gl_buffer_usage) :
    GL_DataBuffer()
{
    set_data(gl_buffer_type, data_count, buffer_data, gl_buffer_usage);
}

template<typename T>
GL_DataBuffer<T>::~GL_DataBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_gl_id));
}


template<typename T>
void GL_DataBuffer<T>::set_data(uint32_t gl_buffer_type, uint32_t data_count, const T* buffer_data, uint32_t gl_buffer_usage)
{
    switch (gl_buffer_type)
    {
        /**
         * See: https://www.khronos.org/opengl/wiki/Buffer_Object#General_use
         * for more GL buffer types
         */
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
        break;

    default:
        fprintf(stderr, "ERROR | Invalid OpenGL buffer type: 0x%04x", gl_buffer_type);
        ASSERT(0);
        return;
    }

    /**
     * TODO: Advanced type-checking can be done here to ensure incorrect types aren't used for certain buffers
     */

    m_gl_buffer_type = gl_buffer_type;
    m_data_count = data_count;
    m_data_size = (uint32_t)sizeof(T);
    m_buffer_size = m_data_count * m_data_size;

    GL_CALL(glBindBuffer(m_gl_buffer_type, m_gl_id));
    GL_CALL(glBufferData(m_gl_buffer_type, m_buffer_size, buffer_data, gl_buffer_usage));
}


template<typename T>
inline void GL_DataBuffer<T>::bind() const
{
    GL_CALL(glBindBuffer(m_gl_buffer_type, m_gl_id));
}


template<typename T>
inline void GL_DataBuffer<T>::unbind() const
{
    GL_CALL(glBindBuffer(m_gl_buffer_type, 0));
}


template class GL_DataBuffer<signed char>;
template class GL_DataBuffer<unsigned char>;
template class GL_DataBuffer<short>;
template class GL_DataBuffer<unsigned short>;
template class GL_DataBuffer<int32_t>;
template class GL_DataBuffer<uint32_t>;
template class GL_DataBuffer<float>;
template class GL_DataBuffer<double>;
template class GL_DataBuffer<int32_t>;
