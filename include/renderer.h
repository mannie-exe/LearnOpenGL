#pragma once

#define GLEW_STATIC

#include <cstdint>
#include <GL/glew.h>
#include "vertex_array.h"
#include "data_buffer.h"
#include "shader_program.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define GL_CALL(x) gl_clear_error();\
    x;\
    ASSERT(gl_log_call(#x, __FILE__, __LINE__))


void gl_clear_error();


bool gl_log_call(const char* function, const char* source_file, uint32_t line_number);


class GL_Renderer
{
public:
    void clear();

    template<typename T, typename K>
    void draw(const GL_VertexArray<T>* vertex_array, const GL_DataBuffer<K>* index_buffer, GL_ShaderProgram* shader_program);
};
