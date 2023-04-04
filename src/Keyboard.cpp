#include <GlewGlutter.h>

#include <Keyboard.h>
#include <iostream>

bool lightMovement;
int lightMovementIndex = 0;

std::map<unsigned char, key_status_t> m_keyStatusMap;

void setupKeys() {
    lightMovement = false;
    key_status_t keyW, keyA, keyS, keyD, keyZ, keyX;
    keyW.key = 'w';
    keyW.isPressed = false;
    keyA.key = 'a';
    keyA.isPressed = false;
    keyS.key = 's';
    keyS.isPressed = false;
    keyD.key = 'd';
    keyD.isPressed = false;
    keyZ.key = 'z';
    keyZ.isPressed = false;
    keyX.key = 'x';
    keyX.isPressed = false;
    m_keyStatusMap['w'] = keyW;
    m_keyStatusMap['a'] = keyA;
    m_keyStatusMap['s'] = keyS;
    m_keyStatusMap['d'] = keyD;
    m_keyStatusMap['z'] = keyZ;
    m_keyStatusMap['x'] = keyX;
}

void setMovementVector(unsigned char key, float dx, float dy, float dz) {
    m_keyStatusMap[key].isPressed = true;
//TODO
   /* if (glfwGetTime() - m_keyStatusMap[key].previousActivation > 2) {
        if (lightMovement) {
            m_keyStatusMap[key].movement = vr::Vec3( -1 * dx, -1 * dy, -1 * dz);
        } else {
            m_keyStatusMap[key].movement = vr::Vec3( dx, dy, dz);
        }
        m_keyStatusMap[key].previousActivation = glfwGetTime();
    }*/

}

void updateKeyPress() {
    vr::Vec3 movementSum;
    for (std::map<unsigned char, key_status_t>::iterator it = m_keyStatusMap.begin(); it != m_keyStatusMap.end(); ++it) {
        if (it->second.isPressed) {
            keyboard(it->second.key, 0, 0);
            movementSum += it->second.movement;
        }
    }
    if(!lightMovement) {
        doMovement(-1, movementSum);    
    } else {
        doMovement(lightMovementIndex, movementSum);
    }
}

void keyboard(unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
        case 'Q':
        case 'q':
        case 27:
            exit(0);
        case 'a':
            setMovementVector(keyPressed, 0.04f, 0.0f, 0.0f);
            break;
        case 'd':
            setMovementVector(keyPressed, -0.04f, 0.0f, 0.0f);
            break;
        case 'w':
            setMovementVector(keyPressed, 0.0f, 0.0f, 0.04f);
            break;
        case 's':
            setMovementVector(keyPressed, 0.0f, 0.0f, -0.04f);
            break;
        case 'z':
            setMovementVector(keyPressed, 0.0f, 0.04f, 0.0f);
            break;
        case 'x':
            setMovementVector(keyPressed, 0.0f, -0.04f, 0.0f);
            break;
        case 'l':
            lightMovement = !lightMovement;
            break;
        case 'f':
            toggleDrawFPS();
            break;
        case 'p':
            togglePostEffects();
            break;
        case 'o':
            toggleEnableShadows();
            break;
        case 'i':
            toggleShowBuffers();
            break;
        case 'u':
            toggleLight(lightMovementIndex);
            break;
            
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
            lightMovementIndex = ((int)keyPressed) - 49;
            break;

        default:
            break;
    }
}


void keyboardSpecial(int c, int x, int y) {
}

void keyboardSpecialUp(unsigned char keyReleased, int x, int y) {
    switch (keyReleased) {
        case 'w':
        case 'a':
        case 's':
        case 'd':
        case 'z':
        case 'x':
            m_keyStatusMap[keyReleased].isPressed = false;
            break;
        default:
            break;
    }

}