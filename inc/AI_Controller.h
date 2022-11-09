#pragma once
#include "SFML_EditorWidget.h"
#include "QuadCopter2D.h"
#include "neuronalNet.h"
#include "netModel.h"

class AI_Controller: public QObject, public QSFML::Objects::CanvasObject
{
        Q_OBJECT
    public:
        AI_Controller(const std::string &name = "AI_Controller",
                     CanvasObject *parent = nullptr);
        AI_Controller(const AI_Controller &other);
        ~AI_Controller();
        CLONE_FUNC(AI_Controller);

        size_t getInputSize();

        void setPause(bool pause);
        void enableNetDisplay(bool enable);

        void reset();
        void update() override;
        std::vector<float> getInputVector();
        void processOutputVector(const NeuronalNet::SignalVector &out);

        void setNet(NeuronalNet::Net *net);
        float getScore() const;
        void setScore(float value);
        sf::Vector2f getMovedVec();
        float getAngleError();


    private:
        float calculateDeltaScore();

        QuadCopter2D *m_copter;
        NeuronalNet::Net *m_net;
        NeuronalNet::Graphics::NetModel *m_netModel;


        sf::Vector2f m_startPos;
        float m_score;

        float m_targetHeight;
        float m_targetAngle;

        bool m_paused;

};
