#include "Force.h"

Force::Force()
{
    m_actingPoint = sf::Vector2f(0,0);
    m_force = sf::Vector2f(0,0);
    m_torque = 0;
}
Force::Force(const sf::Vector2f &pos,
             const sf::Vector2f &force,
             float torque)
{
    m_actingPoint = pos;
    m_force = force;
    m_torque = torque;
}
Force::Force(const Force &other)
{
    m_actingPoint = other.m_actingPoint;
    m_force = other.m_force;
    m_torque = other.m_torque;
}

const Force &Force::operator=(const Force &other)
{
    m_actingPoint = other.m_actingPoint;
    m_force = other.m_force;
    m_torque = other.m_torque;
    return *this;
}
const Force Force::operator+(const Force &other)
{
    sf::Vector2f newOrigin = (other.m_actingPoint + m_actingPoint)*0.5f;
    return Force(newOrigin, m_force + other.m_force, m_torque + other.m_torque);
}
const Force Force::operator-(const Force &other)
{
    sf::Vector2f newOrigin = (other.m_actingPoint + m_actingPoint)*0.5f;
    return Force(newOrigin, m_force - other.m_force, m_torque - other.m_torque);
}
const Force &Force::operator+=(const Force &other)
{
    m_actingPoint = (m_actingPoint + other.m_actingPoint)*0.5f;
    m_force += other.m_force;
    m_torque += other.m_torque;
    return *this;
}
const Force &Force::operator-=(const Force &other)
{
    m_actingPoint = (m_actingPoint + other.m_actingPoint)*0.5f;
    m_force -= other.m_force;
    m_torque -= other.m_torque;
    return *this;
}
const Force Force::operator*(float scalar)
{
    return Force(m_actingPoint, m_force*scalar, m_torque*scalar);
}
const Force &Force::operator*=(float scalar)
{
    m_force *= scalar;
    m_torque *= scalar;
    return *this;
}

void Force::setActingPoint(const sf::Vector2f &pos)
{
    m_actingPoint = pos;
}
const sf::Vector2f &Force::getActingPoint() const
{
    return m_actingPoint;
}

void Force::setForceVector(const sf::Vector2f &force)
{
    m_force = force;
}
const sf::Vector2f &Force::getForceVector() const
{
    return m_force;
}
void Force::setForce(float force)
{
    m_force = QSFML::VectorMath::getNormalized(m_force)*force;
}
float Force::getForce() const
{
    return QSFML::VectorMath::getLength(m_force);
}

void Force::setTorque(float torque)
{
    m_torque = torque;
}
float Force::getTorque() const
{
    return m_torque;
}
float Force::getTorque(const sf::Vector2f &onPos) const
{
    return QSFML::VectorMath::crossProduct(m_force, onPos - m_actingPoint) + m_torque;
}

Force Force::getSum(const std::vector<Force> &forcefield,
                    const sf::Vector2f &onPos)
{
    Force sum;
    sum.setActingPoint(onPos);
    for(size_t i=0; i<forcefield.size(); ++i)
    {
        sum.m_force += forcefield[i].m_force;
        sum.m_torque += forcefield[i].getTorque(onPos);
    }
    return sum;
}

/*Force Force::getOnPosition(const sf::Vector2f &pos)
{
    sf::Vector2f dir = pos - m_actingPoint;

    float torque = QSFML::VectorMath::crossProduct(m_force, dir);


}*/
/*float Force::getGlobalTorque() const
{
    return QSFML::VectorMath::crossProduct(m_force, m_actingPoint);
}
*/



ForcePainter::ForcePainter(const std::string &name)
    : Drawable(name)
{
    m_scale = 1;
    m_torqueVertsCount = 0;
    m_torqueVerts = nullptr;
    setColor(sf::Color::Red);
}
ForcePainter::ForcePainter(const ForcePainter &other)
    : Drawable(other)
{
    m_force = other.m_force;
    m_scale = other.m_scale;
    m_color = other.m_color;
    m_torqueVertsCount = 0;
    m_torqueVerts = nullptr;
}
ForcePainter::~ForcePainter()
{
    sf::Vertex * tmp = m_torqueVerts;
    m_torqueVerts = nullptr;
    m_torqueVertsCount = 0;
    delete[] tmp;
}
void ForcePainter::setColor(const sf::Color &color)
{
    m_color = color;
    m_vec.setColor(m_color);
    for(size_t i=0; i<m_torqueVertsCount; ++i)
    {
        m_torqueVerts[i].color = m_color;
    }
}
void ForcePainter::setScale(float scale)
{
    m_scale = scale;
    setForce(m_force);
}
void ForcePainter::setForce(const Force &force)
{
    m_force = force;
    m_vec.setPoints(m_force.getActingPoint(), m_force.getActingPoint() + m_force.getForceVector()*m_scale);
    updateVisuals();
}
void ForcePainter::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const
{
    m_vec.draw(target, states);
    if(m_torqueVerts)
        target.draw(m_torqueVerts, m_torqueVertsCount, sf::LinesStrip);
}
void ForcePainter::updateVisuals()
{
    // Torque
    if(m_force.getTorque() == 0)
    {
        sf::Vertex * tmp = m_torqueVerts;
        m_torqueVerts = nullptr;
        m_torqueVertsCount = 0;
        delete[] tmp;

        return;
    }

    float radius = 2;
    float radiusOffset = 0.3;

    float deltaAngle = m_force.getTorque()*0.001*m_scale;
    size_t resolution = 10 + abs(deltaAngle*10);
    if(resolution != m_torqueVertsCount)
    {
        sf::Vertex * tmp = m_torqueVerts;
        m_torqueVerts = nullptr;
        m_torqueVertsCount = 0;
        delete[] tmp;

        m_torqueVerts = new sf::Vertex[resolution];
        m_torqueVertsCount = resolution;
        for(size_t i=0; i<m_torqueVertsCount; ++i)
        {
            m_torqueVerts[i].color = m_color;
        }
    }
    sf::Vector2f pos = m_force.getActingPoint();
    float angle = 0;

    deltaAngle /= (float)m_torqueVertsCount;
    for(size_t i=0; i<m_torqueVertsCount; ++i)
    {
        sf::Vector2f f = QSFML::VectorMath::getRotated(sf::Vector2f(1,0) * (radius + radiusOffset*i),angle);
        m_torqueVerts[i].position = pos + f;
        angle += deltaAngle;
    }
}
