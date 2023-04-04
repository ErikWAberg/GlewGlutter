#ifndef _UPDATE_CALLBACK_H
#define _UPDATE_CALLBACK_H

class UpdateCallback {

public:
    UpdateCallback(void) : m_enabled(true) {
    }

    ~UpdateCallback(void) {
    }

    void toggleOnOff() {
        m_enabled = !m_enabled;
    }

    virtual void update() = 0;

protected:
    bool m_enabled;
};

#endif

