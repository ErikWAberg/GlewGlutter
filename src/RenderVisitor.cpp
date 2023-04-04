#include <RenderVisitor.h>
#include <iostream>
#include <Scene.h>

void RenderVisitor::visit(Group *groupNode) {
    groupNode->State::applyState();
    for (std::list<Node *>::const_iterator child = groupNode->children.begin(); child != groupNode->children.end(); child++) {
        (*child)->State::transform(groupNode->State::getModel());
        (*child)->accept(*this);
    }
    groupNode->State::resetState();
}

void RenderVisitor::visit(Geometry *geometryNode) {

    geometryNode->State::applyState();
    geometryNode->draw();
    geometryNode->State::resetState();
    
}

void RenderVisitor::visit(Transform *transformNode) {
    transformNode->State::applyState();

    for (std::list<Node *>::const_iterator child = transformNode->children.begin(); child != transformNode->children.end(); child++) {
        (*child)->State::transform(transformNode->State::getModel());
        (*child)->accept(*this);
    }

    transformNode->State::resetState();
}


void RenderVisitor::visit(Camera *cameraNode) {
    cameraNode->applyModel();
    cameraNode->attachMatricies(); 
    
    for (std::list<Node *>::const_iterator child = cameraNode->children.begin(); child != cameraNode->children.end(); child++) {
        (*child)->accept(*this);
    }
    cameraNode->resetModel();
   
}

void RenderVisitor::visit(Shadow *shadowNode) {
    shadowNode->doShadowPass();
    for (std::list<Node *>::const_iterator child = shadowNode->Group::children.begin(); child != shadowNode->Group::children.end(); child++) {
        (*child)->accept(*this);
    }
  
}