#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>


class GL_ShaderProgram
{
private:
    uint32_t m_gl_id;
    std::unordered_map<const char*, int32_t> m_uniform_location_cache;

private:
    uint32_t compile_shader(uint32_t gl_shader_type, const char* shader_source);
    int32_t get_uniform_location(const std::string& uniform_name);

public:
    GL_ShaderProgram();
    GL_ShaderProgram(const std::string& vert_file_path, const std::string& frag_file_path);

    ~GL_ShaderProgram();

    void create(const std::string& vert_file_path, const std::string& frag_file_path);

    void set_uniform_1f(const std::string& uniform_name, float v0);
    void set_uniform_4f(const std::string& uniform_name, float v0, float v1, float v2, float v3);

    void bind() const;
    void unbind() const;

    inline uint32_t get_id() const { return m_gl_id; }
};
