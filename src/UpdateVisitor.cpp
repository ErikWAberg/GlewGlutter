#include <UpdateVisitor.h>
#include <UpdateCallback.h>

#include <iostream>

#include <Group.h>
#include <Geometry.h>
#include <Camera.h>
#include <Shadow.h>

void UpdateVisitor::visit(Group *groupNode) {
    UpdateCallback *callback = groupNode->Node::getUpdateCallback();
    if (callback != NULL) {
        callback->update();
    }

    for (std::list<Node *>::const_iterator child = groupNode->children.begin(); child != groupNode->children.end(); child++) {
        (*child)->accept(*this);
    }

}

void UpdateVisitor::visit(Geometry *geometryNode) {
    UpdateCallback *callback = geometryNode->Node::getUpdateCallback();
    if (callback != NULL) {
        callback->update();
    }
}

void UpdateVisitor::visit(Transform *transformNode) {
    UpdateCallback *callback = transformNode->Node::getUpdateCallback();
    if (callback != NULL) {
        callback->update();
    }

    for (std::list<Node *>::const_iterator child = transformNode->children.begin(); child != transformNode->children.end(); child++) {
        (*child)->accept(*this);
    }
}

void UpdateVisitor::visit(Camera *cameraNode) {
    UpdateCallback *callback = cameraNode->Node::getUpdateCallback();
    if (callback != NULL) {
        callback->update();
    }

    for (std::list<Node *>::const_iterator child = cameraNode->children.begin(); child != cameraNode->children.end(); child++) {
        (*child)->accept(*this);
    }
}

void UpdateVisitor::visit(Shadow *shadowNode) {
    UpdateCallback *callback = shadowNode->Node::getUpdateCallback();
    if (callback != NULL) {
        callback->update();
    }

    for (std::list<Node *>::const_iterator child = shadowNode->children.begin(); child != shadowNode->children.end(); child++) {
        (*child)->accept(*this);
    }
}