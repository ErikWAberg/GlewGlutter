#ifndef _GROUP_H
#define _GROUP_H

#include <Node.h>
#include <list>

class Group : public Node {
public:
    Group(void) : Node() {
    }

    ~Group(void) {
    }

    //Group
    void addChild(Node *child);

    void attachShader(ShaderProgram *shaderProgram);

    void attachLight(Light *light);

    void setGroupName(std::string name);

    std::string getGroupName(void);

    //Node
    void accept(NodeVisitor &nodeVisitor);

    std::list<Node *> children; //TODO. fix public
    std::string groupName;
protected:

};

#endif