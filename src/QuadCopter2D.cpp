#include "QuadCopter2D.h"


QuadCopter2D::QuadCopter2D(const std::string &name,
             CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_painter = new QuadCopter2DPainter();
    m_painter->m_copter = this;
    m_deltaT = 0.1;
    addComponent(m_painter);

    m_forceVec = new ForcePainter();
    //m_forceVec->setColor(sf::Color::Red);
    addComponent(m_forceVec);


    m_frame = new QuadCopterFrame2D("Frame");
    m_motorLeft = new Motor2D("MotorLeft");
    m_motorRight = new Motor2D("MotorRight");

    m_leftKey = new QSFML::Components::KeyPressEvent("LeftKey",sf::Keyboard::A);
    m_rightKey = new QSFML::Components::KeyPressEvent("RightKey",sf::Keyboard::D);
    connect(m_leftKey, &QSFML::Components::KeyPressEvent::fallingEdge, this, &QuadCopter2D::onLeftKeyFalling);
    connect(m_rightKey, &QSFML::Components::KeyPressEvent::fallingEdge, this, &QuadCopter2D::onRightKeyFalling);
    connect(m_leftKey, &QSFML::Components::KeyPressEvent::risingEdge, this, &QuadCopter2D::onLeftKeyRising);
    connect(m_rightKey, &QSFML::Components::KeyPressEvent::risingEdge, this, &QuadCopter2D::onRightKeyRising);

    addChild(m_frame);
    addChild(m_motorLeft);
    addChild(m_motorRight);

    addComponent(m_leftKey);
    addComponent(m_rightKey);
    m_pos.setForceVector(sf::Vector2f(200,250));
    m_pos.setTorque(0);
}
QuadCopter2D::QuadCopter2D(const QuadCopter2D &other)
    : CanvasObject(other)
{
    m_painter = new QuadCopter2DPainter();
    m_painter->m_copter = this;
    m_deltaT = 0.1;
    addComponent(m_painter);

    m_forceVec = new ForcePainter();
    //m_forceVec->setColor(sf::Color::Red);
    addComponent(m_forceVec);

    m_frame = new QuadCopterFrame2D("Frame");
    m_motorLeft = new Motor2D("MotorLeft");
    m_motorRight = new Motor2D("MotorRight");

    addChild(m_frame);
    addChild(m_motorLeft);
    addChild(m_motorRight);
}
QuadCopter2D::~QuadCopter2D()
{

}

void QuadCopter2D::update()
{
    m_motorLeft->setOrigin(m_frame->getLeftMotorMount());
    m_motorRight->setOrigin(m_frame->getRightMotorMount());
    m_motorLeft->setForceDirection(m_frame->getUpDir());
    m_motorRight->setForceDirection(m_frame->getUpDir());

    Force f = getCenteredForce();
    Force gravity(m_frame->getOrigin(),sf::Vector2f(0,9.81), 0);
    f += gravity;
    m_forceVec->setForce(f);
    //m_forceVec->setStart(m_frame->getTopCenterPoint());
    //m_forceVec->setDirection(getCenteredForce().getForceVector());

    m_acceleration = f * m_deltaT;
    m_velocity += m_acceleration * m_deltaT;
    m_pos += m_velocity * m_deltaT;
    if(m_pos.getForceVector().y >= 600)
    {
        m_pos.setForceVector(sf::Vector2f(m_pos.getForceVector().x, 600));
        if(m_velocity.getForceVector().y>0)
            m_velocity.setForceVector(sf::Vector2f(m_velocity.getForceVector().x, 0));
        m_velocity.setTorque(0);
        m_pos.setTorque(0);
    }
    m_frame->setRotation(m_pos.getTorque()*0.0001+M_PI_2);
    sf::Vector2f pos(m_pos.getForceVector().x, m_pos.getForceVector().y);
    m_frame->setOrigin(pos);
}

void QuadCopter2D::onLeftKeyFalling()
{
    m_motorLeft->setThrust(10);
}
void QuadCopter2D::onRightKeyFalling()
{
    m_motorRight->setThrust(10);
}
void QuadCopter2D::onLeftKeyRising()
{
    m_motorLeft->setThrust(0);
}
void QuadCopter2D::onRightKeyRising()
{
    m_motorRight->setThrust(0);
}

Force QuadCopter2D::getCenteredForce()
{
    //float leftL = QSFML::VectorMath::getLength(m_motorLeft->getOrigin() - m_frame->getOrigin());
    //float rightL = QSFML::VectorMath::getLength(m_motorRight->getOrigin() - m_frame->getOrigin());

    //sf::Vector2f force = leftL * m_motorLeft->getThrustVector() +

    return Force::getSum({m_motorLeft->getThrustVector(),
                          m_motorRight->getThrustVector()},
                          m_frame->getTopCenterPoint());
}


QuadCopter2D::QuadCopter2DPainter::QuadCopter2DPainter(const std::string &name)
    : Drawable(name)
{
    m_copter = nullptr;
}
QuadCopter2D::QuadCopter2DPainter::QuadCopter2DPainter(const QuadCopter2DPainter &other)
    : Drawable(other)
{
    m_copter = nullptr;
}
QuadCopter2D::QuadCopter2DPainter::~QuadCopter2DPainter()
{

}

void QuadCopter2D::QuadCopter2DPainter::draw(sf::RenderTarget& target,
                                             sf::RenderStates states) const
{

}
