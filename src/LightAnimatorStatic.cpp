#include "LightAnimatorStatic.h"
#include <iostream>
#include <GL/glew.h>

void LightAnimatorStatic::update() {
    if (!m_enabled) return;
    vr::Vec3 movement(0, (float) (varying * 0.05), 0);
    m_group->State::moveLight(1, movement);
    m_count += varying;
    if (m_count == 0) {
        varying = 1;
        m_iterations++;
        if (m_iterations >= 4) {
            m_group->enablePolygonMode(true, GL_POINT);

        }
    }
    else if (m_count == 20) {
        varying = -1;
        m_iterations++;
        if (m_iterations >= 5) {
            m_group->enablePolygonMode(true, GL_LINE);

        }
        if (m_iterations >= 7) {
            m_iterations = 0;
            m_group->enablePolygonMode(false, GL_LINE);
        }
    }
}