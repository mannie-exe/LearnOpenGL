#pragma once

#include <cstdint>
#include <string>


class GL_ShaderProgram
{
private:
    uint32_t m_gl_id;
    std::string m_vert_file_path;
    std::string m_frag_file_path;

private:
    uint32_t compile_shader(uint32_t gl_shader_type, const char* shader_source);

public:
    GL_ShaderProgram();
    GL_ShaderProgram(const std::string& vert_file_path, const std::string& frag_file_path);

    ~GL_ShaderProgram();

    void create(const std::string& vert_file_path, const std::string& frag_file_path);

    void bind() const;
    void unbind() const;

    inline uint32_t get_id() const { return m_gl_id; }
};
