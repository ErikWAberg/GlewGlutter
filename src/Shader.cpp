#include <Shader.h>
#include <vr/shaderUtils.h>
#include <vr/glUtils.h>
#include <vr/FileSystem.h>
#include <fstream>
#include <stdlib.h>

GLuint Shader::loadShader(const std::string &path, GLuint shader_type, const std::string &sourceMod) {
    // Create a vertex shader
    glCreateShaderObjectARB(shader_type);
    CheckErrorsGL(std::string("glCreateShaderObjectARB: ") + path);

    std::string filepath = vr::FileSystem::convertToNativeFilePath(path);

    if (!vr::FileSystem::exists(filepath)) {
        std::cerr << "File '" << filepath << "\' does not exist" << std::endl;
        return 0;
    }

    // Open the shader text file
    std::ifstream in;
    in.open(filepath.c_str(), std::ios::in);
    if (!in.is_open())
        return 0;

    std::string source;
    std::string line;
    int lineNumber = 0;
    while (!in.eof()) {
        if (lineNumber == 1) {
            source += sourceMod;
        }
        std::getline(in, line);
        source += line;
        source += "\n";
        lineNumber++;
    }

    GLuint id = vr::loadShaderFromString(source, shader_type, path);
    if (!id) {
        std::cerr << "Error when compiling shader file: '" << filepath << "\'" << std::endl;
        return 0;
    }

    return id;
}

ShaderProgram *Shader::createShaderProgram(const std::string shaderName, const std::string vShaderPath, const std::string fShaderPath, const std::string &sourceMod) {
    // Create a vertex shader from a string
    std::cout << "Loading shaders: " << vShaderPath << ", " << fShaderPath << std::endl;
    ShaderProgram *sh(new ShaderProgram());
    sh->setName(shaderName);
   // sh->program = glCreateProgram();

    sh->v_shader = loadShader(vShaderPath, GL_VERTEX_SHADER, sourceMod);
    if (!sh->v_shader) {
        std::cerr << "vshader error" << std::endl;
        exit(0);
    }


    sh->f_shader = loadShader(fShaderPath, GL_FRAGMENT_SHADER, sourceMod);
    if (!sh->f_shader) {
        std::cerr << "fshader error" << std::endl;
        exit(0);
    }


    // Create a program object where the both shaders are attached.
    // A Shader Program consists of 1 or two shaders.
    sh->program = glCreateProgramObjectARB();
    glAttachObjectARB(sh->program, sh->v_shader);
    glAttachObjectARB(sh->program, sh->f_shader);
    //std::cout << "Shader location: " << sh->program << std::endl;
    // Try to link the program (with the two attached shaders)
    DO_GL_AND_CHECK(glLinkProgramARB(sh->program));

    // Check if it was successful
    if (!vr::checkLinkStatus(sh->program)) {
        std::cerr << "Could not link program " << shaderName << std::endl;
        exit(0);
    }

    CheckErrorsGL("BEGIN :GetLocation");
    CheckErrorsGL("END :GetLocation");
    return sh;
}



