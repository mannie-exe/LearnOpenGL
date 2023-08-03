#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


using namespace std;


static void GLAPIENTRY handle_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""),
        type, severity, message);
    exit(-1);
}


static GLuint compile_gl_shader(GLenum shader_type, const string& source)
{
    const char* source_string = source.c_str();

    uint32_t shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &source_string, nullptr);
    glCompileShader(shader_id);

    int32_t compile_result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);
    if (compile_result == GL_FALSE)
    {
        int log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
        char* log_message = (char *)alloca(log_length * sizeof(char));
        glGetShaderInfoLog(shader_id, log_length, &log_length, log_message);

        cout << "Failed to compile shader" << endl;
        cout << log_message << endl;

        glDeleteShader(shader_id);

        return 0;
    }

    return shader_id;
}


static GLuint create_gl_shader(const string& vertex_source, const string& fragment_source)
{
    GLuint program_id = glCreateProgram();

    cout << "Creating shader program [" << program_id << "]:" << endl;
    cout << "    -> VERTEX" << vertex_source << endl;
    cout << "    -> FRAGMENT" << fragment_source << endl;

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

    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);

    glLinkProgram(program_id);
    glValidateProgram(program_id);

    // TODO: Handle shader link and validation errors

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    cout << "Shader program created successfully [" << program_id << "]" << endl;

    return program_id;
}


int main(void)
{
    /**
     * Initialize
     */
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    cout << "Initialized GLFW successfully" << endl;

    if (glewInit() != GLEW_OK)
        return -1;
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(handle_gl_error, nullptr);
    cout << "Initialized GLEW sucessfully with OpenGL version: " << glGetString(GL_VERSION) << endl;

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

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, v_buffer_size, &vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, v_component_count, GL_FLOAT, false, v_buffer_stride, (const void*)0);
    glEnableVertexAttribArray(0);

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

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_buffer_size, &element_data, GL_STATIC_DRAW);

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
    glUseProgram(shader_program);

    /**
     * Render loop
     */
    while (!glfwWindowShouldClose(window))
    {
        // Clear draw buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw vertices
        glDrawElements(GL_TRIANGLES, e_buffer_length, GL_UNSIGNED_INT, nullptr);

        // Swap draw buffers
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    /**
     * Exit
     */
    glDeleteProgram(shader_program);

    glfwTerminate();

    return 0;
}
