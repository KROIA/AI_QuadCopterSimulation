#include "AI_Controller.h"


AI_Controller::AI_Controller(const std::string &name,
                             CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_score = 0;
    m_targetHeight = 300;
    m_targetAngle = -M_PI_2;
    m_startPos = sf::Vector2f(100+(rand()%1800),300+rand()%20);

    m_copter = new QuadCopter2D(name);



    addChild(m_copter);

    m_paused = false;
}
AI_Controller::AI_Controller(const AI_Controller &other)
    : CanvasObject(other)
{
    m_score = other.m_score;
    m_targetHeight = other.m_targetHeight;
    m_targetAngle = other.m_targetAngle;

    m_copter = new QuadCopter2D(other.getName());
    addChild(m_copter);

    m_paused = false;
}
AI_Controller::~AI_Controller()
{

}

size_t AI_Controller::getInputSize()
{
    return getInputVector().size();
}
void AI_Controller::setPause(bool pause)
{
    m_paused = pause;
    m_copter->setPause(m_paused);
}
void AI_Controller::enableNetDisplay(bool enable)
{
    m_netModel->setEnabled(enable);
}

void AI_Controller::reset()
{
    m_copter->setPosition(Force({0,0}, m_startPos, 0));
    m_copter->setSpeed(Force({0,0}, {0,0}, 0));
    setScore(0);
}
void AI_Controller::update()
{
    if(!m_net || m_paused)
        return;
    m_net->setInputVector(getInputVector());
    m_net->calculate();
    processOutputVector(m_net->getOutputVector());
    m_score += calculateDeltaScore();
    if(m_copter->getGroundWasHit())
        m_score *= 0.8;

    m_netModel->setPos(m_copter->getPosition());
}
std::vector<float> AI_Controller::getInputVector()
{
    float angleError   = getAngleError();
    //float heightError  = 600
    float heightError  = (m_copter->getHeight() - m_targetHeight) * 0.01;
    sf::Vector2f moved = getMovedVec();
    float xOffset = moved.x *0.01;
    float yOffset = moved.y *0.01;
    Force acceleration = m_copter->getAcceleration();
    float xAcceleration = acceleration.getForceVector().x * 0.1;
    float yAcceleration = acceleration.getForceVector().y * 0.1;
    float rotationalAcceleration = acceleration.getTorque() *0.01;

    Force speed = m_copter->getSpeed();
    float xSpeed = speed.getForceVector().x *0.01;
    float ySpeed = speed.getForceVector().y *0.01;
    float rotationalSpeed = speed.getTorque() *0.001;

    std::vector<float> inputs{
        angleError,
        heightError,
      //  xOffset,
        //yOffset,
        xAcceleration,
        yAcceleration,
        rotationalAcceleration,
        xSpeed,
        ySpeed,
        rotationalSpeed
    };

   /* qDebug() << angleError << " "
             << heightError << " "
             << xOffset << " "
             << yOffset << " acc: "
             << xAcceleration << " "
             << yAcceleration << " "
             << rotationalAcceleration << " speed: "
             << xSpeed << " "
             << ySpeed << " "
             << rotationalSpeed << " ";*/
    return inputs;
}
void AI_Controller::processOutputVector(const NeuronalNet::SignalVector &out)
{
    float signalOffset = 0;
    float signalMultiplyer = 10;

    float leftMotor  = (out[1] + signalOffset) * signalMultiplyer;
    float rightMotor = (out[0] + signalOffset) * signalMultiplyer;
    m_copter->setMotorForce(leftMotor , rightMotor);
   // qDebug() << leftMotor << " "<< rightMotor;
}

void AI_Controller::setNet(NeuronalNet::Net *net)
{
    m_net = net;
    if(m_net)
    {
        m_netModel = new NeuronalNet::Graphics::NetModel(m_net);
        m_netModel->setConnectionWidth(0.5);
        m_netModel->setSignalWidth(0.4);
        m_netModel->setNeuronSize(2);
        m_netModel->setNeuronSpacing(sf::Vector2f(30,5));
        m_netModel->setVisualConfiguration(NeuronalNet::Graphics::NetModel::getStandardVisualConfiguration() -
                                           NeuronalNet::Graphics::VisualConfiguration::weightMap);
        addChild(m_netModel);
        m_netModel->setEnabled(false);
    }
}
float AI_Controller::getScore() const
{
    return m_score;
}
void AI_Controller::setScore(float value)
{
    m_score = value;
}
sf::Vector2f AI_Controller::getMovedVec()
{
    return m_copter->getPosition() - m_startPos;
}
float AI_Controller::getAngleError()
{
    return QSFML::VectorMath::dotProduct(QSFML::VectorMath::getRotated(sf::Vector2f(1,0),m_targetAngle),
                                         m_copter->getUpVector());
}

float AI_Controller::calculateDeltaScore()
{
    float heightScore = NET_ACTIVATION_GAUSSIAN((m_copter->getHeight() - m_targetHeight)*0.01);

    sf::Vector2f moved = getMovedVec();
    float xPosCore = NET_ACTIVATION_GAUSSIAN((moved.x - m_startPos.x)*0.01);
    float yPosCore = NET_ACTIVATION_GAUSSIAN((moved.x - m_startPos.x)*0.01);

    float angleScore   = getAngleError();
    float accelerationScore = 1+NET_ACTIVATION_GAUSSIAN(m_copter->getAcceleration().getForce());

    if(heightScore < 0)
        heightScore = 0;
    if(xPosCore < 0 )
        xPosCore = 0;
    if(yPosCore < 0 )
        yPosCore = 0;
    if(angleScore < 0)
        angleScore = 0;

    return heightScore + /*xPosCore + yPosCore*/ + angleScore + accelerationScore*0.3;
}
