#include "file_utils.h"
#include <sstream>
#include <fstream>


std::string read_file(const std::string& file_path)
{
    std::fstream file_stream(file_path);
    std::string line;
    std::stringstream string_stream;

    while (getline(file_stream, line))
    {
        string_stream << line << '\n';
    }

    return string_stream.str();
}
