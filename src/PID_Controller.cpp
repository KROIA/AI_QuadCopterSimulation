#include "PID_Controller.h"


PID_Controller::PID_Controller(const std::string &name,
                               CanvasObject *parent)
{
    m_targetHeight = 300;
    m_targetHorizontalPos = 1000;
    m_targetAngle = -M_PI_2;
    m_startPos = sf::Vector2f(100+(rand()%1800),300);
    m_paused = false;

    m_copter = new QuadCopter2D(name);
    addChild(m_copter);

    m_heightPIDChart = new QSFML::Objects::LineChart();
    m_heightPIDChart->setColor(sf::Color::Cyan);
    m_heightPIDChart->setSize(sf::Vector2f(200,100));
    addChild(m_heightPIDChart);

    m_anglePIDChart = new QSFML::Objects::LineChart();
    m_anglePIDChart->setColor(sf::Color::Magenta);
    m_anglePIDChart->setSize(sf::Vector2f(200,100));
    addChild(m_anglePIDChart);

    m_horizontalPIDChart = new QSFML::Objects::LineChart();
    m_horizontalPIDChart->setColor(sf::Color::Green);
    m_horizontalPIDChart->setSize(sf::Vector2f(200,100));
    addChild(m_horizontalPIDChart);

    m_anglePID = new PID();
    m_anglePID->setChartPointCount(1000);
    m_anglePID->setChartSize(sf::Vector2f(80,100));
    addChild(m_anglePID);
    m_heightPID = new PID();
    m_heightPID->setChartPointCount(1000);
    m_heightPID->setChartSize(sf::Vector2f(80,100));
    addChild(m_heightPID);
    m_horizontalPID = new PID();
    m_horizontalPID->setChartPointCount(1000);
    m_horizontalPID->setChartSize(sf::Vector2f(80,100));
    addChild(m_horizontalPID);

  //  m_pidLeft.set(1,1,1);
 //   m_pidRight.set(1,1,1);
    m_anglePID->set(100,0.003,975,0);
    m_heightPID->set(-0.8,-0.001,-319,0);
    m_horizontalPID->set(2.1,0,612,0);
    reset();
}
PID_Controller::PID_Controller(const PID_Controller &other)
{
    m_targetHeight = other.m_targetHeight;
    m_targetAngle = other.m_targetAngle;

    m_copter = new QuadCopter2D(other.getName());
    addChild(m_copter);

    m_paused = false;
    reset();
}
PID_Controller::~PID_Controller()
{

}


/*PID &PID_Controller::getLeftPid()
{
    return m_pidLeft;
}
PID &PID_Controller::getRightPid()
{
    return m_pidRight;
}*/

PID *PID_Controller::getAnglePid()
{
    return m_anglePID;
}
PID *PID_Controller::getHeightPid()
{
    return m_heightPID;
}
PID *PID_Controller::getHorizontalPid()
{
    return m_horizontalPID;
}

void PID_Controller::reset()
{
    m_copter->setPosition(Force({0,0}, m_startPos, 0));
    m_copter->setSpeed(Force({0,0}, {0,0}, 0));
    m_anglePIDChart->clear();
    m_heightPIDChart->clear();
    m_horizontalPIDChart->clear();
    resetPID();
}
void PID_Controller::update()
{
    float heightError = getBounded(getHeightError(),-50,50);
    //float angleError = getAngleError();
    float angleError = QSFML::VectorMath::getAngle(sf::Vector2f(1,0),m_copter->getUpVector()) - m_targetAngle;

    float horizontalError = getBounded(getHorizontalError()*0.01,-500,500);
    //angleError = M_PI-angleError;
    //if(angleError > M_PI)
    //    angleError -= 2*M_PI;
    //qDebug()<<angleError;

    float angleControll = m_anglePID->calculate(angleError);
    float heightControll = m_heightPID->calculate(heightError);
    float horizontalControll = getBounded(m_horizontalPID->calculate(horizontalError), -5,5);

    float maxMotor = 15;
    float leftControll =  getBounded(heightControll - angleControll - horizontalControll,0,maxMotor);
    float rightControll = getBounded(heightControll + angleControll + horizontalControll,0,maxMotor);
   // qDebug() << "Motor controlls: "<<leftControll<<" "<<rightControll;
    m_copter->setMotorForce(leftControll,rightControll);


    std::vector<float> anglePoints = m_anglePIDChart->getDataPoints();
    std::vector<float> heightPoints = m_heightPIDChart->getDataPoints();
    std::vector<float> horizontalPoints = m_horizontalPIDChart->getDataPoints();
    anglePoints.push_back(angleError);
    heightPoints.push_back(heightError);
    horizontalPoints.push_back(horizontalError);
    if(anglePoints.size() > 1000)
        anglePoints.erase(anglePoints.begin());
    if(heightPoints.size() > 1000)
        heightPoints.erase(heightPoints.begin());
    if(horizontalPoints.size() > 1000)
        horizontalPoints.erase(horizontalPoints.begin());
    m_anglePIDChart->setDataPoints(anglePoints);
    m_heightPIDChart->setDataPoints(heightPoints);
    m_horizontalPIDChart->setDataPoints(horizontalPoints);

    sf::Vector2f offset(-100,-250);
    m_heightPIDChart->setOrigin(m_copter->getPosition()+offset);
    m_anglePIDChart->setOrigin(m_copter->getPosition()+offset);
    m_horizontalPIDChart->setOrigin(m_copter->getPosition()+offset);
    m_anglePID->setChartPos(m_copter->getPosition() + sf::Vector2f(-100,-100));
    m_heightPID->setChartPos(m_copter->getPosition() + sf::Vector2f( 0,-100));
    m_horizontalPID->setChartPos(m_copter->getPosition() + sf::Vector2f( 100,-100));

}
float PID_Controller::getAngleError() const
{
    return 1-QSFML::VectorMath::dotProduct(QSFML::VectorMath::getRotated(sf::Vector2f(1,0),m_targetAngle),
                                         m_copter->getUpVector());

}
float PID_Controller::getHeightError() const
{
    return m_copter->getHeight() - m_targetHeight;
}
float PID_Controller::getHorizontalError() const
{
    return m_copter->getPosition().x - m_targetHorizontalPos;
}

/*
void PID_Controller::setPSetting(float p)
{
    m_pidLeft.setPSetting(p);
    m_pidRight.setPSetting(p);
}
void PID_Controller::setISetting(float i)
{
    m_pidLeft.setISetting(i);
    m_pidRight.setISetting(i);
}
void PID_Controller::setDSetting(float d)
{
    m_pidLeft.setDSetting(d);
    m_pidRight.setDSetting(d);
}*/

void PID_Controller::resetPID()
{
    m_anglePID->reset();
    m_heightPID->reset();
    m_horizontalPID->reset();
}
void PID_Controller::setAnglePSetting(float p)
{
    m_anglePID->setPSetting(p);
}
void PID_Controller::setAngleISetting(float i)
{
    m_anglePID->setISetting(i);
}
void PID_Controller::setAngleDSetting(float d)
{
    m_anglePID->setDSetting(d);
}
void PID_Controller::setAngleD1Setting(float d1)
{
    m_anglePID->setD1Setting(d1);
}

void PID_Controller::setHeightPSetting(float p)
{
    m_heightPID->setPSetting(p);
}
void PID_Controller::setHeightISetting(float i)
{
    m_heightPID->setISetting(i);
}
void PID_Controller::setHeightDSetting(float d)
{
    m_heightPID->setDSetting(d);
}
void PID_Controller::setHeightD1Setting(float d1)
{
    m_heightPID->setD1Setting(d1);
}

void PID_Controller::setHorizontalPSetting(float p)
{
    m_horizontalPID->setPSetting(p);
}
void PID_Controller::setHorizontalISetting(float i)
{
    m_horizontalPID->setISetting(i);
}
void PID_Controller::setHorizontalDSetting(float d)
{
    m_horizontalPID->setDSetting(d);
}
void PID_Controller::setHorizontalD1Setting(float d1)
{
    m_horizontalPID->setD1Setting(d1);
}
float PID_Controller::getBounded(float value, float min, float max)
{
    if(value < min)
        return min;
    if(value > max)
        return max;
    return value;
}
