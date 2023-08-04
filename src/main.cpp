#include <iostream>
#include <ctime>
#include "renderer.h"
#include <GLFW/glfw3.h>


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


static GLuint create_gl_shader(const std::string& vertex_source, const std::string& fragment_source)
{
    GL_CALL(GLuint program_id = glCreateProgram()); 

    std::cout << "INFO | Creating shader program [id: " << program_id << "]:" << std::endl;
    std::cout << "    -> VERTEX" << vertex_source << std::endl;
    std::cout << "    -> FRAGMENT" << fragment_source << std::endl;

    GLuint vertex_id = compile_gl_shader(GL_VERTEX_SHADER, vertex_source);
    if (vertex_id == 0)
    {
        // TODO: Handle shader errors
        return 0;
    }
    GLuint fragment_id = compile_gl_shader(GL_FRAGMENT_SHADER, fragment_source);
    if (fragment_id == 0)
    {
        // TODO: Handle shader errors
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
     * Initialize window
     */
    GLFWwindow* window;

    // Initialize GLFW core
    if (!glfwInit())
    {
        std::cout << "ERROR | GLFW > Core failed to initialize" << std::endl;
        return -1;
    }
    std::cout << "INFO | GLFW > Core initialized" << std::endl;

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    std::cout << "INFO | GLEW initialized; using OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /**
     * Configure rendering context
     */
    GLuint vertex_buffer;
    GLuint mesh_buffer[2];
    GLuint shader_program;

    // "component" refers to one part of a 2D vector (i.e. x or y)
    const uint32_t v_component_size_bytes = sizeof(GLfloat);
    const uint32_t v_component_count = 2;
    const uint32_t vertex_count = 4;

    const uint32_t v_buffer_length = vertex_count * v_component_count;
    const uint32_t v_buffer_size = v_buffer_length * v_component_size_bytes;
    const uint32_t v_buffer_stride = v_component_count * v_component_size_bytes;

    const uint32_t e_component_size_bytes = sizeof(GLuint);
    const uint32_t e_component_count = 3;
    const uint32_t element_count = 2;

    const uint32_t e_buffer_length = element_count * e_component_count;
    const uint32_t e_buffer_size = e_buffer_length * e_component_size_bytes;
    const uint32_t e_buffer_stride = e_component_count * e_component_size_bytes;

    GL_CALL(glGenBuffers(2, mesh_buffer));

    // VAO
    //
    // Configure vertex array buffer
    GL_CALL(glGenVertexArrays(1, &vertex_buffer));
    GL_CALL(glBindVertexArray(vertex_buffer));

    // VBO
    // 
    // Configure vertex position buffer
    const GLfloat vertex_data[v_buffer_length] = {
        -0.5f,   0.5f,
         0.5f,   0.5f,
        -0.5f,  -0.5f,
         0.5f,  -0.5f,
    };

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mesh_buffer[0]));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, v_buffer_size, &vertex_data, GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, v_component_count, GL_FLOAT, false, v_buffer_stride, (const void*)0));

    // EBO
    // 
    // Configure index/element buffer
    const GLuint element_data[e_buffer_length] = {
        0, 1, 2,
        2, 1, 3,
    };

    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_buffer[1]));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_buffer_size, &element_data, GL_STATIC_DRAW));

    // Configure shader program
    shader_program = create_gl_shader(
        R"glsl(
            #version 430 core

            layout(location = 0) in vec4 position;
            
            void main()
            {
                gl_Position = position;
            })glsl",
        R"glsl(
            #version 430 core

            uniform float u_time;
            uniform vec4 u_color;

            out vec4 color;

            void main()
            {
                color = vec4(u_color.rgb * ((sin(u_time) + 1.0) * 0.5), 1.0);
            })glsl");
    GL_CALL(glUseProgram(shader_program));

    // Configure shader uniforms
    GL_CALL(uint32_t u_time_location = glGetUniformLocation(shader_program, "u_time"));
    ASSERT(u_time_location != -1);
    GL_CALL(uint32_t u_color_location = glGetUniformLocation(shader_program, "u_color"));
    ASSERT(u_color_location != -1);
    GL_CALL(glUniform4f(u_color_location, 0.03f, 0.67f, 0.92f, 1.0f));

    // Clear draw state before loop
    GL_CALL(glBindVertexArray(0));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glUseProgram(0));

    /**
     * Render loop
     */
    while (!glfwWindowShouldClose(window))
    {
        // Bind draw state buffers
        GL_CALL(glBindVertexArray(vertex_buffer));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_buffer[1]));
        GL_CALL(glUseProgram(shader_program));

        // Configure shader uniforms
        GL_CALL(glUniform1f(u_time_location, clock() / (float)CLOCKS_PER_SEC));

        // Clear frame buffer
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        // Draw vertices
        GL_CALL(glDrawElements(GL_TRIANGLES, e_buffer_length, GL_UNSIGNED_INT, nullptr));

        // Swap draw buffers
        GL_CALL(glfwSwapBuffers(window));

        // Run GLFW event loop
        GL_CALL(glfwPollEvents());
    }

    /**
     * Clean-up before exit
     */
    GL_CALL(glDeleteProgram(shader_program));
    glfwTerminate();

    return 0;
}
