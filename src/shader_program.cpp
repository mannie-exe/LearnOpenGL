#include "shader_program.h"
#include "renderer.h"
#include "file_utils.h"
#include <iostream>


GL_ShaderProgram::GL_ShaderProgram()
{
    GL_CALL(m_gl_id = glCreateProgram());
    ASSERT(m_gl_id);
}


GL_ShaderProgram::GL_ShaderProgram(const std::string& vert_file_path, const std::string& frag_file_path) :
    GL_ShaderProgram()
{
    create(vert_file_path, frag_file_path);
}


GL_ShaderProgram::~GL_ShaderProgram()
{
    GL_CALL(glDeleteProgram(m_gl_id));
}


uint32_t GL_ShaderProgram::compile_shader(uint32_t gl_shader_type, const char* shader_source)
{
    GL_CALL(uint32_t shader_id = glCreateShader(gl_shader_type));
    GL_CALL(glShaderSource(shader_id, 1, &shader_source, nullptr));
    GL_CALL(glCompileShader(shader_id));

    int32_t compile_result;
    GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result));
    if (compile_result == GL_FALSE)
    {
        int log_length;
        GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length));
        char* log_message = (char*)alloca(log_length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(shader_id, log_length, &log_length, log_message));
        GL_CALL(glDeleteShader(shader_id));

        fprintf(stderr, "ERROR | Failed to compile shader\n%s", log_message);
        return 0;
    }

    return shader_id;
}


int32_t GL_ShaderProgram::get_uniform_location(const std::string& uniform_name)
{
    const char* name_raw = uniform_name.c_str();

    if (m_uniform_location_cache.find(name_raw) == m_uniform_location_cache.end())
    {
        GL_CALL(int32_t uniform_location = glGetUniformLocation(m_gl_id, name_raw));
        if (uniform_location == -1)
        {
            fprintf(stdout, "WARN | Shader uniform location not found [uniform: %s, shader_program_id: %d]", name_raw, m_gl_id);
        }
        m_uniform_location_cache[name_raw] = uniform_location;
    }

    return m_uniform_location_cache[name_raw];
}


void GL_ShaderProgram::create(const std::string& vert_file_path, const std::string& frag_file_path)
{
    uint32_t vert_shader_id = compile_shader(GL_VERTEX_SHADER, read_file(vert_file_path).c_str());
    ASSERT(vert_shader_id);
    uint32_t frag_shader_id = compile_shader(GL_FRAGMENT_SHADER, read_file(frag_file_path).c_str());
    ASSERT(frag_shader_id);

    GL_CALL(glAttachShader(m_gl_id, vert_shader_id));
    GL_CALL(glAttachShader(m_gl_id, frag_shader_id));

    GL_CALL(glLinkProgram(m_gl_id));
    GL_CALL(glValidateProgram(m_gl_id));

    GL_CALL(glDeleteShader(vert_shader_id));
    GL_CALL(glDeleteShader(frag_shader_id));
}


void GL_ShaderProgram::set_uniform_1f(const std::string& uniform_name, float v0)
{
    GL_CALL(glUniform1f(get_uniform_location(uniform_name), v0));
}


void GL_ShaderProgram::set_uniform_4f(const std::string& uniform_name, float v0, float v1, float v2, float v3)
{
    GL_CALL(glUniform4f(get_uniform_location(uniform_name), v0, v1, v2, v3));
}


void GL_ShaderProgram::bind() const
{
    GL_CALL(glUseProgram(m_gl_id));
}


void GL_ShaderProgram::unbind() const
{
    GL_CALL(glUseProgram(0));
}
