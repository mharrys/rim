#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <memory>
#include <string>
#include <sys/stat.h>

class Program;

struct GLProgram;
struct GLShader;

enum class ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader {
    friend Program;
public:
    Shader(ShaderType type);
    ~Shader();

    Shader(Shader const &) = delete;
    Shader& operator=(Shader const &) = delete;

    bool compile_from_file(std::string filename);
    bool compile_from_source(std::string const & source);

    bool recompile();

    bool is_valid() const;
    bool is_from_file() const;
    bool is_modified() const;
private:
    bool read(std::string filename, std::string & source);
    bool compile(std::string const & source);
    time_t last_modified() const;

    std::unique_ptr<GLShader> gl;

    bool valid;

    std::string filename;
    time_t last_load_modified;
};

#endif
