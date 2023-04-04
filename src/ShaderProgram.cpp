#include <ShaderProgram.h>
#include <iostream>


bool ShaderProgram::checkForError() {
    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR) {
        std::cerr << "glError: " << gluErrorString(glErr) << std::endl;
        return true;
    }

    return false;
}
void ShaderProgram::useProgram(){
    glUseProgram(program);
}

void ShaderProgram::setName(std::string shaderName) {
    m_name = shaderName;
}

std::string ShaderProgram::getName() {
    return m_name;
}

GLint ShaderProgram::getUniform(const GLchar *uniformName) {
    if (!uniformName)
        throw std::runtime_error("uniformName was NULL");

    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &id);
    if(program != id) {
        std::cerr << "Fetching uniform: " << uniformName <<" non-current program, using shader: " << m_name <<  std::endl;
    }

    GLint uniform = glGetUniformLocation(program, uniformName);
    if (uniform == -1 ) { //&& program == 5
        std::cerr << "Uniform not found: " << uniformName << " using shader: " << m_name << std::endl;
    }

    return uniform;
}



