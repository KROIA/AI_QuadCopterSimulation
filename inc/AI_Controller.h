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

        //oid setAiControlled(bool enable);
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



    private slots:
        /*void onUpPressed();
        void onLeftPressed();
        void onDownPressed();
        void onRightPressed();
        void onRightTurn();
        void onLeftTurn();*/
    private:
        float calculateDeltaScore();
        float getBounded(float value, float min=-1, float max=1);

        QuadCopter2D *m_copter;
        NeuronalNet::Net *m_net;
        NeuronalNet::Graphics::NetModel *m_netModel;
/*
        QSFML::Components::KeyPressEvent *m_keyW;
        QSFML::Components::KeyPressEvent *m_keyA;
        QSFML::Components::KeyPressEvent *m_keyS;
        QSFML::Components::KeyPressEvent *m_keyD;
        QSFML::Components::KeyPressEvent *m_keyQ;
        QSFML::Components::KeyPressEvent *m_keyE;
*/


        sf::Vector2f m_startPos;
        float m_score;

        float m_targetHeight;
        float m_targetAngle;

        bool m_paused;
        //bool m_aiControlled;

        sf::Vector2f m_keyControlledPos;
        float m_keyControlledAngle;

};
