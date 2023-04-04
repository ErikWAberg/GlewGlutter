#ifndef _GEOMETRY_H
#define _GEOMETRY_H


#include <GL/glew.h>
#include <memory>
#include <sstream>
#include <iostream>

#include <Node.h>
#include <vr/Vec3.h>
#include <vr/Vec2.h>
#include <vr/Vec4.h>
#include <vr/ObjLoader.h>
#include <vr/Timer.h>
#include <vr/DrawText.h>
#include <State.h>
#include <Transform.h>


class Geometry;

typedef std::vector<std::shared_ptr<Geometry> > GeometrySharedPtrVector;

class Geometry : public Node {
public:
    Geometry();

    ~Geometry();


    //Node
    void accept(NodeVisitor &nodeVisitor);
    
    //Geometry
    void createGLBuffers(void);

    void draw();
    

    void setVertices(const vr::Vec3Vector &vertices);

    void setNormals(const vr::Vec3Vector &normals) {
        m_normals = normals;
    }

    void setIndices(const vr::UIntVector &indices) {
        m_indices = indices;
    }

    void setTexCoords(const vr::Vec2Vector &texCoords) {
        m_texCoords = texCoords;
    }

    void setMaterial(const vr::material_t &material);

    void drawMesh();

private:
    void updateData();
    
    vr::Vec3Vector m_vertices;
    vr::UIntVector m_indices;
    vr::Vec3Vector m_normals;

    vr::Vec2Vector m_texCoords;
    
    float m_maxDistance;
    vr::Vec3 m_center;

    mutable GLuint m_VAO;
    mutable GLuint m_VAO_Positions;
    mutable GLuint m_VBO[4];


};


#endif