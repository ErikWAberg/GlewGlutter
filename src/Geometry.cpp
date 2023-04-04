#include <vr/Matrix.h>
#include <Geometry.h>

#define BUFFER_OFFSET(i) ((void*)(i))

Geometry::Geometry() : Node() {
   // this->State::setHasModel(true);
}

void Geometry::accept(NodeVisitor &nodeVisitor) {
    nodeVisitor.visit(this);
}


void Geometry::draw() {    
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
}

void Geometry::drawMesh() {
    glBindVertexArray(m_VAO_Positions);
    glDrawElements(GL_TRIANGLES, (GLsizei) m_indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
}


void Geometry::setVertices(const vr::Vec3Vector &vertices) {
    m_vertices = vertices;
    m_maxDistance = 0;
    m_center.set(0, 0, 0);

    if (m_vertices.empty())
        return;

    updateData();
}


void Geometry::updateData() {

    vr::Vec3 minimum(1E30f, 1E30f, 1E30f);
    vr::Vec3 maximum(-1E30f, -1E30f, -1E30f);
    vr::Vec3 sum;
    for (vr::Vec3Vector::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it) {
        vr::Vec3 v = *it;
        sum += v;

        for (size_t i = 0; i < 3; i++) {
            minimum[i] = vr::minimum(minimum[i], v[i]);
            maximum[i] = vr::maximum(maximum[i], v[i]);

        }
    }
    m_center = sum / (vr::Vec3::value_type) m_vertices.size();
    m_maxDistance = (minimum - maximum).length();
}


void Geometry::createGLBuffers(void) {
    // std::cout << "Creating data buffers, Index: "  << m_indices.size() << " Vertex: " << m_vertices.size() << " Normals: " << m_normals.size() << std::endl;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(4, m_VBO);
    glBindVertexArray(m_VAO);


    //Position
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vr::Vec3::value_type) * 3 * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Normal
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vr::Vec3::value_type) * 3 * m_normals.size(),
            &m_normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Texture

    if (m_texCoords.size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vr::Vec2::value_type) * 2 * m_texCoords.size(),
                &m_texCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


Geometry::~Geometry() {
    /*if (m_texture)
    {
      glDeleteTextures(1, &m_texture);
    }*/
}


void Geometry::setMaterial(const vr::material_t& material) {
    State::setMaterial(new Material(material));
}
