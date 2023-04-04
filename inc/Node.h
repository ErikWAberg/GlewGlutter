#ifndef _NODE_H
#define _NODE_H

#include <State.h>
#include <NodeVisitor.h>
#include <UpdateCallback.h>

class Node : public State {
public:
    Node(void) : State(), m_updateCallback(NULL) {
    }

    ~Node(void) {
    }

    virtual void accept(NodeVisitor &nodeVisitor) = 0;

    void setUpdateCallback(UpdateCallback *updateCallback) {
        m_updateCallback = updateCallback;
    }

    UpdateCallback *getUpdateCallback() {
        return m_updateCallback;
    }

private:
    UpdateCallback *m_updateCallback;

};


#endif