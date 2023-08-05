#include "attrib_array.h"
#include "renderer.h"
#include "gl_utils.h"


GL_AttribArray::GL_AttribArray() :
    m_stride(0) {}


GL_AttribArray::~GL_AttribArray()
{
    m_layout.clear();
}


template<typename T>
void GL_AttribArray::push(uint32_t count, bool normalized)
{
    uint32_t gl_type = get_gl_type<T>();
    ASSERT(gl_type);
    uint32_t data_size = (uint32_t)sizeof(T);
    uint32_t stride = count * data_size;

    m_layout.push_back({
        count,
        gl_type,
        normalized ? GL_TRUE : GL_FALSE,
        data_size});
    m_stride += stride;
}


template void GL_AttribArray::push<signed char>(uint32_t, bool);
template void GL_AttribArray::push<unsigned char>(uint32_t, bool);
template void GL_AttribArray::push<short>(uint32_t, bool);
template void GL_AttribArray::push<unsigned short>(uint32_t, bool);
template void GL_AttribArray::push<int32_t>(uint32_t, bool);
template void GL_AttribArray::push<uint32_t>(uint32_t, bool);
template void GL_AttribArray::push<float>(uint32_t, bool);
template void GL_AttribArray::push<double>(uint32_t, bool);
template void GL_AttribArray::push<int32_t>(uint32_t, bool);
