#include "shader.hpp"

#include "lib/gl.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

static GLenum translate_shader_type(ShaderType type)
{
    switch (type) {
    case ShaderType::VERTEX:
        return GL_VERTEX_SHADER;
    case ShaderType::FRAGMENT:
        return GL_FRAGMENT_SHADER;
    default:
        return 0;
    }
}

Shader::Shader(ShaderType type)
    : gl(new GLShader()),
      valid(false),
      last_load_modified(0)
{
    gl->type = translate_shader_type(type);
    gl->shader = glCreateShader(gl->type);
}

Shader::~Shader()
{
    if (gl->shader != 0) {
        glDeleteShader(gl->shader);
    }
}

bool Shader::compile_from_file(std::string filename)
{
    std::string source;
    if (!read(filename, source)) {
        return false;
    }

    this->filename = filename;
    last_load_modified = last_modified();

    valid = compile(source);

    return valid;
}

bool Shader::compile_from_source(std::string const & source)
{
    last_load_modified = 0;

    valid = compile(source);

    return valid;
}

bool Shader::recompile()
{
    if (!is_from_file()) {
        std::cerr << "Shader::recompile: attempted to recompile shader created from source." << std::endl;
        return false;
    }

    return compile_from_file(filename);
}

bool Shader::is_valid() const
{
    return valid;
}

bool Shader::is_from_file() const
{
    return last_load_modified != 0;
}

bool Shader::is_modified() const
{
    return is_from_file() && (last_modified() > last_load_modified);
}

bool Shader::read(std::string filename, std::string & source)
{
    std::ifstream f(filename.c_str(), std::ifstream::in | std::ifstream::binary);

    if (f.is_open()) {
        std::stringstream buffer;
        buffer << f.rdbuf();
        source = buffer.str();
        return true;
    } else {
        std::cerr << "Shader::read_to: could not read from file \"" << filename << "\"." << std::endl;
        return false;
    }
}

static void print_shader_error(GLuint shader, std::string filename)
{
    GLint log_len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

    GLchar * log_info = new GLchar[log_len];
    glGetShaderInfoLog(shader, log_len, NULL, log_info);

    std::cerr << "Shader::make: could not compile \"" << filename << "\": " << log_info << std::endl;

    delete[] log_info;
}

bool Shader::compile(std::string const & source)
{
    char const * shader_source = source.c_str();

    glShaderSource(gl->shader, 1, &shader_source, NULL);
    glCompileShader(gl->shader);

    GLint status;
    glGetShaderiv(gl->shader, GL_COMPILE_STATUS, &status);
    if (status ==  GL_FALSE) {
        print_shader_error(gl->shader, filename);
        return false;
    }

    return true;
}

time_t Shader::last_modified() const
{
   struct stat buf;

   if (stat(filename.c_str(), &buf) == 0) {
       return buf.st_mtime;
   } else {
       std::cerr << "Shader:last_modified: could not get file status for \"" << filename << "\"." << std::endl;
       return 0;
   }
}
