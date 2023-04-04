#ifndef _RENDER_VISITOR_H
#define _RENDER_VISITOR_H

#include <NodeVisitor.h>

class RenderVisitor : public NodeVisitor {
public:
    RenderVisitor(void) {
    }

    ~RenderVisitor(void) {
    }

    void visit(Group *groupNode);

    void visit(Geometry *geometryNode);

    void visit(Transform *transformNode);

    void visit(Camera *cameraNode);

    void visit(Shadow *shadowNode);
private:
};

#endif