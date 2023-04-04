#include <Material.h>
#include <State.h>

Material::Material(const vr::material_t& material) {
    m_material = material;
    /*std::cout << "Ambient: " << material.ambient << std::endl;
    std::cout << "Diffuse: " << material.diffuse << std::endl;
    std::cout << "Specular: " << material.specular << std::endl;
    std::cout << "Emission: " << material.emission << std::endl;
    std::cout << "Transmittance: " << material.transmittance << std::endl;
    std::cout << "Shininess: " << material.shininess << std::endl;
    std::cout << "ior: " << material.ior << std::endl;
    std::cout << "dissolve: " << material.dissolve << std::endl;*/
}

void Material::apply(ShaderProgram* shaderProgram) {

    GLint Ambient = shaderProgram->getUniform("Ambient");
    glUniform3fv(Ambient, 1, m_material.ambient.ptr());

    GLint Diffuse = shaderProgram->getUniform("Diffuse");
    glUniform3fv(Diffuse, 1, m_material.diffuse.ptr());

    GLint Specular = shaderProgram->getUniform("Specular");
    glUniform3fv(Specular, 1, m_material.specular.ptr());

    GLint Shininess = shaderProgram->getUniform("Shininess");
    glUniform1f(Shininess, m_material.shininess);

}