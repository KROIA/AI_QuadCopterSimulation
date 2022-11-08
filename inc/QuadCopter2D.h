#pragma once

#include "SFML_EditorWidget.h"
#include "Motor2D.h"
#include "QuadCopterFrame2D.h"


class QuadCopter2D: public QObject, public QSFML::Objects::CanvasObject
{
        Q_OBJECT
        class QuadCopter2DPainter;
        friend QuadCopter2DPainter;
    public:
        QuadCopter2D(const std::string &name = "QuadCopter2D",
                     CanvasObject *parent = nullptr);
        QuadCopter2D(const QuadCopter2D &other);
        ~QuadCopter2D();
        CLONE_FUNC(QuadCopter2D)

        void update() override;

    protected:

    private slots:
        void onLeftKeyFalling();
        void onRightKeyFalling();
        void onLeftKeyRising();
        void onRightKeyRising();

    private:
        Force getCenteredForce();

        QuadCopter2DPainter *m_painter;
        ForcePainter *m_forceVec;

        QSFML::Components::KeyPressEvent *m_leftKey;
        QSFML::Components::KeyPressEvent *m_rightKey;


        QuadCopterFrame2D *m_frame;
        Motor2D *m_motorLeft;
        Motor2D *m_motorRight;

        Force m_acceleration;
        Force m_velocity;
        Force m_pos;
        float m_deltaT;

        class QuadCopter2DPainter: public QSFML::Components::Drawable
        {
            public:
                QuadCopter2DPainter(const std::string &name = "QuadCopter2DPainter");
                QuadCopter2DPainter(const QuadCopter2DPainter &other);
                ~QuadCopter2DPainter();

                void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

                QuadCopter2D* m_copter;
        };
};

