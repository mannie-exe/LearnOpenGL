#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError());
}


static bool GLLogCall(const char *function, const char * source_file, uint32_t line_number)
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

    uint32_t shader_id = glCreateShader(shader_type);
    GLCall(glShaderSource(shader_id, 1, &source_string, nullptr));
    GLCall(glCompileShader(shader_id));

    int32_t compile_result;
    GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result));
    if (compile_result == GL_FALSE)
    {
        int log_length;
        GLCall(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length));
        char* log_message = (char *)alloca(log_length * sizeof(char));
        GLCall(glGetShaderInfoLog(shader_id, log_length, &log_length, log_message));

        std::cout << "Failed to compile shader" << std::endl;
        std::cout << log_message << std::endl;

        GLCall(glDeleteShader(shader_id));

        return 0;
    }

    return shader_id;
}


static GLuint create_gl_shader(const std::string& vertex_source, const std::string& fragment_source)
{
    GLuint program_id = glCreateProgram();

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

    GLCall(glAttachShader(program_id, vertex_id));
    GLCall(glAttachShader(program_id, fragment_id));

    GLCall(glLinkProgram(program_id));
    GLCall(glValidateProgram(program_id));

    // TODO: Handle shader link and validation errors

    GLCall(glDeleteShader(vertex_id));
    GLCall(glDeleteShader(fragment_id));

    std::cout << "INFO | Shader program created successfully [id: " << program_id << "]" << std::endl;

    return program_id;
}


int main(void)
{
    /**
     * Initialize
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
    std::cout << "INFO | GLFW initialized" << std::endl;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR | GLEW failed to initialize" << std::endl;
        return -1;
    }
    std::cout << "INFO | GLEW initialized, OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /**
     * Configure
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

    GLCall(glGenBuffers(1, &vertex_buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, v_buffer_size, &vertex_data, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, v_component_count, GL_FLOAT, false, v_buffer_stride, (const void*)0));
    GLCall(glEnableVertexAttribArray(0));

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

    GLCall(glGenBuffers(1, &element_buffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_buffer_size, &element_data, GL_STATIC_DRAW));

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

            out vec4 color;

            void main()
            {
                color = vec4(0.03, 0.67, 0.92, 1.0);
            })glsl");
    GLCall(glUseProgram(shader_program));

    /**
     * Render loop
     */
    while (!glfwWindowShouldClose(window))
    {
        // Clear draw buffer
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // Draw vertices
        GLCall(glDrawElements(GL_TRIANGLES, e_buffer_length, GL_UNSIGNED_INT, nullptr));

        // Swap draw buffers
        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());
    }

    /**
     * Exit
     */
    GLCall(glDeleteProgram(shader_program));
    glfwTerminate();

    return 0;
}
