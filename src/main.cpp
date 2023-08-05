#include <iostream>
#include <ctime>
#include "renderer.h"
#include <GLFW/glfw3.h>
#include "data_buffer.h"


static GLuint compile_gl_shader(GLenum shader_type, const std::string& source)
{
    const char* source_string = source.c_str();

    GL_CALL(uint32_t shader_id = glCreateShader(shader_type));
    GL_CALL(glShaderSource(shader_id, 1, &source_string, nullptr));
    GL_CALL(glCompileShader(shader_id));

    int32_t compile_result;
    GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result));
    if (compile_result == GL_FALSE)
    {
        int log_length;
        GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length));
        char* log_message = (char *)alloca(log_length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(shader_id, log_length, &log_length, log_message));

        std::cout << "Failed to compile shader" << std::endl;
        std::cout << log_message << std::endl;

        GL_CALL(glDeleteShader(shader_id));

        return 0;
    }

    return shader_id;
}


static uint32_t create_gl_shader(const std::string& vertex_source, const std::string& fragment_source)
{
    GL_CALL(GLuint program_id = glCreateProgram()); 

    std::cout << "INFO | Creating shader program [id: " << program_id << "]:" << std::endl;
    std::cout << "    -> VERTEX" << vertex_source << std::endl;
    std::cout << "    -> FRAGMENT" << fragment_source << std::endl;

    uint32_t vertex_id = compile_gl_shader(GL_VERTEX_SHADER, vertex_source);
    if (vertex_id == 0)
    {
        return 0;
    }
    uint32_t fragment_id = compile_gl_shader(GL_FRAGMENT_SHADER, fragment_source);
    if (fragment_id == 0)
    {
        return 0;
    }

    GL_CALL(glAttachShader(program_id, vertex_id));
    GL_CALL(glAttachShader(program_id, fragment_id));

    GL_CALL(glLinkProgram(program_id));
    GL_CALL(glValidateProgram(program_id));

    // TODO: Handle shader link and validation errors

    GL_CALL(glDeleteShader(vertex_id));
    GL_CALL(glDeleteShader(fragment_id));

    std::cout << "INFO | Shader program created successfully [id: " << program_id << "]" << std::endl;

    return program_id;
}


int main(void)
{
    /**
     * Initialize program context (Window and OpenGL)
     */
    GLFWwindow* window;
    {
        // Initialize GLFW core
        if (!glfwInit())
        {
            std::cout << "ERROR | GLFW > Core failed to initialize" << std::endl;
            return -1;
        }
        std::cout << "INFO | GLFW > Core initialized" << std::endl;

        // Create window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if (!(window = glfwCreateWindow(800, 800, "Hello World", nullptr, nullptr)))
        {
            glfwTerminate();
            std::cout << "ERROR | GLFW > Window failed to initialize" << std::endl;
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        std::cout << "INFO | GLFW > Window initialized" << std::endl;

        // Initialize OpenGL
        if (glewInit())
        {
            std::cout << "ERROR | GLEW failed to initialize" << std::endl;
            return -1;
        }
        fprintf(stdout, "INFO | GLEW > OpenGL initialized: v%s\n", glGetString(GL_VERSION));
    }

    /**
     * Enter OpenGL rendering context
     */
    {
        /**
         * GL Buffers
         */
        GLuint vertex_array;
        GL_DataBuffer<float>* vertex_buffer;
        GL_DataBuffer<uint32_t>* index_buffer;
        // "component" refers to one part of a 2D vector (i.e. x or y)
        const uint32_t v_component_size = sizeof(float);
        const uint32_t v_component_count = 2;
        const uint32_t v_count = 4;
        const uint32_t v_buffer_count = v_count * v_component_count;
        const uint32_t v_buffer_stride = v_component_count * v_component_size;
        const uint32_t e_component_count = 3;
        const uint32_t e_count = 2;
        const uint32_t e_buffer_count = e_count * e_component_count;
        {
            // VAO; configure vertex array buffer
            GL_CALL(glGenVertexArrays(1, &vertex_array));
            GL_CALL(glBindVertexArray(vertex_array));

            // VBO; configure vertex position buffer
            const float vertex_data[v_buffer_count] = {
                -0.5f,   0.5f,
                 0.5f,   0.5f,
                -0.5f,  -0.5f,
                 0.5f,  -0.5f,
            };
            vertex_buffer = new GL_DataBuffer<float>(GL_ARRAY_BUFFER, v_buffer_count, vertex_data, GL_STATIC_DRAW);

            // VAA; configure VBO layout (shape and position; acts on last-bound VAO and VBO)
            GL_CALL(glEnableVertexAttribArray(0));
            GL_CALL(glVertexAttribPointer(0, v_component_count, GL_FLOAT, false, v_buffer_stride, (const void*)0));

            // EBO; configure index/element buffer
            const uint32_t element_data[e_buffer_count] = {
                0, 1, 2,
                2, 1, 3,
            };
            index_buffer = new GL_DataBuffer<uint32_t>(GL_ELEMENT_ARRAY_BUFFER, e_buffer_count, element_data, GL_STATIC_DRAW);

            // Clear GL buffer state
            //fprintf(stderr, "%d, %d, %d\n", vertex_array, vertex_buffer.get_id(), index_buffer.get_id());
            //int32_t _vao, _vbo, _ebo;
            //glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &_vao);
            //glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &_vbo);
            //glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &_ebo);
            //fprintf(stderr, "%d, %d, %d\n", _vao, _vbo, _ebo);
            GL_CALL(glBindVertexArray(0));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
            //glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &_vao);
            //glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &_vbo);
            //glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &_ebo);
            //fprintf(stderr, "%d, %d, %d\n", _vao, _vbo, _ebo);
        }

        /**
         * Shaders
         */
        GLuint shader_program;
        uint32_t u_time_location;
        uint32_t u_color_location;
        {
            // Configure shader program
            shader_program = create_gl_shader(
                // Vertex shader
                R"glsl(
            #version 460 core

            layout(location = 0) in vec4 position;
            
            void main()
            {
                gl_Position = position;
            })glsl",
                // Fragment shader
                R"glsl(
            #version 460 core

            uniform float u_time;
            uniform vec4 u_color;

            out vec4 color;

            void main()
            {
                color = vec4(u_color.rgb * ((sin(u_time) + 1.0) * 0.5), 1.0);
            })glsl");
            if (shader_program == 0)
            {
                exit(-1);
            }
            GL_CALL(glUseProgram(shader_program));

            // Uniforms
            GL_CALL(u_time_location = glGetUniformLocation(shader_program, "u_time"));
            ASSERT(u_time_location != -1);
            GL_CALL(u_color_location = glGetUniformLocation(shader_program, "u_color"));
            ASSERT(u_color_location != -1);
            GL_CALL(glUniform4f(u_color_location, 0.03f, 0.67f, 0.92f, 1.0f));

            // Clear shader state
            GL_CALL(glUseProgram(0));
        }

        /**
         * Render Loop
         */
        while (!glfwWindowShouldClose(window))
        {
            // Configure GL buffer state
            GL_CALL(glBindVertexArray(vertex_array));
            GL_CALL(glEnableVertexAttribArray(0));
            index_buffer->gl_bind();

            // Configure GL shader state
            GL_CALL(glUseProgram(shader_program));
            GL_CALL(glUniform1f(u_time_location, clock() / (float)CLOCKS_PER_SEC));

            // Clear frame buffer
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
            // Draw buffers
            GL_CALL(glDrawElements(GL_TRIANGLES, e_buffer_count, GL_UNSIGNED_INT, nullptr));
            // Swap frame buffers
            GL_CALL(glfwSwapBuffers(window));
            // Run GLFW event loop
            GL_CALL(glfwPollEvents());

            // Clear GL state
            GL_CALL(glUseProgram(0));
            index_buffer->gl_unbind();
            vertex_buffer->gl_unbind();
            GL_CALL(glBindVertexArray(0));
        }

        /**
         * Clean-up before exiting rendering context
         */
        GL_CALL(glDeleteProgram(shader_program));
        GL_CALL(glDeleteVertexArrays(1, &vertex_array));
    }

    /**
     * Clean-up before exiting
     */
    glfwTerminate();
}
