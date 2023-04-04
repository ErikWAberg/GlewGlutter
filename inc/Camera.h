#ifndef _CAMERA_H
#define _CAMERA_H

#include <vr/Matrix.h>
#include <vr/Vec3.h>
#include <Group.h>
#include <Node.h>
#include <assimp/vector3.h>

class Camera : public Group {

public:
    Camera();
    Camera(int wWidth, int wHeight, vr::Vec3 pos, vr::Vec3 eye, vr::Vec3 up);
    Camera(int wWidth, int wHeight, float fov, float near, float far, vr::Vec3 pos, vr::Vec3 eye, vr::Vec3 up);
     
    ~Camera();

    //Node
    void accept(NodeVisitor &nodeVisitor);

    //Camera
    void move(float dx, float dy, float dz);

    void rotate(float hAngle, float vAngle);

    void move(vr::Vec3 movement);

    void attachMatricies();


    void enableOrthographic();

    void enablePerspective();

    void enableFishEye();

    void disableFishEye();

    void invert();

    void changeLookAt(vr::Vec3 eye, vr::Vec3 center, vr::Vec3 up);

    void setDirection(vr::Vec3 center);

    void setPosition(vr::Vec3 eye);

    void setUp(vr::Vec3 up);
    aiVector3t
    Vec3 applyEyePos(void);
    
    vr::Matrix getView();

private:

    
    vr::Matrix m_view;
    vr::Matrix m_projection;
    
    vr::Vec3 m_eye;
    vr::Vec3 m_center;
    vr::Vec3 m_up;

    int m_windowWidth;
    int m_windowHeight;

    double m_fov;
    double m_aspect;

    double m_left;
    double m_right;
    double m_top;
    double m_bottom;
    double m_znear;
    double m_zfar;

};

#endif