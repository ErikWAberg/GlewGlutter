#include <LightAnimator.h>


void LightAnimator::update() {
    m_transMatrix.makeTranslate(m_light->getPositionVec3());
    vr::Matrix scale;
    scale.makeScale(0.2, 0.2, 0.2);
    m_transMatrix.scale(5.2, 0.2, 0.2);
    m_transMatrix.preMult(scale);
    m_transformUpdater->updateTransform(m_transMatrix);
    m_transformUpdater->update();
}