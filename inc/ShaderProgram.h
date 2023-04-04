#ifndef _SHADER_PROGRAM_H
#define _SHADER_PROGRAM_H

#include <GL/glew.h>
#include <stdexcept>
#include <string>
class ShaderProgram {

public:
    ShaderProgram() : v_shader(0), f_shader(0), program(0), tex0Uniform(0), hasTexture0(0) {
    }

    ~ShaderProgram() {
        if (v_shader)
            glDeleteShader(v_shader);
        if (f_shader)
            glDeleteShader(f_shader);
        if (program)
            glDeleteProgram(program);

        f_shader = v_shader = program = tex0Uniform = 0;
    }

    GLuint v_shader, f_shader, program;
    GLuint tex0Uniform;
    GLuint hasTexture0;

    bool checkForError();

    GLint getUniform(const GLchar *uniformName);

    void useProgram();

    void setName(std::string shaderName);

    std::string getName();

private:

    std::string m_name;

};


#endif