#include <Transform.h>
#include <Geometry.h>

void Transform::accept(NodeVisitor &nodeVisitor) {
    nodeVisitor.visit(this);
}
