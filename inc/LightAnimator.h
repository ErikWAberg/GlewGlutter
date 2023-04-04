#ifndef _LIGHT_ANIMATOR_H
#define _LIGHT_ANIMATOR_H

#include <vr/Matrix.h>
#include <UpdateCallback.h>
#include <Light.h>
#include <TransformUpdate.h>


class LightAnimator : public UpdateCallback {

public:
    LightAnimator(Light* light, TransformUpdate* transformUpdater)
            : UpdateCallback() {
        m_transMatrix = vr::Matrix();
        m_transformUpdater = transformUpdater;
        m_light = light;
    }

    ~LightAnimator() {
    }

    void update();

private:
    Light* m_light;
    TransformUpdate* m_transformUpdater;

    vr::Matrix m_transMatrix;

};

#endif