#include <Group.h>

void Group::addChild(Node *child) {
    children.push_back(child);
}

void Group::accept(NodeVisitor &visitor) {
    visitor.visit(this);
}

void Group::attachLight(Light *light) {
    State::addLight(light);
}

void Group::setGroupName(std::string name) {
    groupName = name;
}

std::string Group::getGroupName(void) {
    return groupName;
}


void Group::attachShader(ShaderProgram *shaderProgram) {
    this->State::setShaderProgram(shaderProgram);
    for (std::list<Node *>::const_iterator child = children.begin(); child != children.end(); child++) {
        Group *v = dynamic_cast<Group *>(*child);
        if (v != 0) {
            v->Group::attachShader(shaderProgram);
        } else {
            (*child)->State::setShaderProgram(shaderProgram);
        }

    }
}

