#ifndef PROGRAM_HPP_INCLUDED
#define PROGRAM_HPP_INCLUDED

#include "lib/glm.hpp"

#include <memory>
#include <string>
#include <vector>

struct GLProgram;

class VertexShader;
class FragmentShader;

class Program {
public:
    Program();
    ~Program();

    Program(Program const &) = delete;
    Program & operator=(Program const &) = delete;

    static void use_none();

    bool attach(VertexShader & vs, FragmentShader & fs);
    void use();

    bool is_valid() const;

    void set_uniform(std::string name, int value);
    void set_uniform(std::string name, bool value);
    void set_uniform(std::string name, float value);
    void set_uniform(std::string name, glm::mat3 const & value);
    void set_uniform(std::string name, glm::mat4 const & value);
    void set_uniform(std::string name, glm::vec2 const & value);
    void set_uniform(std::string name, glm::vec3 const & value);
    void set_uniform(std::string name, glm::vec4 const & value);
    void set_uniform(std::string name, std::vector<float> const & values);
private:
    std::unique_ptr<GLProgram> gl;

    bool linked;
};

#endif
