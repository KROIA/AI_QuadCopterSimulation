#include "QuadCopter2D.h"
#include "SimulationSettings.h"



QuadCopter2D::QuadCopter2D(const std::string &name,
             CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_painter = new QuadCopter2DPainter();
    m_painter->m_copter = this;
    addComponent(m_painter);

    m_forceVec = new ForcePainter();
    //m_forceVec->setColor(sf::Color::Red);
    m_forceVec->setScale(10);
    addComponent(m_forceVec);

    m_accelerationVec = new ForcePainter();
    m_accelerationVec->setColor(sf::Color::Green);
    m_accelerationVec->setScale(100);
    addComponent(m_accelerationVec);
    m_velocityVec = new ForcePainter();
    m_velocityVec->setColor(sf::Color::Blue);
    m_velocityVec->setScale(10);
    addComponent(m_velocityVec);


    m_frame = new QuadCopterFrame2D("Frame");
    m_motorLeft = new Motor2D("MotorLeft");
    m_motorRight = new Motor2D("MotorRight");

    m_leftKey = new QSFML::Components::KeyPressEvent("LeftKey",sf::Keyboard::A);
    m_rightKey = new QSFML::Components::KeyPressEvent("RightKey",sf::Keyboard::D);
    m_torqueRightKey = new QSFML::Components::KeyPressEvent("TorqueR",sf::Keyboard::E);
    m_torqueLeftKey = new QSFML::Components::KeyPressEvent("TorqueL",sf::Keyboard::Q);
    connect(m_leftKey, &QSFML::Components::KeyPressEvent::fallingEdge, this, &QuadCopter2D::onLeftKeyFalling);
    connect(m_rightKey, &QSFML::Components::KeyPressEvent::fallingEdge, this, &QuadCopter2D::onRightKeyFalling);
    connect(m_leftKey, &QSFML::Components::KeyPressEvent::risingEdge, this, &QuadCopter2D::onLeftKeyRising);
    connect(m_rightKey, &QSFML::Components::KeyPressEvent::risingEdge, this, &QuadCopter2D::onRightKeyRising);
    connect(m_torqueRightKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2D::onTorqueRKeyPressed);
    connect(m_torqueLeftKey, &QSFML::Components::KeyPressEvent::down, this, &QuadCopter2D::onTorqueLKeyPressed);

    addChild(m_frame);
    addChild(m_motorLeft);
    addChild(m_motorRight);

    addComponent(m_leftKey);
    addComponent(m_rightKey);
    addComponent(m_torqueRightKey);
    addComponent(m_torqueLeftKey);
    m_pos.setForceVector(sf::Vector2f(200,250));
    m_pos.setTorque(0);
    m_groundHeight = 600;
    m_sealingHeight = 0;
    m_leftWallPos = 0;
    m_rightWallPos = 2000;
    m_paused = false;
    m_groundWasHit = false;
}
QuadCopter2D::QuadCopter2D(const QuadCopter2D &other)
    : CanvasObject(other)
{
    m_painter = new QuadCopter2DPainter();
    m_painter->m_copter = this;
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
    m_paused = false;
}
QuadCopter2D::~QuadCopter2D()
{

}

void QuadCopter2D::setPause(bool pause)
{
    m_paused = pause;
}
void QuadCopter2D::update()
{
    if(m_paused)
        return;
    sf::Vector2f pos(m_pos.getForceVector().x, m_pos.getForceVector().y);
    m_frame->setOrigin(pos);
    m_motorLeft->setOrigin(m_frame->getLeftMotorMount());
    m_motorRight->setOrigin(m_frame->getRightMotorMount());
    m_motorLeft->setForceDirection(m_frame->getUpDir());
    m_motorRight->setForceDirection(m_frame->getUpDir());

    Force f = getCenteredForce();
    Force gravity(m_frame->getTopCenterPoint(),sf::Vector2f(0,9.81), 0);

    f += gravity + m_noiseForce;
    f.setActingPoint(m_frame->getTopCenterPoint());


    //m_forceVec->setStart(m_frame->getTopCenterPoint());
    //m_forceVec->setDirection(getCenteredForce().getForceVector());

    m_acceleration = f * SimulationSettings::getDeltaT();
    m_velocity += m_acceleration * SimulationSettings::getDeltaT();
    m_pos += m_velocity * SimulationSettings::getDeltaT();

    checkCollision();
    m_frame->setRotation(m_pos.getTorque()*0.0001+M_PI_2);
    //qDebug() << QSFML::VectorMath::getAngle(m_frame->getUpDir());
    //qDebug() << m_noiseForce.getTorque();
    m_noiseForce.setForce(0);
    m_noiseForce.setTorque(0);

    m_forceVec->setForce(f);
    m_accelerationVec->setForce(m_acceleration);
    m_velocityVec->setForce(m_velocity);
}
void QuadCopter2D::checkCollision()
{
    m_groundWasHit = false;

    if(m_pos.getForceVector().y > m_groundHeight)
    {
        m_pos.setForceVector(sf::Vector2f(m_pos.getForceVector().x, m_groundHeight));
        if(m_velocity.getForceVector().y>0)
            m_velocity.setForceVector(sf::Vector2f(m_velocity.getForceVector().x, 0));
        m_velocity.setTorque(0);
        m_pos.setTorque(0);
        m_groundWasHit = true;
    }else if(m_pos.getForceVector().y < m_sealingHeight)
    {
        m_pos.setForceVector(sf::Vector2f(m_pos.getForceVector().x, m_sealingHeight));
        if(m_velocity.getForceVector().y<0)
            m_velocity.setForceVector(sf::Vector2f(m_velocity.getForceVector().x, 0));
        m_velocity.setTorque(0);
        m_pos.setTorque(0);
        m_groundWasHit = true;
    }

    if(m_pos.getForceVector().x > m_rightWallPos)
    {
        m_pos.setForceVector(sf::Vector2f(m_rightWallPos, m_pos.getForceVector().y));
        if(m_velocity.getForceVector().x>0)
            m_velocity.setForceVector(sf::Vector2f(0, m_velocity.getForceVector().y));
        m_velocity.setTorque(0);
        m_pos.setTorque(0);
        m_groundWasHit = true;
    }else if(m_pos.getForceVector().x < m_leftWallPos)
    {
        m_pos.setForceVector(sf::Vector2f(m_leftWallPos, m_pos.getForceVector().y));
        if(m_velocity.getForceVector().x<0)
            m_velocity.setForceVector(sf::Vector2f(0, m_velocity.getForceVector().y));
        m_velocity.setTorque(0);
        m_pos.setTorque(0);
        m_groundWasHit = true;
    }
}

bool QuadCopter2D::getGroundWasHit() const
{
    return m_groundWasHit;
}
float QuadCopter2D::getAngle()
{
    return QSFML::VectorMath::getAngle(m_frame->getUpDir());
}
sf::Vector2f QuadCopter2D::getUpVector()
{
    return m_frame->getUpDir();
}
Force QuadCopter2D::getAcceleration()
{
    return m_acceleration;
}
Force QuadCopter2D::getSpeed()
{
    return m_velocity;
}
float QuadCopter2D::getHeight()
{
    return m_groundHeight - m_pos.getForceVector().y;
}
sf::Vector2f QuadCopter2D::getPosition()
{
    return m_pos.getForceVector();
}
void QuadCopter2D::setPosition(const Force &pos)
{
    m_pos = pos;
}
void QuadCopter2D::setSpeed(const Force &speed)
{
    m_velocity = speed;
}
void QuadCopter2D::setMotorForce(float left, float right)
{
    if(left < 0)
        left = 0;
    if(right < 0)
        right = 0;
    m_motorLeft->setThrust(left);
    m_motorRight->setThrust(right);
}
void QuadCopter2D::setPosition(const sf::Vector2f &pos)
{
    m_pos.setForceVector(pos);
}
void QuadCopter2D::setAngle(float angle)
{
    m_pos.setTorque(angle);
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
void QuadCopter2D::onTorqueRKeyPressed()
{
    m_noiseForce.setTorque(300);
    m_noiseForce.setActingPoint(m_frame->getTopCenterPoint());
}
void QuadCopter2D::onTorqueLKeyPressed()
{
    m_noiseForce.setTorque(-300);
    m_noiseForce.setActingPoint(m_frame->getTopCenterPoint());
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
