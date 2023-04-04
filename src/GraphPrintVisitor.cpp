#include <GraphPrintVisitor.h>

#include <iostream>

#include <Group.h>
#include <Geometry.h>
#include <Camera.h>
#include <Shadow.h>

std::string GraphPrintVisitor::indent() {
    std::string indentation = "";
    
    for(int i = 0; i < m_graphDepth-1; i++) {
        indentation += "    ";        
    }
    if(m_graphDepth == 1) return indentation + "---";
    return indentation + "|---";
}


void GraphPrintVisitor::visit(Group *groupNode) {
    m_graphDepth++;
 
    std::string affix;
    affix = groupNode->getGroupName();
    if(affix.length() == 0) {
        affix = groupNode->State::getShaderName();
    }

    int children = groupNode->children.size();
    std::cout << indent() << "Group, depth: " << m_graphDepth << " children: " << children << ", obj/shader: " << affix <<  std::endl;
    
    for (std::list<Node *>::const_iterator child = groupNode->children.begin(); child != groupNode->children.end(); child++) {
        (*child)->accept(*this);
    }
    m_graphDepth--;

}

void GraphPrintVisitor::visit(Geometry *geometryNode) {
  /*  m_graphDepth++;
    std::string indentation = indent();
    m_graphDepth--;
    std::cout <<  indentation << "Geometry" << m_graphDepth  << " " << geometryNode->State::getShaderName() << std::endl;
*/
}

void GraphPrintVisitor::visit(Transform *transformNode) {
    m_graphDepth++;

    std::string affix;
    affix = transformNode->getGroupName();
    if(affix.length() == 0) {
        affix = transformNode->State::getShaderName();
    }

    int children = transformNode->children.size();
    std::cout << indent() << "Transform, depth: " << m_graphDepth << " children: " << children << ", obj/shader: " << affix <<  std::endl;
    
    for (std::list<Node *>::const_iterator child = transformNode->children.begin(); child != transformNode->children.end(); child++) {
        (*child)->accept(*this);      
    }
    m_graphDepth--;
}

void GraphPrintVisitor::visit(Camera *cameraNode) {
    m_graphDepth++;

    std::string affix;
    affix = cameraNode->getGroupName();
    if(affix.length() == 0) {
        affix = cameraNode->State::getShaderName();
    }

    int children = cameraNode->children.size();
    std::cout << indent() << "Camera, depth: " << m_graphDepth << " children: " << children << ", obj/shader: " << affix <<  std::endl;

    for (std::list<Node *>::const_iterator child = cameraNode->children.begin(); child != cameraNode->children.end(); child++) {
        (*child)->accept(*this);
    }
    m_graphDepth--;
}

void GraphPrintVisitor::visit(Shadow *shadowNode) {
    m_graphDepth++;

    std::string affix;
    affix = shadowNode->getGroupName();
    if(affix.length() == 0) {
        affix = shadowNode->State::getShaderName();
    }

    int children = shadowNode->children.size();
    std::cout << indent() << "Shadow, depth: " << m_graphDepth << " children: " << children << ", obj/shader: " << affix <<  std::endl;


    for (std::list<Node *>::const_iterator child = shadowNode->children.begin(); child != shadowNode->children.end(); child++) {
        (*child)->accept(*this);
    }
    m_graphDepth--;
}