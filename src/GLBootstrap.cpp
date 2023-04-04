#include <iostream>
#include <GLBootstrap.h>
#include <Keyboard.h>
#include <GlewGlutter.h>

// This is the callback we'll be registering with GLFW for errors.
// It'll just print out the error to the STDERR stream.
void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void initGLUT(int argc, char **argv, int winX, int winY) {


    if (!glfwInit()) {
    fprintf(stderr, "Failed initialize GLFW.");
    exit(EXIT_FAILURE);
  }

  // Set the error callback, as mentioned above.
  glfwSetErrorCallback(error_callback);

  // Set up OpenGL options.
  // Use OpenGL verion 4.1,
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  // GLFW_OPENGL_FORWARD_COMPAT specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed.
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // Indicate we only want the newest core profile, rather than using backwards compatible and deprecated features.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Make the window resize-able.
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a window to put our stuff in.
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

  // If the window fails to be created, print out the error, clean up GLFW and exit the program.
  if(!window) {
    fprintf(stderr, "Failed to create GLFW window.");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Use the window as the current context (everything that's drawn will be place in this window).
  glfwMakeContextCurrent(window);

  // Set the keyboard callback so that when we press ESC, it knows what to do.
  glfwSetKeyCallback(window, key_callback);

  printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

   /* glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(winX, winY);
    glutInitWindowPosition(50, 0);
    glutCreateWindow("The Glew Glutter");

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutKeyboardUpFunc(keyboardSpecialUp);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(reshapeWindow);
    glutIgnoreKeyRepeat(true);

    */
}

void initGL(void) {

    glewExperimental = GL_TRUE;
    glewInit();


    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl << std::endl;
    // Ensure we have the necessary OpenGL Shading Language extensions.
    if (
    glewGetExtension("GL_ARB_fragment_shader") != GL_TRUE ||
    glewGetExtension("GL_ARB_vertex_shader") != GL_TRUE ||
    glewGetExtension("GL_ARB_shader_objects") != GL_TRUE ||
    glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE) {
        std::cerr << "Driver does not support OpenGL Shading Language" << std::endl;
     //   exit(1);
    }

    //if(!GLEW_VERSION_3_2)
    //glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

}

void gl_bootstrap(int argc, char **argv, int winX, int winY) {
    initGLUT(argc, argv, winX, winY);
    initGL();

}