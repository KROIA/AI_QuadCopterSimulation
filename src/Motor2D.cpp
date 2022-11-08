#include "Motor2D.h"


Motor2D::Motor2D(const std::string &name,
             CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_painter = new Motor2DPainter();
    m_painter->m_copter = this;
    addComponent(m_painter);

    m_forceVec = new ForcePainter();
    addComponent(m_forceVec);

    setThrust(0);
    setForceDirection(sf::Vector2f(0,1));
    setColor(sf::Color(200,150,0));
    setOrigin(sf::Vector2f(0,0));
    m_width = 20;
    m_height = 30;
}
Motor2D::Motor2D(const Motor2D &other)
    : CanvasObject(other)
{
    m_painter = new Motor2DPainter();
    m_painter->m_copter = this;
    addComponent(m_painter);

    m_forceVec = new ForcePainter();
    addComponent(m_forceVec);

    m_thrust = other.m_thrust;
    m_forceDirection = other.m_forceDirection;
    m_color = other.m_color;
    m_width = other.m_width;
    m_height = other.m_height;
}
Motor2D::~Motor2D()
{

}

void Motor2D::update()
{


    //Body
    float angle = QSFML::VectorMath::getAngle(m_forceDirection);

    sf::Vector2f origin = m_origin;
    sf::Vector2f leftDir = QSFML::VectorMath::getRotated(QSFML::VectorMath::getUnitVector(), angle - M_PI_2);
    sf::Vector2f upDir = m_forceDirection;


    m_BBL = origin + leftDir * m_width/2.f;
    m_BTL = m_BBL + upDir * m_height;
    m_BBR = origin - leftDir * m_width/2.f;
    m_BTR = m_BBR + upDir * m_height;
    m_forceVec->setForce(getThrustVector());
}


void Motor2D::setThrust(float thrust)
{
    m_thrust = thrust;
}
float Motor2D::getThrust() const
{
    return m_thrust;
}
Force Motor2D::getThrustVector() const
{
    return Force(m_origin, m_thrust * m_forceDirection, 0);
}

void Motor2D::setForceDirection(const sf::Vector2f &dir)
{
    m_forceDirection = dir;

}
const sf::Vector2f &Motor2D::getForceDirection() const
{
    return m_forceDirection;
}

void Motor2D::setColor(const sf::Color &color)
{
    m_color = color;
}
const sf::Color &Motor2D::getColor() const
{
    return m_color;
}
void Motor2D::setOrigin(const sf::Vector2f &origin)
{
    m_origin = origin;
}
const sf::Vector2f &Motor2D::getOrigin() const
{
    return m_origin;
}
void Motor2D::setRotation(float rad)
{
    m_forceDirection = QSFML::VectorMath::getRotated(sf::Vector2f(0,1), rad);
    //m_forceDirection.y = -m_forceDirection.y;
}

Motor2D::Motor2DPainter::Motor2DPainter(const std::string &name)
    : Drawable(name)
{
    m_copter = nullptr;
}
Motor2D::Motor2DPainter::Motor2DPainter(const Motor2DPainter &other)
    : Drawable(other)
{
    m_copter = nullptr;
}
Motor2D::Motor2DPainter::~Motor2DPainter()
{
}

void Motor2D::Motor2DPainter::draw(sf::RenderTarget& target,
                                   sf::RenderStates states) const
{
    if(!m_copter)
        return;

    //Body
    sf::Vertex body[]
    {
        sf::Vertex(m_copter->m_BBL),
        sf::Vertex(m_copter->m_BTL),
        sf::Vertex(m_copter->m_BTR),
        sf::Vertex(m_copter->m_BBR),
        sf::Vertex(m_copter->m_BBL)
    };

    for(size_t i=0; i<5; ++i)
        body[i].color = m_copter->m_color;
    target.draw(body, 5, sf::LinesStrip);


}
