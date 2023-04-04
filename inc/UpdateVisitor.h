#ifndef _UPDATE_VISITOR_H
#define _UPDATE_VISITOR_H

#include <NodeVisitor.h>


class UpdateVisitor : public NodeVisitor {
public:
    UpdateVisitor(void) {
    }

    ~UpdateVisitor(void) {
    }

    void visit(Group *groupNode);

    void visit(Geometry *geometryNode);

    void visit(Transform *transformNode);

    void visit(Camera *cameraNode);

    void visit(Shadow *shadowNode);
};

#endif