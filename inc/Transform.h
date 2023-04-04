#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <Group.h>
#include <vr/Matrix.h>

class Transform : public Group {
public:
    Transform(void) : Group() {/*stackIndex = 0; */  }

    ~Transform(void) {
    }


    //Node
    void accept(NodeVisitor &nodeVisitor);
    

};


#endif