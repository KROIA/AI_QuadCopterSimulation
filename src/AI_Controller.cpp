#include "AI_Controller.h"


AI_Controller::AI_Controller(const std::string &name,
                             CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_score = 0;
    m_targetHeight = 300;
    m_targetAngle = -M_PI_2;
    m_startPos = sf::Vector2f(100+(rand()%1800),300);

    m_copter = new QuadCopter2D(name);

   /* m_keyW = new QSFML::Components::KeyPressEvent("",sf::Keyboard::W);
    m_keyA = new QSFML::Components::KeyPressEvent("",sf::Keyboard::A);
    m_keyS = new QSFML::Components::KeyPressEvent("",sf::Keyboard::S);
    m_keyD = new QSFML::Components::KeyPressEvent("",sf::Keyboard::D);
    m_keyQ = new QSFML::Components::KeyPressEvent("",sf::Keyboard::Q);
    m_keyE = new QSFML::Components::KeyPressEvent("",sf::Keyboard::E);
    connect(m_keyW, &QSFML::Components::KeyPressEvent::down,
            this, &AI_Controller::onUpPressed);
    connect(m_keyA, &QSFML::Components::KeyPressEvent::down,
            this, &AI_Controller::onLeftPressed);
    connect(m_keyS, &QSFML::Components::KeyPressEvent::down,
            this, &AI_Controller::onDownPressed);
    connect(m_keyD, &QSFML::Components::KeyPressEvent::down,
            this, &AI_Controller::onRightPressed);
    connect(m_keyQ, &QSFML::Components::KeyPressEvent::down,
            this, &AI_Controller::onRightTurn);
    connect(m_keyE, &QSFML::Components::KeyPressEvent::down,
            this, &AI_Controller::onLeftTurn);*/

    addChild(m_copter);
    /*addComponent(m_keyW);
    addComponent(m_keyA);
    addComponent(m_keyS);
    addComponent(m_keyD);
    addComponent(m_keyQ);
    addComponent(m_keyE);
    m_aiControlled = true;*/

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
/*
void AI_Controller::setAiControlled(bool enable)
{
    m_aiControlled = enable;
    m_keyControlledPos = m_copter->getPosition();
    m_keyControlledAngle = m_copter->getAngle();
}*/
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

    //if(m_aiControlled)
    {
        m_net->setInputVector(getInputVector());
        m_net->calculate();
        processOutputVector(m_net->getOutputVector());
        m_score += calculateDeltaScore();
        if(m_copter->getGroundWasHit())
            m_score *= 0.5;
    }
   // else
    {

    }

    m_netModel->setPos(m_copter->getPosition());
}
std::vector<float> AI_Controller::getInputVector()
{
    float angleError   = getAngleError();
    //float heightError  = 600
    float heightDifference = m_copter->getHeight() - m_targetHeight;
    float heightError1 = getBounded(heightDifference * 0.001);
    float heightError2 = getBounded(heightError1*2);
    float heightError3 = getBounded(heightError2*2);
    sf::Vector2f moved = getMovedVec();
    float xOffset1 = getBounded(moved.x *0.001);
    float xOffset2 = getBounded(xOffset1 * 2);
    float xOffset3 = getBounded(xOffset2 * 2);
    float yOffset = moved.y *0.01;
    Force acceleration = m_copter->getAcceleration();
    float xAcceleration = acceleration.getForceVector().x * 0.1;
    float yAcceleration = acceleration.getForceVector().y * 0.1;
    float rotationalAcceleration = acceleration.getTorque() *0.01;
    float accelerationValue = acceleration.getForce() * 0.1;

    Force speed = m_copter->getSpeed();
    float xSpeed1 = speed.getForceVector().x *0.01;
    float xSpeed2 = getBounded(xSpeed1*2);
    float xSpeed3 = getBounded(xSpeed2*2);
    float ySpeed1 = speed.getForceVector().y *0.01;
    float ySpeed2 = getBounded(ySpeed1*2);
    float ySpeed3 = getBounded(ySpeed2*2);
    float rotationalSpeed = speed.getTorque() *0.001;
    float speedValue = speed.getForce() * 0.01;

    std::vector<float> inputs{
        angleError,
        heightError1,
        heightError2,
        heightError3,
        //heightError1*heightError1,
        //heightError2*heightError2,
        //heightError3*heightError3,
        xOffset1,
        xOffset2,
        xOffset3,
        //yOffset,
        //xAcceleration,
        //xAcceleration*xAcceleration*xAcceleration,
        //yAcceleration,
        //yAcceleration*yAcceleration*yAcceleration,
        //rotationalAcceleration,
        //rotationalAcceleration*rotationalAcceleration*rotationalAcceleration,
        accelerationValue,
        xSpeed1,
        xSpeed2,
        xSpeed3,
        ySpeed1,
        ySpeed2,
        ySpeed3,
        //ySpeed,
        speedValue,
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
    float signalMultiplyer = 15;

    float leftMotor  = (out[0] + signalOffset) * signalMultiplyer;
    float rightMotor = (out[1] + signalOffset) * signalMultiplyer;
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
        m_netModel->setNeuronSpacing(sf::Vector2f(30,1));
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
    float velocityScore = 1+NET_ACTIVATION_GAUSSIAN(m_copter->getSpeed().getForce()*0.1);
    //qDebug() <<accelerationScore << " " <<m_copter->getAcceleration().getForce();

    if(heightScore < 0)
        heightScore = 0;
    if(xPosCore < 0 )
        xPosCore = 0;
    if(yPosCore < 0 )
        yPosCore = 0;
    if(angleScore < 0)
        angleScore = 0;

    //return angleScore;
    //return accelerationScore*accelerationScore + velocityScore*velocityScore*0.1 + angleScore + heightScore*3;
    return heightScore + xPosCore /*+ yPosCore */+ angleScore + accelerationScore*accelerationScore;
}
float AI_Controller::getBounded(float value, float min, float max)
{
    if(value < min)
        return min;
    if(value > max)
        return max;
    return value;
}
/*
void AI_Controller::onUpPressed()
{

}
void AI_Controller::onLeftPressed()
{

}
void AI_Controller::onDownPressed()
{

}
void AI_Controller::onRightPressed()
{

}
void AI_Controller::onRightTurn()
{

}
void AI_Controller::onLeftTurn()
{

}
*/
