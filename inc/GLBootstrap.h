#ifndef _GLBOOTSTRAP_H
#define _GLBOOTSTRAP_H

#include <GL/glew.h>

#ifdef _WIN32
#define  _CRT_SECURE_NO_WARNINGS 1

#include <windows.h>
#endif
#include <GLFW/glfw3.h>

//#include <GL/glut.h>

extern GLFWwindow* window;
void gl_bootstrap(int argc, char **argv, int winX, int winY);

#endif