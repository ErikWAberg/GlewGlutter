#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include <vr/Vec3.h>
#include <map>

typedef struct _key_status_t {
    unsigned char key;
    bool isPressed;
    float previousActivation;
    vr::Vec3 movement;
} key_status_t;

    void keyboard(unsigned char c, int x, int y);

    void keyboardSpecial(int c, int x, int y);

    void keyboardSpecialUp(unsigned char c, int x, int y);

    void updateKeyPress();
    void setupKeys();

    void setMovementVector(bool moveLight, unsigned char key, float dx, float dy, float dz);



#endif