#ifndef _LAB1_H
#define _LAB1_H

//#include <SDL2/SDL_opengl.h>
//#include <GL/glut.h>


#include <vr/Vec3.h>

void display(void);

void reshapeWindow(int w, int h);

void idle(void);

void mouse(int button, int dir, int x, int y);

void mouseMotion(int x, int y);

void doMovement(int moveLight, vr::Vec3 movement);

void toggleEnableShadows();

void toggleDrawFPS();

void togglePostEffects();

void toggleShowBuffers();

void renderString(float x, float y, const unsigned char *string);

void toggleLight(int lightIndex);

#endif