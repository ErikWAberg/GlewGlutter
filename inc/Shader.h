#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <memory>

#include <iostream>
#include <ShaderProgram.h>

class Shader {
public:
    static Shader &instance() {
        static Shader instance;

        return instance;
    }

    ShaderProgram *createShaderProgram(const std::string shaderName, const std::string vShaderPath, const std::string fShaderPath, const std::string &sourceMod);

private:
    Shader() {
    };   // Constructor? (the {} brackets) are needed here.

    Shader(Shader const &);

    void operator=(Shader const &);

    GLuint loadShader(const std::string &path, GLuint shader_type, const std::string &sourceMod);
};


#endif