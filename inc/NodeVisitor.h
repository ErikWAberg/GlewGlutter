#ifndef _NODE_VISITOR_H
#define _NODE_VISITOR_H

class Node;

class Group;

class Geometry;

class Transform;

class Camera;

class Shadow;

class NodeVisitor {
public:
    NodeVisitor(void) {
    }

    ~NodeVisitor(void) {
    }

    virtual void visit(Group *groupNode) = 0;

    virtual void visit(Geometry *geometryNode) = 0;

    virtual void visit(Transform *transformNode) = 0;

    virtual void visit(Camera *cameraNode) = 0;

    virtual void visit(Shadow *shadowNode) = 0;
};

#endif
