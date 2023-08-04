#include <iostream>
#include "renderer.h"


void gl_clear_error()
{
    while (glGetError());
}


bool gl_log_call(const char* function, const char* source_file, uint32_t line_number)
{
    while (GLenum gl_error = glGetError())
    {
        fprintf(stderr, "ERROR | OpenGL [0x%04x] %s\n    in %s @ %s:%d)", gl_error, "<message>", function, source_file, line_number);
        return false;
    }

    return true;
}
