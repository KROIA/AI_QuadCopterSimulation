#include "PID.h"



float PID::m_deltaT = 1;
PID::PID(const std::string &name,
         CanvasObject *parent)
    :   CanvasObject(name, parent)
{

    m_pChart = new QSFML::Objects::LineChart();
    m_pChart->setColor(sf::Color::Red);
    m_pChart->setSize(sf::Vector2f(200,100));
    addChild(m_pChart);

    m_iChart = new QSFML::Objects::LineChart();
    m_iChart->setColor(sf::Color::Green);
    m_iChart->setSize(sf::Vector2f(200,100));
    addChild(m_iChart);

    m_dChart = new QSFML::Objects::LineChart();
    m_dChart->setColor(sf::Color::Blue);
    m_dChart->setSize(sf::Vector2f(200,100));
    addChild(m_dChart);
    m_d1Chart = new QSFML::Objects::LineChart();

    m_d1Chart->setColor(sf::Color::Cyan);
    m_d1Chart->setSize(sf::Vector2f(200,100));
    addChild(m_d1Chart);

    m_outChart = new QSFML::Objects::LineChart();
    m_outChart->setColor(sf::Color::White);
    m_outChart->setSize(sf::Vector2f(200,100));
    addChild(m_outChart);



    m_iBounrdy = 100000;

    setChartPointCount(100);
}
PID::PID(const PID &other)
    :   CanvasObject(other)
{

}
PID::PID(float p, float i, float d, float d1)
{
    set(p, i, d, d1);
    reset();
}
PID::~PID()
{

}

void PID::setDeltaT(float t)
{
    m_deltaT = t;
    if(m_deltaT <= 0)
        m_deltaT = 0.000001;
}
float PID::getDeltaT()
{
    return m_deltaT;
}

void PID::setPSetting(float p)
{
    m_pSetting = p;
}
float PID::getPSetting() const
{
    return m_pSetting;
}

void PID::setISetting(float i)
{
    m_iSetting = i;
}
float PID::getISetting() const
{
    return m_iSetting;
}
void PID::setIBoundry(float radius)
{
    m_iBounrdy = radius;
}
float PID::getIBoundry() const
{
    return m_iBounrdy;
}

void PID::setDSetting(float d)
{
    m_dSetting = d;
}
float PID::getDSetting() const
{
    return m_dSetting;
}
void PID::setD1Setting(float d1)
{
    m_d1Setting = d1;
}
float PID::getD1Setting() const
{
    return m_d1Setting;
}

void PID::set(float p, float i, float d, float d1)
{
    m_pSetting = p;
    m_iSetting = i;
    m_dSetting = d;
    m_d1Setting = d1;
}
void PID::reset()
{
    m_p = 0;
    m_i = 0;
    m_d = 0;
    m_d1 = 0;

    m_input = 0;
    m_output = 0;

    m_pChart->clear();
    m_iChart->clear();
    m_dChart->clear();
    m_d1Chart->clear();
    m_outChart->clear();
}

float PID::calculate(float input)
{
    m_p = m_pSetting * input;
    m_i += m_iSetting * input * m_deltaT;
    if(m_i > m_iBounrdy)
        m_i = m_iBounrdy;
    else if(m_i < -m_iBounrdy)
        m_i = -m_iBounrdy;

    float lastD = m_d;
    m_d = m_dSetting * (input - m_input) / m_deltaT;
    m_d1 = m_d1Setting * (m_d - lastD) / m_deltaT;

    //m_lastChange =
    //m_lastInput = m_input;

    m_input = input;
    m_output = m_p + m_i + m_d + m_d1;

    return m_output;
}
float PID::getInput() const
{
    return m_input;
}
float PID::getOutput() const
{
    return m_output;
}
float PID::getP() const
{
    return m_p;
}
float PID::getI() const
{
    return m_i;
}
float PID::getD() const
{
    return m_d;
}

void PID::setChartSize(const sf::Vector2f &size)
{
    m_pChart->setSize(size);
    m_iChart->setSize(size);
    m_dChart->setSize(size);
    m_d1Chart->setSize(size);
    m_outChart->setSize(size);
}

void PID::setChartPos(const sf::Vector2f &pos)
{
    m_pChart->setOrigin(pos);
    m_iChart->setOrigin(pos);
    m_dChart->setOrigin(pos);
    m_d1Chart->setOrigin(pos);
    m_outChart->setOrigin(pos);
}

void PID::setChartPointCount(size_t maxPoints)
{
    m_maxChartPoints = maxPoints;
}
void PID::update()
{
    addPoint(m_pChart, m_p);
    addPoint(m_iChart, m_i);
    addPoint(m_dChart, m_d);
    addPoint(m_d1Chart, m_d1);
    addPoint(m_outChart,m_output);
}
void PID::addPoint(QSFML::Objects::LineChart *chart,
                   float point)
{
    std::vector<float> points = chart->getDataPoints();
    points.push_back(point);
    if(points.size() > m_maxChartPoints)
        points.erase(points.begin());
    chart->setDataPoints(points);
}
