#include "vertex_array.h"
#include "renderer.h"
#include <type_traits>


template<typename T>
GL_VertexArray<T>::GL_VertexArray(uint32_t component_count, uint32_t vertex_count, GL_DataBuffer<T>* gl_data_buffer) :
    m_gl_type(0), m_component_count(component_count), m_vertex_count(vertex_count)
{
    /**
     * NOTE: not all GL types are supported (such as `FIXED` and `HALF_FLOAT`)
     */
    if (std::is_same<T, signed char>::value)
    {
        m_gl_type = GL_BYTE;
    }

    if (std::is_same<T, unsigned char>::value)
    {
        m_gl_type = GL_UNSIGNED_BYTE;
    }

    if (std::is_same<T, short>::value)
    {
        m_gl_type = GL_SHORT;
    }

    if (std::is_same<T, unsigned short>::value)
    {
        m_gl_type = GL_UNSIGNED_SHORT;
    }

    if (std::is_same<T, int32_t>::value)
    {
        m_gl_type = GL_INT;
    }

    if (std::is_same<T, uint32_t>::value)
    {
        m_gl_type = GL_UNSIGNED_INT;
    }

    if (std::is_same<T, float>::value)
    {
        m_gl_type = GL_FLOAT;
    }

    if (std::is_same<T, double>::value)
    {
        m_gl_type = GL_DOUBLE;
    }

    ASSERT(m_gl_type);

    GL_CALL(glGenVertexArrays(1, &m_gl_id));
    GL_CALL(glBindVertexArray(m_gl_id));
    gl_data_buffer->gl_bind();
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, m_component_count, m_gl_type, false, m_component_count * sizeof(T), (const void*)0));
}


template<typename T>
GL_VertexArray<T>::~GL_VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &m_gl_id));
}


template<typename T>
void GL_VertexArray<T>::gl_bind() const
{
    GL_CALL(glBindVertexArray(m_gl_id));
}


template<typename T>
void GL_VertexArray<T>::gl_unbind() const
{
    GL_CALL(glBindVertexArray(0));
}


template class GL_VertexArray<signed char>;
template class GL_VertexArray<unsigned char>;
template class GL_VertexArray<short>;
template class GL_VertexArray<unsigned short>;
template class GL_VertexArray<int32_t>;
template class GL_VertexArray<uint32_t>;
template class GL_VertexArray<float>;
template class GL_VertexArray<double>;
template class GL_VertexArray<int32_t>;
