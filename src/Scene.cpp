#include <vr/FileSystem.h>
#include <Scene.h>
#include <GeometryLoader.h>
#include <iomanip>
#include <GlewGlutter.h>

void Scene::render() {
    m_rootGroup->accept(m_updateVisitor);
    m_rootGroup->accept(m_renderVisitor);
}


void Scene::renderGroup(std::string groupName) {
    Group *groupNode = m_strMappedGroups[groupName];
    if (groupNode == NULL) {
        std::cerr << "No group with name " << groupName << " found!" << std::endl;
        return;
    }
    groupNode->accept(m_updateVisitor);
    groupNode->accept(m_renderVisitor);
}

int Scene::createSceneRoot(ShaderProgram *defaultShader) {
    m_defaultShader = defaultShader;

    if (m_rootGroup != NULL) {
        std::cerr << "Scene already exists!" << std::endl;
        return 1;
    }
    m_rootGroup = new Group();
    m_camera = new Camera();
    m_rootGroup->addChild(m_camera);
    m_rootGroup->attachShader(m_defaultShader);
    m_strMappedGroups["camera"] = m_camera;
    m_strMappedGroups["root"] = m_rootGroup;
    return 0;
}

Group *Scene::getRoot() {
    return m_rootGroup;
}

Camera *Scene::getMainCamera() {
    return m_camera;
}


Group *Scene::getGroupByName(std::string groupName) {
    if (m_strMappedGroups[groupName] == NULL) {
        std::cerr << "A group with that name does not exist!" << std::endl;
        return NULL;
    }
    return m_strMappedGroups[groupName];
}


int Scene::addGroupToParent(std::string groupName, Group *group, Group *parentGroup) {
    if (m_strMappedGroups[groupName] != NULL) {
        std::cerr << "A group with that name already exists!" << std::endl;
        return 1;
    }

    m_strMappedGroups[groupName] = group;
    parentGroup->addChild(group);

    return 0;
}

Node *Scene::createGeometry(std::string objPath) {

    std::string geomName =  objPath.substr(objPath.find_last_of("/") + 1);

    size_t num = m_loadedGeometries.size();

    size_t numRead = GeometryLoader::loadGeometry(objPath, m_loadedGeometries);

    if (numRead == 1) {
        m_loadedGeometries[num]->createGLBuffers();
        return &(*m_loadedGeometries[num]);
    }

    Transform *transform = new Transform();
    transform->Group::setGroupName(geomName);
    
    for (size_t i = 0; i < numRead; i++) {
        transform->addChild(&*m_loadedGeometries[num + i]);
        m_loadedGeometries[num + i]->createGLBuffers();
    }

    return transform;
}


void Scene::setShadowPassActive(bool isDoingShadowPass) {
    m_doingShadowPass = isDoingShadowPass;
}

bool Scene::isDoingShadowPass() {
    return m_doingShadowPass;
}

void Scene::printScene() {
    m_rootGroup->accept(m_graphPrintVisitor);
}

bool Scene::isShadowDisabled() {
    return m_isShadowDisabled;
}

void Scene::setShadowDisabled(bool disableShadows) {
    m_isShadowDisabled= disableShadows;
}

int prevFps= 0;
int frames = 0;
int fps = 0;
double frame_ms = 0;
double prevFrame_ms = 0;

void Scene::drawFPS() {
   
/*
    if (!m_fpsTimer.isRunning())
    {
        m_fpsTimer.start();
        return;
    }

    frames++;

    if(m_fpsTimer.getTime() < 1000 )  {
        fps = prevFps;
        frame_ms = prevFrame_ms;
    } else {
        m_fpsTimer.stop();
        double time_ms = m_fpsTimer.getTime();
        fps = frames / (time_ms/1000);
        frame_ms = time_ms/frames; 
        prevFrame_ms = frame_ms;
        prevFps = fps;
        m_fpsTimer.reset(true);
        frames = 0;
    }
    
     if (!m_shouldDrawFPS)
        return;
    std::ostringstream str;
    str << "FPS: " << std::setprecision(4) << fps << " T/frame: " << frame_ms << "ms" << std::ends;
    renderString(5.0f, 20.0f, (const unsigned char*)str.str().c_str());*/
}
float Scene::getFPS(void) {
    return prevFps;
}

//Todo return geometry vector
//Load a geometry and add it to a group
int Scene::createGroupByGeometry(std::string groupName, Group *group, Group *parentGroup, std::string objPath) {

    if (group == NULL) {
        std::cerr << "Unallocated group node!" << std::endl;
        return 1;
    }

    if (parentGroup == NULL) {
        std::cerr << "Unallocated parent group node!" << std::endl;
        return 2;
    }

    size_t num = m_loadedGeometries.size();

    size_t numRead = GeometryLoader::loadGeometry(objPath, m_loadedGeometries);

    for (size_t i = 0; i < numRead; i++) {
        group->addChild(&*m_loadedGeometries[num + i]);
        m_loadedGeometries[num + i]->createGLBuffers();
    }

    Scene::addGroupToParent(groupName, group, parentGroup);

    return 0;
}

//Load a geometry object and add it as a child to an existing group
GeometrySharedPtrVector Scene::createGeometryInGroup(std::string groupName, std::string objPath) {

    Group *group = Scene::getGroupByName(groupName);
    if (group == NULL) {
        std::cerr << "Unallocated group node!" << std::endl;
        return m_loadedGeometries;
    }

    size_t num = m_loadedGeometries.size();

    size_t numRead = GeometryLoader::loadGeometry(objPath, m_loadedGeometries);

    for (size_t i = 0; i < numRead; i++) {
        group->addChild(&*m_loadedGeometries[num + i]);
        m_loadedGeometries[num + i]->createGLBuffers();
    }

    return m_loadedGeometries;
}

void Scene::toggleShouldDrawFPS() {
    m_shouldDrawFPS = !m_shouldDrawFPS;
}
