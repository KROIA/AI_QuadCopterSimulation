#include "QuadCopter2DKeyController.h"

QuadCopter2DKeyController::QuadCopter2DKeyController(const std::string &name,
             CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_leftKey = new QSFML::Components::KeyPressEvent("LeftKey",sf::Keyboard::Y);
    m_rightKey = new QSFML::Components::KeyPressEvent("RightKey",sf::Keyboard::C);
    m_torqueRightKey = new QSFML::Components::KeyPressEvent("TorqueR",sf::Keyboard::E);
    m_torqueLeftKey = new QSFML::Components::KeyPressEvent("TorqueL",sf::Keyboard::Q);
    m_forceUpKey = new QSFML::Components::KeyPressEvent("FUpKey",sf::Keyboard::W);
    m_forceDownKey = new QSFML::Components::KeyPressEvent("FDownKey",sf::Keyboard::S);
    m_forceLeftKey = new QSFML::Components::KeyPressEvent("FLeftKey",sf::Keyboard::A);
    m_forceRightKey = new QSFML::Components::KeyPressEvent("FRightKey",sf::Keyboard::D);
    connect(m_leftKey, &QSFML::Components::KeyPressEvent::fallingEdge, this, &QuadCopter2DKeyController::onLeftKeyFalling);
    connect(m_rightKey, &QSFML::Components::KeyPressEvent::fallingEdge, this, &QuadCopter2DKeyController::onRightKeyFalling);
    connect(m_leftKey, &QSFML::Components::KeyPressEvent::risingEdge, this, &QuadCopter2DKeyController::onLeftKeyRising);
    connect(m_rightKey, &QSFML::Components::KeyPressEvent::risingEdge, this, &QuadCopter2DKeyController::onRightKeyRising);
    connect(m_torqueRightKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2DKeyController::onTorqueRKeyPressed);
    connect(m_torqueLeftKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2DKeyController::onTorqueLKeyPressed);
    connect(m_forceUpKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2DKeyController::onForceUpKeyPressed);
    connect(m_forceDownKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2DKeyController::onForceDownKeyPressed);
    connect(m_forceLeftKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2DKeyController::onForceLeftKeyPressed);
    connect(m_forceRightKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2DKeyController::onForceRightKeyPressed);


    addComponent(m_leftKey);
    addComponent(m_rightKey);
    addComponent(m_torqueRightKey);
    addComponent(m_torqueLeftKey);
    addComponent(m_forceUpKey);
    addComponent(m_forceDownKey);
    addComponent(m_forceLeftKey);
    addComponent(m_forceRightKey);

}
QuadCopter2DKeyController::QuadCopter2DKeyController(const QuadCopter2DKeyController &other)
    : CanvasObject(other)
{

}
QuadCopter2DKeyController::~QuadCopter2DKeyController()
{

}


void QuadCopter2DKeyController::onLeftKeyFalling()
{
    KC_ALL_QUAD(onLeftKeyFalling);
}
void QuadCopter2DKeyController::onRightKeyFalling()
{
    KC_ALL_QUAD(onRightKeyFalling);
}
void QuadCopter2DKeyController::onLeftKeyRising()
{
    KC_ALL_QUAD(onLeftKeyRising);
}
void QuadCopter2DKeyController::onRightKeyRising()
{
    KC_ALL_QUAD(onRightKeyRising);
}
void QuadCopter2DKeyController::onTorqueRKeyPressed()
{
    KC_ALL_QUAD(onTorqueRKeyPressed);
}
void QuadCopter2DKeyController::onTorqueLKeyPressed()
{
    KC_ALL_QUAD(onTorqueLKeyPressed);
}
void QuadCopter2DKeyController::onForceUpKeyPressed()
{
    KC_ALL_QUAD(onForceUpKeyPressed);
}
void QuadCopter2DKeyController::onForceLeftKeyPressed()
{
    KC_ALL_QUAD(onForceLeftKeyPressed);
}
void QuadCopter2DKeyController::onForceDownKeyPressed()
{
    KC_ALL_QUAD(onForceDownKeyPressed);
}
void QuadCopter2DKeyController::onForceRightKeyPressed()
{
    KC_ALL_QUAD(onForceRightKeyPressed);
}
