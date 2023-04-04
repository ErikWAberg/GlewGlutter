#ifndef _SCENE_H
#define _SCENE_H

#include <Geometry.h>
#include <Group.h>
#include <RenderVisitor.h>
#include <UpdateVisitor.h>
#include <GraphPrintVisitor.h>
#include <Shader.h>
#include <Camera.h>
#include <Shadow.h>
#include <string>
#include <vector>
#include <map>
#include <vr/Timer.h>


class Scene {
    
public:
    static Scene &instance() {
        static Scene instance; // Guaranteed to be destroyed.Instantiated on first use.
        return instance;
    }
    
    int createSceneRoot(ShaderProgram *sharedShaderPtr);

    Group *getRoot();

    Group *getGroupByName(std::string groupName);

    int addGroupToParent(std::string groupName, Group *group, Group *parentGroup);

    Node *createGeometry(std::string objPath);

    int createGroupByGeometry(std::string groupName, Group *group, Group *parentGroup, std::string objPath);

    GeometrySharedPtrVector createGeometryInGroup(std::string groupName, std::string objPath);

    void render();

    void renderGroup(std::string groupName);


    void setShadowPassActive(bool isDoingShadowPass);
    
    bool isDoingShadowPass();

    bool isShadowDisabled();

    void setShadowDisabled(bool disableShadows);

    void printScene();

    Camera *getMainCamera();

    void toggleShouldDrawFPS();

    void drawFPS();

    float getFPS(void);


private:

    Scene() {};

    Scene(Scene const &);

    void operator=(Scene const &);
    Camera *m_camera;

    std::map<std::string, Group *> m_strMappedGroups;

    RenderVisitor m_renderVisitor;
    UpdateVisitor m_updateVisitor;
    GraphPrintVisitor m_graphPrintVisitor;

    GeometrySharedPtrVector m_loadedGeometries;

    ShaderProgram *m_defaultShader;

    Group *m_rootGroup;


    bool m_doingShadowPass;
    bool m_isShadowDisabled;

    bool m_shouldDrawFPS;

    vr::Timer m_fpsTimer;
};


#endif