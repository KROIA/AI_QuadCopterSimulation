#pragma once
#include "QSFML_EditorWidget.h"
#include "Motor2D.h"
#include "QuadCopter2D.h"


class QuadCopter2DKeyController: public QObject, public QSFML::Objects::CanvasObject
{
        Q_OBJECT
    public:
        QuadCopter2DKeyController(const std::string &name = "QuadCopter2DKeyController",
                                  CanvasObject *parent = nullptr);
        QuadCopter2DKeyController(const QuadCopter2DKeyController &other);
        ~QuadCopter2DKeyController();
        CLONE_FUNC(QuadCopter2DKeyController);

    private slots:
        void onLeftKeyFalling();
        void onRightKeyFalling();
        void onLeftKeyRising();
        void onRightKeyRising();

        void onTorqueRKeyPressed();
        void onTorqueLKeyPressed();
        void onForceUpKeyPressed();
        void onForceLeftKeyPressed();
        void onForceDownKeyPressed();
        void onForceRightKeyPressed();

       // void update() override;

    private:
        QSFML::Components::KeyPressEvent *m_leftKey;
        QSFML::Components::KeyPressEvent *m_rightKey;

        QSFML::Components::KeyPressEvent *m_torqueRightKey;
        QSFML::Components::KeyPressEvent *m_torqueLeftKey;
        QSFML::Components::KeyPressEvent *m_forceUpKey;
        QSFML::Components::KeyPressEvent *m_forceDownKey;
        QSFML::Components::KeyPressEvent *m_forceLeftKey;
        QSFML::Components::KeyPressEvent *m_forceRightKey;

};

#define KC_ALL_QUAD(func) \
std::vector<QuadCopter2D*> copters = QuadCopter2D::getInstances(); \
for(size_t i=0; i<copters.size(); ++i)\
{\
    copters[i]->func();\
}\
