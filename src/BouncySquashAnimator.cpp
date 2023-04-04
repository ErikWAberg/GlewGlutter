#include "BouncySquashAnimator.h"
#include <iostream>
#include <cmath>

#include <Scene.h>

const double pi = std::acos(-1);
bool pauseAnimation = false;
int squashDir = 1;
int squashCount = 1;
float yTrans, prevYtrans, squashFactor = 1;
double prevTime = 0;
vr::Matrix scale;
double pauseDueToShadowPass = false;


void BouncySquashAnimator::update() {
    if(Scene::instance().isDoingShadowPass()) {
        pauseDueToShadowPass = true;
        m_state->transform(m_transMatrix);
    } else {
        pauseDueToShadowPass = false;
    }
 

    if(!pauseDueToShadowPass) {
        if (!pauseAnimation) {
            yTrans = (float) (-1.3 + 2 * cos((float) (100 - m_count) / 63) * sin((float) (100 - m_count) / 63));
            m_count += varying;
            if (m_count == 1) {
                varying = 1;
                pauseAnimation = true;
                prevYtrans = yTrans;
            }
            else if (m_count == 100) {
                varying = -1;
                pauseAnimation = true;
                prevYtrans = yTrans;
            }
        } else {
            float ss = (float) squashCount / 17.0;
            squashFactor = (float) sin(pi / (2.0 + ss));

            squashCount += squashDir;
            yTrans = (float) (prevYtrans - (1.1 - squashFactor) / 2.0);
            if (squashCount == 34) {
                squashDir = -1;
            } else if (squashCount == 0) {
                squashDir = 1;
                pauseAnimation = false;
                yTrans = prevYtrans;
            }
        }
      
        scale.makeScale(1.0/squashFactor, squashFactor, 1.0/squashFactor);
        m_transMatrix.makeTranslate(2, yTrans, 0);
        m_transMatrix.mult(scale,m_transMatrix);
        m_state->transform(m_transMatrix);
    
    }

}