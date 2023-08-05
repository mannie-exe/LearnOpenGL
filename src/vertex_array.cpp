#include "vertex_array.h"
#include "renderer.h"
#include <type_traits>
#include "gl_utils.h"


template<typename T>
GL_VertexArray<T>::GL_VertexArray()
{
    GL_CALL(glGenVertexArrays(1, &m_gl_id));
}


template<typename T>
GL_VertexArray<T>::GL_VertexArray(GL_AttribArray* attrib_array, GL_DataBuffer<T>* data_buffer) :
    GL_VertexArray()
{
    set_buffer(attrib_array, data_buffer);
}


template<typename T>
GL_VertexArray<T>::~GL_VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &m_gl_id));
}


template<typename T>
void GL_VertexArray<T>::set_buffer(GL_AttribArray* attrib_array, GL_DataBuffer<T>* data_buffer)
{
    GL_CALL(glBindVertexArray(m_gl_id));
    data_buffer->bind();

    uint32_t attrib_offset = 0;
    for (uint32_t idx = 0; idx < attrib_array->get_count(); idx++)
    {
        const GL_AttribElement& current_attrib = attrib_array->get(idx);

        GL_CALL(glVertexAttribPointer(
            idx,
            current_attrib.component_count,
            current_attrib.gl_type,
            current_attrib.normalized,
            attrib_array->get_stride(),
            (const void*)attrib_offset));
        GL_CALL(glEnableVertexAttribArray(idx));

        attrib_offset += current_attrib.stride;
    }
}


template<typename T>
inline void GL_VertexArray<T>::bind() const
{
    GL_CALL(glBindVertexArray(m_gl_id));
}


template<typename T>
inline void GL_VertexArray<T>::unbind() const
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
