#include "program.hpp"

#include "vertexshader.hpp"
#include "fragmentshader.hpp"

#include "lib/gl.hpp"

#include <iostream>

Program::Program()
    : gl(new GLProgram()),
      linked(false)
{
    gl->program = glCreateProgram();
    if (gl->program == 0) {
        std::cerr << "Program::Program: could not create program handle." << std::endl;
    }
}

Program::~Program()
{
    if (gl->program != 0) {
        glDeleteProgram(gl->program);
    }
}

static void print_program_error(GLuint program)
{
    GLint log_len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

    GLchar * log_info = new GLchar[log_len];
    glGetProgramInfoLog(program, log_len, NULL, log_info);

    std::string error_msg = "Program::attach: could not link program: ";
    std::cerr << error_msg << log_info << std::endl;

    delete[] log_info;
}

bool Program::attach(VertexShader & vs, FragmentShader & fs)
{
    if (gl->program == 0) {
        std::cerr << "Program::attach program handle is not valid." << std::endl;
        return false;
    }

    if (!vs.is_valid()) {
        std::cerr << "Program::attach: vertex shader is not valid." << std::endl;
        return false;
    }

    if (!fs.is_valid()) {
        std::cerr << "Program::attach: fragment shader is not valid." << std::endl;
        return false;
    }

    glAttachShader(gl->program, vs.gl->shader);
    glAttachShader(gl->program, fs.gl->shader);

    // associate vertex attribute index with a shader input variable
    glBindAttribLocation(gl->program, VERTEX_POSITION_INDEX, "vertex_position");
    glBindAttribLocation(gl->program, VERTEX_COLOR_INDEX, "vertex_color");
    glBindAttribLocation(gl->program, VERTEX_NORMAL_INDEX, "vertex_normal");
    glBindAttribLocation(gl->program, VERTEX_TEX_COORD_INDEX, "vertex_tex_coord");

    glLinkProgram(gl->program);

    glDetachShader(gl->program, vs.gl->shader);
    glDetachShader(gl->program, fs.gl->shader);

    GLint status;
    glGetProgramiv(gl->program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        print_program_error(gl->program);
        linked = false;
    } else {
        linked = true;
    }

    return linked;
}

void Program::use_none()
{
    glUseProgram(0);
}

void Program::use()
{
    glUseProgram(gl->program);
}

bool Program::is_valid() const
{
    return gl->program != 0 && linked;
}

static GLint get_uniform_location(GLuint program, GLchar const * name)
{
    GLint location = glGetUniformLocation(program, name);

    if (location == -1) {
        std::cerr << "Program::get_uniform_location: could not get uniform location for \"" << name << "\"." << std::endl;
    }

    return location;
}

void Program::set_uniform(std::string name, int value)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniform1i(location, value);
}

void Program::set_uniform(std::string name, bool value)
{
    set_uniform(name, value ? 1 : 0);
}

void Program::set_uniform(std::string name, float value)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniform1f(location, value);
}

void Program::set_uniform(std::string name, glm::mat3 const & value)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::set_uniform(std::string name, glm::mat4 const & value)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::set_uniform(std::string name, glm::vec2 const & value)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniform2f(location, value[0], value[1]);
}

void Program::set_uniform(std::string name, glm::vec3 const & value)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniform3f(location, value[0], value[1], value[2]);
}

void Program::set_uniform(std::string name, glm::vec4 const & value)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniform4f(location, value[0], value[1], value[2], value[3]);
}

void Program::set_uniform(std::string name, std::vector<float> const & values)
{
    GLint location = get_uniform_location(gl->program, name.c_str());
    glUniform1fv(location, values.size(), &values[0]);
}
