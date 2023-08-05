#include <iostream>
#include <string>
#include "renderer.h"
#include <GLFW/glfw3.h>
#include "data_buffer.h"
#include "vertex_array.h"
#include "attrib_array.h"
#include "shader_program.h"


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
        GL_VertexArray<float> vertex_array;
        GL_DataBuffer<float> vertex_buffer;
        GL_AttribArray attrib_array;
        GL_DataBuffer<uint32_t> index_buffer;
        {
            // VBO; configure vertex position buffer
            const uint32_t v_component_count = 2;
            const uint32_t v_count = 4;
            const uint32_t v_buffer_count = v_count * v_component_count;
            const float vertex_data[v_buffer_count] = {
                -0.5f,   0.5f,
                 0.5f,   0.5f,
                -0.5f,  -0.5f,
                 0.5f,  -0.5f,
            };
            vertex_buffer.set_data(GL_ARRAY_BUFFER, v_buffer_count, vertex_data, GL_STATIC_DRAW);

            // VAA; configure VBO layout
            attrib_array.push<float>(2, false);

            // VAO; configure vertex array object
            vertex_array.set_buffer(&attrib_array, &vertex_buffer);
            
            // EBO; configure index/element buffer
            const uint32_t e_component_count = 3;
            const uint32_t e_count = 2;
            const uint32_t e_buffer_count = e_count * e_component_count;
            const uint32_t element_data[e_buffer_count] = {
                0, 1, 2,
                2, 1, 3,
            };
            index_buffer.set_data(GL_ELEMENT_ARRAY_BUFFER, e_buffer_count, element_data, GL_STATIC_DRAW);

            // Clear GL buffer state
            vertex_array.unbind();
            index_buffer.unbind();
            vertex_buffer.unbind();
        }

        /**
         * Shaders
         */
        GL_ShaderProgram shader_program;
        uint32_t u_time_location;
        uint32_t u_color_location;
        {
            // Initialize shader program
            shader_program.create("./res/shaders/example.vert", "./res/shaders/example.frag");
            shader_program.bind();

            // Configure shader uniforms
            GL_CALL(u_time_location = glGetUniformLocation(shader_program.get_id(), "u_time"));
            GL_CALL(u_color_location = glGetUniformLocation(shader_program.get_id(), "u_color"));
            GL_CALL(glUniform4f(u_color_location, 0.03f, 0.67f, 0.92f, 1.0f));

            // Clear shader state
            //shader_program.unbind();
        }

        /**
         * Render Loop
         */
        while (!glfwWindowShouldClose(window))
        {
            // Configure GL buffer state
            vertex_array.bind();
            index_buffer.bind();

            // Configure GL shader state
            //shader_program.bind();
            GL_CALL(glUniform1f(u_time_location, clock() / (float)CLOCKS_PER_SEC));

            // Clear frame buffer
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
            // Draw buffers
            GL_CALL(glDrawElements(GL_TRIANGLES, index_buffer.get_count(), GL_UNSIGNED_INT, nullptr));
            // Swap frame buffers
            GL_CALL(glfwSwapBuffers(window));
            // Run GLFW event loop
            GL_CALL(glfwPollEvents());

            // Clear GL state
            //shader_program.unbind();
            index_buffer.unbind();
            vertex_array.unbind();
        }
    }

    /**
     * Clean-up before exiting
     */
    glfwTerminate();
}
