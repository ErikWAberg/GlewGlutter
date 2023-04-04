#include <Group.h>
#include <Transform.h>
#include <LightAnimator.h>
#include <Scene.h>
#include <GLBootstrap.h>
#include <BouncySquashAnimator.h>
#include "LightAnimatorStatic.h"
#include <Keyboard.h>
#include <cmath>
#include <Skybox.h>
#include <GBuffer.h>
#include <GLFW/glfw3.h>

int winx = 900, winy = 900;
LightAnimatorStatic *lightAnimator;
vr::Vec2 screenSize = vr::Vec2(winx, winy);

ShaderProgram *skyBoxShader;
ShaderProgram *shadowShader;
ShaderProgram *gBufferShader;
ShaderProgram *lightShader;
ShaderProgram *postShader;

Skybox* skybox;
Shadow* shadowGroup;
GBuffer *gBuffer;
RenderToTexture *renderToTexture;

Geometry* sphereGeometry, *squareGeometry;

bool postEffects = false, showBuffers = true;

float tReshapeStart = -1;
int lightCount = 0;
GLFWwindow* window;

Geometry* createSquareGeom() {
    vr::Vec3Vector squareVertices;
    squareVertices.push_back(vr::Vec3(-1, -1, 0));
    squareVertices.push_back(vr::Vec3(-1, 1, 0));
    squareVertices.push_back(vr::Vec3(1, 1, 0));
    squareVertices.push_back(vr::Vec3(1, -1, 0));

    vr::Vec3Vector squareNormals;
    squareNormals.push_back(vr::Vec3(0, 0, -1));
    squareNormals.push_back(vr::Vec3(0, 0, -1));
    squareNormals.push_back(vr::Vec3(0, 0, -1));
    squareNormals.push_back(vr::Vec3(0, 0, -1));

    vr::UIntVector squareIndices;

    squareIndices.push_back(3);
    squareIndices.push_back(2);
    squareIndices.push_back(0);
    squareIndices.push_back(2);
    squareIndices.push_back(1);
    squareIndices.push_back(0);

    vr::Vec2Vector texCoords;
    texCoords.push_back(vr::Vec2(0,0));
    texCoords.push_back(vr::Vec2(1,0));
    texCoords.push_back(vr::Vec2(1,1));
    texCoords.push_back(vr::Vec2(0,1));

    vr::material_t squareMat;
    squareMat.ambient = vr::Vec3(0.8, 0.4, 0.1);
    squareMat.specular = vr::Vec3(0.7, 0.7, 0.7);
    squareMat.diffuse = vr::Vec3(0.4, 0.4, 0.4);
    squareMat.shininess = 20;

    Geometry *square2d = new Geometry();
    square2d->setVertices(squareVertices);
    square2d->setNormals(squareNormals);
    square2d->setIndices(squareIndices);
    square2d->setMaterial(squareMat);
    square2d->setTexCoords(texCoords);
    square2d->createGLBuffers();

    return square2d;
}

Transform* createPointLightBall(const vr::Vec4& pos, const vr::Vec3& color) {

    vr::Vec3 light_ambient = vr::Vec3(0.3, 0.3, 0.3);
    vr::Vec3 light_diffuse = color; 
    Light *light = new Light(pos, light_ambient, light_diffuse, color);
    Scene::instance().getRoot()->attachLight(light);
    Transform *sphereLightGroup = new Transform();
    sphereLightGroup->addChild(sphereGeometry);

    TransformUpdate* sphereTransformer = new TransformUpdate(vr::Matrix(), sphereLightGroup);
   
    LightAnimator* lightSphere = new LightAnimator(light, sphereTransformer);
    sphereLightGroup->Node::setUpdateCallback(lightSphere);
    std::ostringstream ss;
    ss << "lightSphere" << lightCount;

    Group* asd = Scene::instance().getGroupByName("transformShapes");

    Scene::instance().addGroupToParent(ss.str(), sphereLightGroup, asd);
    lightCount++;
    return sphereLightGroup;
}


void idle(void) {
    updateKeyPress();
    //glutPostRedisplay();
}


void doMovement(int lightIndex, vr::Vec3 movement) {
    if(movement.length2() > 0) {
        if(Scene::instance().getFPS() > 0) {
            movement += movement * 60.0/Scene::instance().getFPS();    
        }
        
        std::vector<Light *> lights = Scene::instance().getRoot()->State::getLights();
        if (lightIndex >= 0 && lights.size() > lightIndex) {
            for(; lightIndex < lights.size(); lightIndex++) {
                if(lights[lightIndex]->getPosition().w() != 0.0) {
                    Scene::instance().getRoot()->State::moveLight(size_t(lightIndex), movement);
                    break;
                }
            }
        } else {            
            Scene::instance().getMainCamera()->Camera::move(movement);
        }
    }
}
/*

void mouse(int button, int dir, int x, int y) {
    if (button == 0) {
        if (dir == 0) {
            glfwDisable(GLFW_MOUSE_CURSOR);
        } else {          
            glfwSetMousePos(winx / 2, winy / 2);
            glfwEnable(GLFW_MOUSE_CURSOR);
        }
    }
}

double mouseLastTime = 0;

void mouseMotion(int x, int y) {
    double current = glfwGetTime();

    if (float(current - mouseLastTime) < 2) {
        return;
    } else if (float(current - mouseLastTime) > 50) {
        glfwGetWindowSize(window, &winx, &winy);
        glfwSetMousePos(winx / 2, winy / 2);
        mouseLastTime = current;
        return;
    }

    mouseLastTime = current;
    glfwGetWindowSize(window, &winx, &winy);
    float mouseSpeed = 0.1f;
    double horizontalAngle = mouseSpeed * float(winx / 2 - x) * M_PI / 180 * -1;
    double verticalAngle = mouseSpeed * float(winy / 2 - y) * M_PI / 180;
    
    glfwSetMousePos(winx / 2, winy / 2);

    Scene::instance().getMainCamera()->Camera::rotate((float) (horizontalAngle * -1), (float) (verticalAngle * -1));
}*/
void updateWindowDimensions(void) {
    gBuffer->~GBuffer();
    gBuffer = new GBuffer(winx, winy);
    if(!gBuffer->generateBuffers()) {
        std::cerr << "Failed to generate GBuffer, quitting." << std::endl;
        exit(0);
    }
    shadowGroup->updateDimensions(winx, winy);
    screenSize = vr::Vec2(winx, winy);
    glViewport(0, 0, winx, winy);
}

void renderDefault(void) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);  

    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox->render();
    Scene::instance().render();
}


void renderGeometry(void) {
    Scene::instance().getRoot()->Group::attachShader(gBufferShader);    

    gBuffer->bindForGeomPass();
   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDepthMask(GL_TRUE);

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    glDisable(GL_CULL_FACE); 
    Scene::instance().render();  
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    glFrontFace(GL_CCW);

}

void drawLightBall(Light* light) {
    vr::Matrix scale;
    vr::Matrix translation;
    Scene::instance().getMainCamera()->attachMatricies();
    float lightSphereScale = light->calcPointLightSphere();
    scale.makeScale(lightSphereScale, lightSphereScale, lightSphereScale);
    translation.makeTranslate(light->getPositionVec3());
    translation.preMult(scale);
    sphereGeometry->State::transform(translation);
    sphereGeometry->State::applyModel();
    sphereGeometry->draw();
    sphereGeometry->State::resetModel();
}

void stencilPass(Light* light) {    
    glEnable(GL_STENCIL_TEST);
    gBuffer->bindForStencilPass();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 0, 0);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    
    drawLightBall(light);

}

void pointLightPass(Light* light) {
    gBuffer->bindForLightPass();

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
            
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    drawLightBall(light);

    glCullFace(GL_BACK);
    glDisable(GL_BLEND);

}


void renderLight(void) {
    Scene::instance().getRoot()->Group::attachShader(lightShader);
    lightShader->useProgram();
    Scene::instance().getMainCamera()->applyEyePos();
    
    gBuffer->loadSamples(lightShader);  

    vr::Matrix scale;
    vr::Matrix translation;
    std::vector<Light *> lights = Scene::instance().getRoot()->State::getLights();
    for (size_t i = 0; i < lights.size(); i++) {
        lights[i]->apply(lightShader, i);
        GLint ScreenSize = lightShader->getUniform("ScreenSize");
        glUniform2fv(ScreenSize, 1, screenSize.ptr());

        if(lights[i]->getPosition().w() != 0.0) {
            if(lights[i]->isEnabled()) {
                stencilPass(lights[i]);
                pointLightPass(lights[i]);
            }
        } else  {

            glDisable(GL_STENCIL_TEST);
            gBuffer->bindForLightPass();
            glDisable(GL_CULL_FACE);

            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
   
            
            vr::Matrix identity;
            GLint view = lightShader->getUniform("view");
            GLint proj = lightShader->getUniform("proj");
            glUniformMatrix4fv(view, 1, GL_FALSE, &identity.ptr()[0]);
            glUniformMatrix4fv(proj, 1, GL_FALSE, &identity.ptr()[0]);

            squareGeometry->applyModel();
            squareGeometry->draw();
            squareGeometry->resetModel();
            glDisable(GL_BLEND);    
        }
        
    }
   
}
  

float sajn = 0.0;
float asdd=0.01;
void renderPostEffect(void) {    
    postShader->useProgram();
    gBuffer->loadFinalSample(postShader, true);

    glDisable(GL_BLEND);

    GLint ScreenSize = postShader->getUniform("ScreenSize");
    glUniform2fv(ScreenSize, 1, screenSize.ptr());
    
    
    if(sajn >= 2*3.1) asdd = -0.01;
    if(sajn <= 0.0) asdd = 0.01;
    sajn += asdd;
    float f =  (1 + sinf(sajn) * 0.5);

    GLint Sajn = postShader->getUniform("Sine");
    glUniform1f(Sajn, sajn);

    squareGeometry->setShaderProgram(postShader);
    squareGeometry->draw();
    
}

void renderString(float x, float y, const unsigned char *string)
{
    /*glUseProgram(0);
    glColor3f( 1.0, 1.0, 1.0);
    glWindowPos2f(x, winy-y);
    glutBitmapString(GLUT_BITMAP_HELVETICA_10, string);*/
}

void display(void) {
    while(!glfwWindowShouldClose(window)) {
        idle();
        if(tReshapeStart != 0 && tReshapeStart != -1) {
            if(glfwGetTime() - tReshapeStart > 300) {
                tReshapeStart = 0;
                updateWindowDimensions();
            } else {
                return;
            }
        }

        gBuffer->startFrame();
        
        renderGeometry();
        

        renderLight();

        gBuffer->loadFinalSample(skyBoxShader, false);

        glDisable(GL_BLEND);
        skybox->render();


        if(postEffects) {
            renderPostEffect();  
        }

       

        if(showBuffers) {
            gBuffer->drawBuffers(winx / 80, 8.0f);
        }
        gBuffer->bindForFinalPass();

        glBlitFramebuffer(0, 0, winx, winy,
                        0, 0, winx, winy,                    
                        GL_COLOR_BUFFER_BIT, GL_LINEAR);

        Scene::instance().drawFPS();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void reshapeWindow(int w, int h) {   
    if(tReshapeStart != -1) {
        if(h > w) w = h;
        winx = w;
        winy = w;
        tReshapeStart = glfwGetTime();    
    } else {
        tReshapeStart = 0;
    }
    
}


void toggleEnableShadows() {
    bool isEnabled = Scene::instance().isShadowDisabled();
    Scene::instance().setShadowDisabled(!isEnabled);
}

void toggleDrawFPS() {
    Scene::instance().toggleShouldDrawFPS();
}

void togglePostEffects() {
    postEffects = !postEffects;
}

void toggleShowBuffers() {
    showBuffers = !showBuffers;
}

void toggleLight(int lightIndex) {
    std::vector<Light *> lights = Scene::instance().getRoot()->State::getLights();
    if (lightIndex >= 0 && lights.size() > lightIndex) {
        lights[lightIndex]->toggleEnabled();
        if(lightIndex == 0 || lightIndex == 1) {
            toggleEnableShadows();
        }
    }
}



void printHelp() {
    std::cout << std::endl << std::endl << "The Glew Glutter- Keyboard bindings:" << std::endl << std::endl;
    std::cout << "---  Move cam:            ---" << std::endl << "Press 'w | a | s | d | z | x'" << std::endl;
    std::cout << "---  Rotate cam:          ---" << std::endl << "click and drag mouse" << std::endl;
    std::cout << "---  Move light source:   ---" << std::endl << "Press 'l'-> move cam keys " << std::endl;
    std::cout << "---  Select light source: ---" << std::endl << "Press 1-9 " << std::endl;
    std::cout << "---  Toggle light source: ---" << std::endl << "Press 'u' " << std::endl;
    std::cout << "---  Toggle GBuffers:     ---" << std::endl << "Press 'i' " << std::endl;
    std::cout << "---  Toggle shadows:      ---" << std::endl << "Press 'o'" << std::endl;
    std::cout << "---  Toggle post process: ---" << std::endl << "Press 'p'" << std::endl;
    std::cout << "---  Toggle FPS:          ---" << std::endl << "Press 'f'" << std::endl;
    std::cout << "---  QUIT:                ---" << std::endl << "Press 'q | Q'" << std::endl;
}


int main(int argc, char **argv) {
    std::string path;
    if(argc == 2) {
         path = argv[1];
    } 
    if(path.length() == 0) {
        path = "/home/c11/c11ean/edu/5dv051/lab3/";
    }

    gl_bootstrap(argc, argv, winx, winy);
    setupKeys();
   
    float tStart = glfwGetTime();

    int NUM_LIGHT = 1;
    int NUM_TEX = 1;

    std::ostringstream ss;
    ss << "#define NUM_LIGHT " << NUM_LIGHT << "\n";
    ss << "#define NUM_TEX " << NUM_TEX << "\n";

    shadowShader = Shader::instance().createShaderProgram("shadow", path + "shaders/shadow.vs", path + "shaders/shadow.fs", ss.str());
    skyBoxShader = Shader::instance().createShaderProgram("skybox", path + "shaders/skybox.vs", path + "shaders/skybox.fs", "");
    gBufferShader = Shader::instance().createShaderProgram("gbuffer", path + "shaders/gbuffer.vs", path + "shaders/gbuffer.fs", ss.str());
    lightShader = Shader::instance().createShaderProgram("light", path + "shaders/light.vs", path + "shaders/light.fs", "");
    postShader = Shader::instance().createShaderProgram("post", path + "shaders/post.vs", path + "shaders/post.fs", "");


    gBuffer = new GBuffer(winx, winy);
    if(!gBuffer->generateBuffers()) {
        std::cerr << "Failed to generate GBuffer, quitting." << std::endl;
        return 0;
    }

    renderToTexture = new RenderToTexture(winx, winy, false);
   
    // --------- LIGHTS ------------
    vr::Vec4 light_position1 = vr::Vec4(1.0, 0.2, 1, 1.0);
    vr::Vec3 light_ambient1 = vr::Vec3(0.4, 0.1, 0.1);
    vr::Vec3 light_diffuse1 = vr::Vec3(0.4, 0.1, 0.1);
    vr::Vec3 light_specular1 = vr::Vec3(0.8, 0.2, 0.2);

    Light *light1 = new Light(light_position1, light_ambient1, light_diffuse1, light_specular1);    

    Scene::instance().createSceneRoot(gBufferShader);
    Scene::instance().getRoot()->attachLight(light1);
    
    Scene::instance().getMainCamera()->Camera::move(1, -4.0f, -10);
    Scene::instance().getMainCamera()->Camera::rotate(-0.0f, 0.3);
    
    shadowGroup = new Shadow(winx, winy, shadowShader, light1);

    Scene::instance().addGroupToParent("shadow", shadowGroup, Scene::instance().getMainCamera());


    // --------- END: LIGHTS ------------

    // --------- GEOMETRIES SETUP -------

    Transform *shapes = new Transform();
    shapes->setGroupName("SceneGeoms");
    Scene::instance().addGroupToParent("transformShapes", shapes, shadowGroup);


    // ---------- 1 BOX -> 2 DISPLAYED BOXES -----
    //----------- WITH MULTI-TEXTURE -------------

    Node *box = Scene::instance().createGeometry(path + "models/box.obj");

    box->State::addTexture((path + "models/box.bmp").c_str());
    vr::Matrix trans = vr::Matrix();

    Transform *metaBox1 = new Transform();
    metaBox1->setGroupName("TexturedBox1");
    metaBox1->addChild(box);
    Transform *metaBox2 = new Transform();
    metaBox2->setGroupName("TexturedBox2");
    metaBox2->addChild(box);

    Scene::instance().addGroupToParent("box1", metaBox1, shapes);
    Scene::instance().addGroupToParent("box2", metaBox2, shapes);

    trans.makeTranslate(2, 0.7, 0);
    TransformUpdate *metaBox1Transformer = new TransformUpdate(trans, metaBox1);
    metaBox1->Node::setUpdateCallback(metaBox1Transformer);

    trans.makeTranslate(2, -2.3f, 0);
    TransformUpdate *metaBox2Transformer = new TransformUpdate(trans, metaBox2);
    metaBox2->Node::setUpdateCallback(metaBox2Transformer);

    // ---------- END: BOX -> 2 DISPLAYED BOXES -----

    // ---------- 1 MOVING SPHERE + LightSphere -------------------
    Node *sphere = Scene::instance().createGeometry(path + "models/sphere.obj");
    sphereGeometry = dynamic_cast<Geometry *>(sphere);

    Transform *sphereGroup = new Transform();
    sphereGroup->setGroupName("BouncySphere");
    sphereGroup->addChild(sphere);

    BouncySquashAnimator *simpleSphereAnimator = new BouncySquashAnimator(sphere);
    sphereGroup->Node::setUpdateCallback(simpleSphereAnimator);

    Transform *sphereLightGroup = new Transform();
    sphereLightGroup->setGroupName("LightSphere");
    sphereLightGroup->addChild(sphere);

    TransformUpdate* sphereTransformer = new TransformUpdate(vr::Matrix(), sphereLightGroup);
   
    LightAnimator* lightSphere = new LightAnimator(light1, sphereTransformer);
    sphereLightGroup->Node::setUpdateCallback(lightSphere);



    Scene::instance().addGroupToParent("sphere", sphereGroup, shapes);
    Scene::instance().addGroupToParent("lightSphere", sphereLightGroup, shapes);

    // --------- END: 1 MOVING SPHERE ---------------

    // --------- DOWNSIZED C3P0 ROBOT ---------------

    Node *robot = Scene::instance().createGeometry(path + "models/C3P0.obj");
    shapes->addChild(robot);

    trans.makeScale(0.003, 0.003, 0.003);
    TransformUpdate *robotTranslater = new TransformUpdate(trans, robot);
    robot->Node::setUpdateCallback(robotTranslater);

    // --------- END:  DOWNSIZED C3P0 ROBOT ---------

    // --------- CUSTOM MADE SQUARE (FLOOR) ---------

    squareGeometry = createSquareGeom();

    Transform* floorTransform = new Transform();
    floorTransform->setGroupName("GroundPlane");
    floorTransform->addChild(squareGeometry);

    vr::Matrix scale;
    trans.makeTranslate(-50, -2.73, 0);

    scale.makeScale(100, 0, 100);

    trans.preMult(scale);
    vr::Vec3 axis(1,0,0);
    scale.makeRotate(3.1415/2.0,axis );
    trans.mult(scale, trans);
    TransformUpdate *squareUpdater = new TransformUpdate(trans, floorTransform);
    floorTransform->Node::setUpdateCallback(squareUpdater); //Make bigger and move

    shapes->addChild(floorTransform);

    // -------END: CUSTOM MADE SQUARE (FLOOR) ---------


    Node *sandtroop = Scene::instance().createGeometry(path + "models/Sandtrooper/Sandtrooper.obj");
    shapes->addChild(sandtroop);
    scale.makeTranslate(-3.1,-2.7,7);
    trans.makeScale(0.007, 0.007, 0.007);
    trans.mult(trans, scale );
    TransformUpdate *asddd = new TransformUpdate(trans, sandtroop);
    sandtroop->Node::setUpdateCallback(asddd);


    Node *jabba = Scene::instance().createGeometry(path + "models/jabba/Jabba.obj");
    shapes->addChild(jabba);
    scale.makeTranslate(-3.1,-2.7,10);
    trans.makeScale(0.007, 0.007, 0.007);
    trans.mult(trans, scale );
    TransformUpdate *jabbatrans = new TransformUpdate(trans, jabba);
    jabba->Node::setUpdateCallback(jabbatrans);

    // --------- END: GEOMETRIES SETUP --------------

     
    skybox = new Skybox(skyBoxShader, sphereGeometry);

    bool skyBoxCreated =
        skybox->init(path + "models/sky_pos_x.jpg",
                path + "models/sky_neg_x.jpg",
                path + "models/sky_pos_y.jpg",
                path + "models/sky_neg_y.jpg",
                path + "models/sky_pos_z.jpg",
                path + "models/sky_neg_z.jpg");


    if(!skyBoxCreated) {
        std::cerr << "Failed to load skybox images" << std::endl;
        return 0;

    }

    Scene::instance().getRoot()->Group::attachShader(gBufferShader);

    std::cout << std::endl<< std::endl;
    Scene::instance().printScene();

    std::cout << "Geometries load time: " << glfwGetTime() - tStart << "ms" << std::endl;

    printHelp();
    createPointLightBall(vr::Vec4(1, 2, 1, 1.0), vr::Vec3(0.0, 0.0, 1.0));    
    createPointLightBall(vr::Vec4(0, -1.2, 2, 1.0), vr::Vec3(0.0, 1.0, 1.0));    
    createPointLightBall(vr::Vec4(-1, 2.2, 1, 1.0), vr::Vec3(0.0, 1.0, 0.0));    
    createPointLightBall(vr::Vec4(-2, 2.2, 1.5, 1.0), vr::Vec3(1.0, 1.0, 1.0));
    
    createPointLightBall(vr::Vec4(-3, 0.2, 2, 1.0), vr::Vec3(1.0, 0.5, 0.0));

    createPointLightBall(vr::Vec4(0.5, 2.8, -6, 1.0), vr::Vec3(1.0, 0.0, 1.0));
    createPointLightBall(vr::Vec4(-2, 0.8, -6, 1.0), vr::Vec3(1.0, 0.0, 1.0));



    vr::Vec4 light_position2 = vr::Vec4(0, 1, -20, 0.0);
    vr::Vec3 light_ambient2 = vr::Vec3(0.15, 0.15, 0.15);
    vr::Vec3 light_diffuse2 = vr::Vec3(0.1, 0.1, 0.1);
    vr::Vec3 light_specular2 = vr::Vec3(0.02, 0.02, 0.02);
    Light *light2 = new Light(light_position2, light_ambient2, light_diffuse2, light_specular2);
    Scene::instance().getRoot()->attachLight(light2);
    std::cout << window << std::endl;
    if(window == NULL) std::cout << "NULL" << std::endl;
    //toggleDrawFPS();
    //glutMainLoop();
    display();

    return 0;
}

