#include "QuadCopterFrame2D.h"


QuadCopterFrame2D::QuadCopterFrame2D(const std::string &name,
             CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_painter = new QuadCopterFrame2DPainter();
    m_painter->m_copter = this;
    addComponent(m_painter);



    m_up = sf::Vector2f(0,1);
    m_origin = sf::Vector2f(0,0);
    m_width = 100;
    m_height = 10;

    m_motorCenterDistance = (m_width/2.f) - 20;
}
QuadCopterFrame2D::QuadCopterFrame2D(const QuadCopterFrame2D &other)
    : CanvasObject(other)
{
    m_painter = new QuadCopterFrame2DPainter();
    m_painter->m_copter = this;
    addComponent(m_painter);



    m_up = other.m_up;
    m_origin = other.m_origin;
    m_width = other.m_width;
    m_height = other.m_height;
    m_motorCenterDistance = other.m_motorCenterDistance;
}
QuadCopterFrame2D::~QuadCopterFrame2D()
{

}

void QuadCopterFrame2D::update()
{
    float angle = QSFML::VectorMath::getAngle(m_up);



    sf::Vector2f origin = m_origin;
    m_leftDir = QSFML::VectorMath::getRotated(QSFML::VectorMath::getUnitVector(), angle + M_PI_2);
    m_BBL = origin + m_leftDir * m_width/2.f;
    m_BTL = m_BBL + m_up * m_height;
    m_BBR = origin - m_leftDir * m_width/2.f;
    m_BTR = m_BBR + m_up * m_height;
}




void QuadCopterFrame2D::setColor(const sf::Color &color)
{
    m_color = color;
}
const sf::Color &QuadCopterFrame2D::getColor() const
{
    return m_color;
}
void QuadCopterFrame2D::setOrigin(const sf::Vector2f &origin)
{
    m_origin = origin;
}
const sf::Vector2f &QuadCopterFrame2D::getOrigin() const
{
    return m_origin;
}
void QuadCopterFrame2D::setRotation(float rad)
{
    m_up = QSFML::VectorMath::getRotated(sf::Vector2f(-1,0), rad);
}

sf::Vector2f QuadCopterFrame2D::getLeftDir() const
{
    return m_leftDir;
}
sf::Vector2f QuadCopterFrame2D::getUpDir() const
{
    return m_up;
}
sf::Vector2f QuadCopterFrame2D::getLeftMotorMount() const
{
    return getTopCenterPoint() + m_leftDir*m_motorCenterDistance;
}
sf::Vector2f QuadCopterFrame2D::getRightMotorMount() const
{
    return getTopCenterPoint() - m_leftDir*m_motorCenterDistance;
}
sf::Vector2f QuadCopterFrame2D::getTopCenterPoint() const
{
    return m_origin + m_up*m_height;
}


QuadCopterFrame2D::QuadCopterFrame2DPainter::QuadCopterFrame2DPainter(const std::string &name)
    : Drawable(name)
{
    m_copter = nullptr;
}
QuadCopterFrame2D::QuadCopterFrame2DPainter::QuadCopterFrame2DPainter(const QuadCopterFrame2DPainter &other)
    : Drawable(other)
{
    m_copter = nullptr;
}
QuadCopterFrame2D::QuadCopterFrame2DPainter::~QuadCopterFrame2DPainter()
{

}

void QuadCopterFrame2D::QuadCopterFrame2DPainter::draw(sf::RenderTarget& target,
                                             sf::RenderStates states) const
{
    if(!m_copter) return;

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
