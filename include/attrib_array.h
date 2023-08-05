#pragma once

#include <cstdint>
#include <vector>


struct GL_AttribElement
{
    uint32_t component_count;
    uint32_t gl_type;
    int32_t normalized;
    uint32_t data_size;
    uint32_t stride;
};


class GL_AttribArray
{
private:
    std::vector<GL_AttribElement> m_layout;
    uint32_t m_stride;

public:
    GL_AttribArray();

    ~GL_AttribArray();

    template<typename T>
    void push(uint32_t count, bool normalized);

    inline const GL_AttribElement& get(uint32_t idx) const { return m_layout[idx]; }
    inline uint32_t get_count() const { return (uint32_t)m_layout.size(); }
    inline uint32_t get_stride() const { return m_stride; }
};
