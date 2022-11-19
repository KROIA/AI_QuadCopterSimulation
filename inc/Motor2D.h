#pragma once

#include "QSFML_EditorWidget.h"
#include "Force.h"

class Motor2D : public QSFML::Objects::CanvasObject
{
        class Motor2DPainter;
        friend Motor2DPainter;
    public:
        Motor2D(const std::string &name = "Motor2D",
                     CanvasObject *parent = nullptr);
        Motor2D(const Motor2D &other);
        ~Motor2D();
        CLONE_FUNC(Motor2D)

        void update() override;

        void setThrust(float thrust);
        float getThrust() const;
        Force getThrustVector() const;

        void setForceDirection(const sf::Vector2f &dir);
        const sf::Vector2f &getForceDirection() const;

        void setColor(const sf::Color &color);
        const sf::Color &getColor() const;

        void setOrigin(const sf::Vector2f &origin);
        const sf::Vector2f &getOrigin() const;

        void setRotation(float rad);

    protected:

    private:
        Motor2DPainter *m_painter;
        ForcePainter *m_forceVec;

        float m_thrust;
        sf::Vector2f m_forceDirection;
        sf::Color m_color;
        sf::Vector2f m_origin;


        float m_width;
        float m_height;
        sf::Vector2f m_BBL;
        sf::Vector2f m_BTL;
        sf::Vector2f m_BBR;
        sf::Vector2f m_BTR;


        class Motor2DPainter: public QSFML::Components::Drawable
        {
            public:
                Motor2DPainter(const std::string &name = "Motor2DPainter");
                Motor2DPainter(const Motor2DPainter &other);
                ~Motor2DPainter();

                void setMotor(Motor2D *m);
                void updateColor();
                inline void updatePos()
                {
                    if(!m_copter)
                        return;

                    m_body[0].position = m_copter->m_BBL;
                    m_body[1].position = m_copter->m_BBR;
                    m_body[2].position = m_copter->m_BTR;
                    m_body[3].position = m_copter->m_BTL;
                    m_body[4].position = m_copter->m_BBL;
                }

                void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

            private:
                Motor2D* m_copter;
                sf::Vertex m_body[5];
        };
};

