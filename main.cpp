#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define ASSERT(x) if (!(x)) __debugbreak();
#define gl_call(x) gl_clear_error();\
    x;\
    ASSERT(gl_log_call(#x, __FILE__, __LINE__))

static void gl_clear_error()
{
    while (glGetError());
}


static bool gl_log_call(const char *function, const char * source_file, uint32_t line_number)
{
    while (GLenum gl_error = glGetError())
    {
        fprintf(stderr, "ERROR | OpenGL [0x%04x] %s\n    in %s @ %s:%d)", gl_error, "<message>", function, source_file, line_number);
        return false;
    }

    return true;
}


static GLuint compile_gl_shader(GLenum shader_type, const std::string& source)
{
    const char* source_string = source.c_str();

    gl_call(uint32_t shader_id = glCreateShader(shader_type));
    gl_call(glShaderSource(shader_id, 1, &source_string, nullptr));
    gl_call(glCompileShader(shader_id));

    int32_t compile_result;
    gl_call(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result));
    if (compile_result == GL_FALSE)
    {
        int log_length;
        gl_call(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length));
        char* log_message = (char *)alloca(log_length * sizeof(char));
        gl_call(glGetShaderInfoLog(shader_id, log_length, &log_length, log_message));

        std::cout << "Failed to compile shader" << std::endl;
        std::cout << log_message << std::endl;

        gl_call(glDeleteShader(shader_id));

        return 0;
    }

    return shader_id;
}


static GLuint create_gl_shader(const std::string& vertex_source, const std::string& fragment_source)
{
    gl_call(GLuint program_id = glCreateProgram());

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

    gl_call(glAttachShader(program_id, vertex_id));
    gl_call(glAttachShader(program_id, fragment_id));

    gl_call(glLinkProgram(program_id));
    gl_call(glValidateProgram(program_id));

    // TODO: Handle shader link and validation errors

    gl_call(glDeleteShader(vertex_id));
    gl_call(glDeleteShader(fragment_id));

    std::cout << "INFO | Shader program created successfully [id: " << program_id << "]" << std::endl;

    return program_id;
}


int main(void)
{
    /**
     * Initialize window
     */
    GLFWwindow* window;

    if (!glfwInit())
    {
        std::cout << "ERROR | GLFW failed to initialize" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(800, 800, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        std::cout << "ERROR | GLFW window failed to initialize" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    std::cout << "INFO | GLFW initialized" << std::endl;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR | GLEW failed to initialize" << std::endl;
        return -1;
    }
    std::cout << "INFO | GLEW initialized, OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /**
     * Configure rendering context
     */
    GLuint vertex_buffer;
    GLuint element_buffer;
    GLuint shader_program;

    // Configure vertex/position buffer
    // "component" refers to one part of a 2D vertex (i.e. x or y)
    const uint32_t v_component_size_bytes = sizeof(GLfloat);
    const uint32_t v_component_count = 2;
    const uint32_t vertex_count = 4;

    const uint32_t v_buffer_length = vertex_count * v_component_count;
    const uint32_t v_buffer_size = v_buffer_length * v_component_size_bytes;
    const uint32_t v_buffer_stride = v_component_count * v_component_size_bytes;

    const GLfloat vertex_data[v_buffer_length] = {
        -0.5f,   0.5f,
         0.5f,   0.5f,
        -0.5f,  -0.5f,
         0.5f,  -0.5f,
    };

    gl_call(glGenBuffers(1, &vertex_buffer));
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
    gl_call(glBufferData(GL_ARRAY_BUFFER, v_buffer_size, &vertex_data, GL_STATIC_DRAW));
    gl_call(glVertexAttribPointer(0, v_component_count, GL_FLOAT, false, v_buffer_stride, (const void*)0));
    gl_call(glEnableVertexAttribArray(0));

    // Configure index/element buffer
    const uint32_t e_component_size_bytes = sizeof(GLuint);
    const uint32_t e_component_count = 3;
    const uint32_t element_count = 2;

    const uint32_t e_buffer_length = element_count * e_component_count;
    const uint32_t e_buffer_size = e_buffer_length * e_component_size_bytes;
    const uint32_t e_buffer_stride = e_component_count * e_component_size_bytes;

    const GLuint element_data[e_buffer_length] = {
        0, 1, 2,
        2, 1, 3,
    };

    gl_call(glGenBuffers(1, &element_buffer));
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer));
    gl_call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_buffer_size, &element_data, GL_STATIC_DRAW));

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
    gl_call(glUseProgram(shader_program));

    // Configure shader uniforms
    gl_call(uint32_t u_time_location = glGetUniformLocation(shader_program, "u_time"));
    ASSERT(u_time_location != -1);
    gl_call(uint32_t u_color_location = glGetUniformLocation(shader_program, "u_color"));
    ASSERT(u_color_location != -1);
    gl_call(glUniform4f(u_color_location, 0.03f, 0.67f, 0.92f, 1.0f));

    /**
     * Render loop
     */
    while (!glfwWindowShouldClose(window))
    {
        // Clear draw buffer
        gl_call(glClear(GL_COLOR_BUFFER_BIT));

        // Configure shader time uniform
        gl_call(glUniform1f(u_time_location, clock() / (float)CLOCKS_PER_SEC));

        // Draw vertices
        gl_call(glDrawElements(GL_TRIANGLES, e_buffer_length, GL_UNSIGNED_INT, nullptr));

        // Swap draw buffers
        gl_call(glfwSwapBuffers(window));

        gl_call(glfwPollEvents());
    }

    /**
     * Clean-up before exit
     */
    gl_call(glDeleteProgram(shader_program));
    glfwTerminate();

    return 0;
}
