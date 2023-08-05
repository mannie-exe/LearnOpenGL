#include "gl_utils.h"
#include <type_traits>
#include "renderer.h"


/**
 * NOTE: not all GL types are supported (such as `FIXED` and `HALF_FLOAT`)
 */

template<typename T>
uint32_t get_gl_type()
{
    if (std::is_same<T, signed char>::value)
    {
        return GL_BYTE;
    }

    if (std::is_same<T, unsigned char>::value)
    {
        return GL_UNSIGNED_BYTE;
    }

    if (std::is_same<T, short>::value)
    {
        return GL_SHORT;
    }

    if (std::is_same<T, unsigned short>::value)
    {
        return GL_UNSIGNED_SHORT;
    }

    if (std::is_same<T, int32_t>::value)
    {
        return GL_INT;
    }

    if (std::is_same<T, uint32_t>::value)
    {
        return GL_UNSIGNED_INT;
    }

    if (std::is_same<T, float>::value)
    {
        return GL_FLOAT;
    }

    if (std::is_same<T, double>::value)
    {
        return GL_DOUBLE;
    }

    return 0;
}


size_t get_gl_type_size(uint32_t gl_type)
{
    if (gl_type == GL_BYTE ||
        gl_type == GL_UNSIGNED_BYTE)
    {
        return sizeof(char);
    }

    if (gl_type == GL_SHORT ||
        gl_type == GL_UNSIGNED_SHORT)
    {
        return sizeof(short);
    }

    if (gl_type == GL_INT ||
        gl_type == GL_UNSIGNED_INT)
    {
        return sizeof(int32_t);
    }

    if (gl_type == GL_FLOAT)
    {
        return sizeof(float);
    }

    if (gl_type == GL_DOUBLE)
    {
        return sizeof(double);
    }

    return 0;
}


template uint32_t get_gl_type<signed char>();
template uint32_t get_gl_type<unsigned char>();
template uint32_t get_gl_type<short>();
template uint32_t get_gl_type<unsigned short>();
template uint32_t get_gl_type<int32_t>();
template uint32_t get_gl_type<uint32_t>();
template uint32_t get_gl_type<float>();
template uint32_t get_gl_type<double>();
template uint32_t get_gl_type<int32_t>();
