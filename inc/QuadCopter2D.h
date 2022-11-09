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

        void setPause(bool pause);

        void update() override;
        void checkCollision();

        bool getGroundWasHit() const;
        float getAngle();
        sf::Vector2f getUpVector();
        Force getAcceleration();
        Force getSpeed();
        float getHeight();
        sf::Vector2f getPosition();
        void setPosition(const Force &pos);
        void setSpeed(const Force &speed);


        void setMotorForce(float left, float right);
        void setPosition(const sf::Vector2f &pos);
        void setAngle(float angle);

    protected:

    private slots:
        void onLeftKeyFalling();
        void onRightKeyFalling();
        void onLeftKeyRising();
        void onRightKeyRising();

        void onNoiseKeyPressed();

    private:
        Force getCenteredForce();

        QuadCopter2DPainter *m_painter;
        ForcePainter *m_forceVec;
        ForcePainter *m_accelerationVec;
        ForcePainter *m_velocityVec;

        QSFML::Components::KeyPressEvent *m_leftKey;
        QSFML::Components::KeyPressEvent *m_rightKey;
        QSFML::Components::KeyPressEvent *m_noiseKey;

        QuadCopterFrame2D *m_frame;
        Motor2D *m_motorLeft;
        Motor2D *m_motorRight;

        Force m_acceleration;
        Force m_velocity;
        Force m_pos;
        float m_deltaT;
        float m_groundHeight;
        float m_sealingHeight;
        float m_leftWallPos;
        float m_rightWallPos;
        bool m_paused;
        bool m_groundWasHit;

        Force m_noiseForce;

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

