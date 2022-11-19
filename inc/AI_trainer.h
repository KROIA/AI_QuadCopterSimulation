#pragma once

#include "QSFML_EditorWidget.h"
#include "QuadCopter2D.h"
#include "neuronalNet.h"
#include "AI_Controller.h"

class AI_Trainer: public QObject,  public QSFML::Objects::CanvasObject
{
        Q_OBJECT
    public:
        AI_Trainer(const std::string &name = "AI_Trainer",
                     CanvasObject *parent = nullptr);
        AI_Trainer(const AI_Trainer &other);
        ~AI_Trainer();
        CLONE_FUNC(AI_Trainer);

        void setPause(bool pause);
        void setup();

        void reset();
        void update() override;
        void learn();


    public slots:
        void toggleDisplay();
    private slots:
        void onPauseToggled();
        void onNetDisplayToggle();

    private:
        void onCanvasParentChange(QSFML::Canvas *oldParent, QSFML::Canvas *newParent) override;

        NeuronalNet::GeneticNet *m_geneticNet;
        std::vector<AI_Controller*> m_agents;
        QSFML::Components::KeyPressEvent *m_pauseToggle;
        QSFML::Components::KeyPressEvent *m_netDisplayToggle;


        size_t m_cyclesCounter;
        size_t m_maxCycles;
        bool m_paused;
        bool m_displayNets;
};
