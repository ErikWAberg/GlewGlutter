#include <Camera.h>
#include <Scene.h>

Camera::Camera() : Group() {
    m_fov = 68.0; 
    m_aspect = 1.0;
    m_znear = 1;
    m_zfar = 200.0;

    m_left = -1;
    m_right = 1;
    m_bottom = -1;
    m_top = 1;

    m_eye.set(0.0, 2.0, 7.0);
    m_center.set(0.0, 1.0, 1.0);
    m_up.set(0.0, 1.0, 0.0);

    m_view.makeLookAt(m_eye, m_center, m_up);
    m_projection.makePerspective(m_fov, m_aspect, m_znear, m_zfar);

}

Camera::Camera(int wWidth, int wHeight, float fov, float near, float far, vr::Vec3 eye, vr::Vec3 center, vr::Vec3 up) : Group() {
    m_windowWidth = wWidth;
    m_windowHeight = wHeight;

    m_eye = eye;
    m_center = center;
    m_up = up;

    m_left = -1;
    m_right = 1;
    m_bottom = -1;
    m_top = 1;

    m_fov = fov;
    m_aspect = (double) m_windowWidth / m_windowHeight;
    m_znear = near;
    m_zfar = far;

    m_view.makeLookAt(m_eye, m_center, m_up);
    m_projection.makePerspective(m_fov, m_aspect, m_znear, m_zfar);  
    std::cout << m_view << "   " << m_eye << std::endl;

}


Camera::Camera(int wWidth, int wHeight, vr::Vec3 eye, vr::Vec3 center, vr::Vec3 up) : Group() {
    m_windowWidth = wWidth;
    m_windowHeight = wHeight;

    m_eye = eye;
    m_center = center;
    m_up = up;

    m_left = -1;
    m_right = 1;
    m_bottom = -1;
    m_top = 1;

    m_fov = 68.0;
    m_aspect = 1.0;//(double) m_windowWidth / m_windowHeight;
    m_znear = 0.01;
    m_zfar = 100.0;

    m_view.makeLookAt(m_eye, m_center, m_up);
    m_projection.makePerspective(m_fov, m_aspect, m_znear, m_zfar);  
}

Camera::~Camera() {

}

void Camera::enableOrthographic() {    
    m_projection.makeOrtho(m_left, m_right, m_bottom, m_top, m_znear, m_zfar);
}

void Camera::enablePerspective() {
    m_projection.makePerspective(m_fov, m_aspect, m_znear, m_zfar);
}
void Camera::enableFishEye() {
    m_projection.makePerspective(90.0, m_aspect, m_znear, m_zfar);
}

void Camera::disableFishEye() {    
    m_projection.makePerspective(m_fov, m_aspect, m_znear, m_zfar);
}

void Camera::changeLookAt(vr::Vec3 eye, vr::Vec3 center, vr::Vec3 up) {
    m_eye = eye;
    m_center = center;
    m_up = up;
    m_view.makeLookAt(m_eye, m_center, m_up);
}

vr::Vec3 Camera::applyEyePos(void) {
    //m_view.getLookAt(m_eye, m_center, m_up, m_zfar);
    GLint eyePos = State::m_shaderProgram->getUniform("EyePos");
    glUniform3fv(eyePos, 1, m_eye.ptr());
    return m_eye;
}

void Camera::setDirection(vr::Vec3 center) {
    m_center = center;
    m_view.makeLookAt(m_eye, m_center, m_up);
}

void Camera::setPosition(vr::Vec3 eye) {
    m_eye = eye;
    m_view.makeLookAt(m_eye, m_center, m_up);
}

void Camera::setUp(vr::Vec3 up) {
    m_up = up;
    m_view.makeLookAt(m_eye, m_center, m_up);
}

vr::Matrix Camera::getView() {
    return m_view;
}


void Camera::invert() {
    m_projection.invert(m_projection);
    m_view.invert(m_view);
}


void Camera::attachMatricies() {

    GLint view = State::m_shaderProgram->getUniform("view");
    glUniformMatrix4fv(view, 1, GL_FALSE, &m_view.ptr()[0]);

    GLint proj = State::m_shaderProgram->getUniform("proj");
    glUniformMatrix4fv(proj, 1, GL_FALSE, &m_projection.ptr()[0]);
/*
    GLint eyePos = State::m_shaderProgram->getUniform("EyePos");
    glUniform3fv(eyePos, 1, m_eye.ptr());*/

}

void Camera::accept(NodeVisitor &nodeVisitor) {
    nodeVisitor.visit(this);
}




void Camera::move(float dx, float dy, float dz) {
    vr::Vec4 t = vr::Vec4(dx * -1, dy * -1, dz * -1, 1);
    m_view.invert(m_view);

    vr::Vec4 p0M = m_view.preMult(t);
    vr::Vec3 p0 = vr::Vec3(p0M.x(), p0M.y(), p0M.z());
    t[2] = -1 - dz;
    vr::Vec4 pRefM = m_view.preMult(t);
    vr::Vec3 pRef = vr::Vec3(pRefM.x(), pRefM.y(), pRefM.z());
    vr::Matrix view = vr::Matrix();
    view.makeLookAt(p0, pRef, vr::Vec3(0, 1, 0));
    m_view = view;
    m_eye = m_eye - vr::Vec3(dx,dy,dz);
}

void Camera::rotate(float hAngle, float vAngle) {
    vr::Vec4 t = vr::Vec4(0.0, 0.0, -1.0, 1.0);
    vr::Matrix rotateY = vr::Matrix(cos(hAngle), 0.0, -sin(hAngle), 0.0,
            0.0, 1.0, 0.0, 0.0,
            sin(hAngle), 0.0, cos(hAngle), 0.0,
            0.0, 0.0, 0.0, 1.0);
    vr::Matrix rotateX = vr::Matrix(1.0, 0.0, 0.0, 0.0,
            0.0, cos(vAngle), -sin(vAngle), 0.0,
            0.0, sin(vAngle), cos(vAngle), 0.0,
            0.0, 0.0, 0.0, 1.0);
    m_view.invert(m_view);
    vr::Vec4 pY = rotateY.preMult(t);
    vr::Vec4 pX = rotateX.preMult(pY);

    vr::Vec4 pRefM = m_view.preMult(pX);
    vr::Vec4 p0M = m_view.preMult(vr::Vec4(0, 0, 0, 1));
    vr::Vec3 p0 = vr::Vec3(p0M.x(), p0M.y(), p0M.z());
    vr::Vec3 pRef = vr::Vec3(pRefM.x(), pRefM.y(), pRefM.z());
    vr::Matrix view = vr::Matrix();
    view.makeLookAt(p0, pRef, m_up);
    m_view = view;
}

void Camera::move(vr::Vec3 movement) {
    Camera::move(movement.x(), movement.y(), movement.z());
    //std::cout << "Cam: " << std::endl;
    //std::cout << m_view << std::endl;
}
