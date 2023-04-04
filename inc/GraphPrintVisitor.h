#ifndef _GRAPH_PRINT_VISITOR_H
#define _GRAPH_PRINT_VISITOR_H

#include <NodeVisitor.h>
#include <string>

class GraphPrintVisitor : public NodeVisitor {
public:
    GraphPrintVisitor() : m_graphDepth(0) {
    }

    ~GraphPrintVisitor() {
    }

    void visit(Group *groupNode);

    void visit(Geometry *geometryNode);

    void visit(Transform *transformNode);

    void visit(Camera *cameraNode);

    void visit(Shadow *shadowNode);

    std::string indent();

private:
    int m_graphDepth;
};

#endif