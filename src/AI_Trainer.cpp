#include "AI_trainer.h"
#include "canvas/CanvasSettings.h"
#include "SimulationSettings.h"

AI_Trainer::AI_Trainer(const std::string &name,
                        CanvasObject *parent)
    : CanvasObject(name, parent)
{
    setup();
}
AI_Trainer::AI_Trainer(const AI_Trainer &other)
    : CanvasObject(other)
{
    setup();
}
AI_Trainer::~AI_Trainer()
{
    delete m_geneticNet;
}

void AI_Trainer::setPause(bool pause)
{
    m_paused = pause;
    for(size_t i=0; i<m_agents.size(); ++i)
    {
        m_agents[i]->setPause(m_paused);
    }
}

void AI_Trainer::setup()
{
    m_pauseToggle = new QSFML::Components::KeyPressEvent("pauseToggle",sf::Keyboard::Space);
    connect(m_pauseToggle, &QSFML::Components::KeyPressEvent::fallingEdge,
            this, &AI_Trainer::onPauseToggled);
    addComponent(m_pauseToggle);

    m_displayNets = false;
    m_netDisplayToggle = new QSFML::Components::KeyPressEvent("netToggle",sf::Keyboard::N);
    connect(m_netDisplayToggle, &QSFML::Components::KeyPressEvent::fallingEdge,
            this, &AI_Trainer::onNetDisplayToggle);
    addComponent(m_netDisplayToggle);

    m_paused = false;
    m_maxCycles = 20000;

    size_t agentCount = 100;
    size_t inputCount = 0;
    m_agents.reserve(agentCount);
    for(size_t i=0; i<agentCount; ++i)
    {
        AI_Controller* controller = new AI_Controller("Controller_"+std::to_string(i));
        if(inputCount == 0)
            inputCount = controller->getInputSize();

        //addChild(controller);
        m_agents.push_back(controller);
    }


    m_geneticNet = new NeuronalNet::GeneticNet(agentCount);
    NeuronalNet::NetSerializer serializer;
    serializer.setFilePath("save.net");
    if(!serializer.readFromFile(m_geneticNet))
    {
        m_geneticNet->setDimensions(inputCount, 1, 10 , 2);
        m_geneticNet->setActivation(NeuronalNet::Activation::sigmoid);
        m_geneticNet->setMutationChance(1);
        m_geneticNet->setMutationFactor(0.1);
        m_geneticNet->build();
    }



    for(size_t i=0; i<agentCount; ++i)
    {
        m_agents[i]->setNet(m_geneticNet->getNet(i));
    }
    reset();
}
void AI_Trainer::reset()
{
    m_cyclesCounter = 0;
    for(size_t i=0; i<m_agents.size(); ++i)
    {
        m_agents[i]->reset();
    }
}
void AI_Trainer::update()
{
    if(m_paused)
        return;
    /*for(size_t j=0; j<50; ++j)
    {
        for(size_t i=0; i<m_agents.size(); ++i)
        {
            m_agents[i]->update();
        }

    }*/

    ++m_cyclesCounter;
    if(m_cyclesCounter/SimulationSettings::getDeltaT() >= m_maxCycles)
    {
        learn();
        NeuronalNet::NetSerializer serializer;
        serializer.setFilePath("save.net");
        serializer.saveToFile(m_geneticNet);
        reset();
    }
}
void AI_Trainer::learn()
{
    std::vector<float> scores(m_agents.size(), 0);
    float average = 0;
    for(size_t i=0; i<m_agents.size(); ++i)
    {
        scores[i] = m_agents[i]->getScore();
        average += scores[i];
    }
    average /= (float)m_agents.size();
    qDebug() <<average;
    m_geneticNet->learn(scores);
}
void AI_Trainer::toggleDisplay()
{
    if(getCanvasParent())
    {
        QSFML::CanvasSettings::UpdateControlls controlls =  getCanvasParent()->getUpdateControlls();
        controlls.enablePaintLoop = !controlls.enablePaintLoop;
        getCanvasParent()->setUpdateControlls(controlls);
    }
}
void AI_Trainer::onPauseToggled()
{
    setPause(!m_paused);
}
void AI_Trainer::onNetDisplayToggle()
{
    m_displayNets = !m_displayNets;
    /*for(size_t i=0; i<m_agents.size(); ++i)
    {
        m_agents[i]->enableNetDisplay(m_displayNets);
    }*/
    m_agents[0]->enableNetDisplay(m_displayNets);

}
void AI_Trainer::onCanvasParentChange(QSFML::Canvas *oldParent,
                                      QSFML::Canvas *newParent)
{
    if(oldParent)
    {
        for(size_t i=0; i<m_agents.size(); ++i)
        {
            oldParent->removeObject(m_agents[i]);
        }
    }
    if(newParent)
    {
        for(size_t i=0; i<m_agents.size(); ++i)
        {
            newParent->addObject(m_agents[i]);
        }
    }
}
