#include "renderer.h"
#include <cstdint>
#include <cstdio>
#include <ctime>
#include "gl_utils.h"


void gl_clear_error()
{
    while (glGetError());
}


bool gl_log_call(const char* function, const char* source_file, uint32_t line_number)
{
    while (GLenum gl_error = glGetError())
    {
        fprintf(stderr, "ERROR | OpenGL [0x%04x] %s\n    in %s @ %s:%d)", gl_error, "<message>", function, source_file, line_number);
        return false;
    }

    return true;
}

void GL_Renderer::clear()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}


template<typename T, typename K>
void GL_Renderer::draw(const GL_VertexArray<T>* vertex_array, const GL_DataBuffer<K>* index_buffer, GL_ShaderProgram* shader_program)
{
    // Get index buffer data type
    uint32_t gl_type = get_gl_type<K>();

    // Configure vertex buffer state
    vertex_array->bind();
    index_buffer->bind();

    // Configure shader state
    shader_program->bind();
    shader_program->set_uniform_1f("u_time", clock() / (float)CLOCKS_PER_SEC);

    // Draw object
    GL_CALL(glDrawElements(GL_TRIANGLES, index_buffer->get_count(), gl_type, nullptr));

    // Clear GL state
    shader_program->unbind();
    index_buffer->unbind();
    vertex_array->unbind();
}


template void GL_Renderer::draw<float, uint32_t>(const GL_VertexArray<float>*, const GL_DataBuffer<uint32_t>*, GL_ShaderProgram*);
