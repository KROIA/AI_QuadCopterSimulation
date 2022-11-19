#pragma once

#include "QSFML_EditorWidget.h"

class QuadCopterFrame2D : public QSFML::Objects::CanvasObject
{
        class QuadCopterFrame2DPainter;
        friend QuadCopterFrame2DPainter;
    public:
        QuadCopterFrame2D(const std::string &name = "QuadCopterFrame2D",
                     CanvasObject *parent = nullptr);
        QuadCopterFrame2D(const QuadCopterFrame2D &other);
        ~QuadCopterFrame2D();
        CLONE_FUNC(QuadCopterFrame2D)

        void update() override;

        void setColor(const sf::Color &color);
        const sf::Color &getColor() const;

        void setOrigin(const sf::Vector2f &origin);
        const sf::Vector2f &getOrigin() const;

        void setRotation(float rad);

        sf::Vector2f getLeftDir() const;
        sf::Vector2f getUpDir() const;
        sf::Vector2f getLeftMotorMount() const;
        sf::Vector2f getRightMotorMount() const;
        sf::Vector2f getTopCenterPoint() const;

    protected:

    private:


        QuadCopterFrame2DPainter *m_painter;


        sf::Color m_color;

        sf::Vector2f m_up;
        sf::Vector2f m_origin;
        sf::Vector2f m_leftDir;

        float m_width;
        float m_height;
        float m_motorCenterDistance;
        sf::Vector2f m_BBL;
        sf::Vector2f m_BTL;
        sf::Vector2f m_BBR;
        sf::Vector2f m_BTR;




        class QuadCopterFrame2DPainter: public QSFML::Components::Drawable
        {
            public:
                QuadCopterFrame2DPainter(const std::string &name = "QuadCopterFrame2DPainter");
                QuadCopterFrame2DPainter(const QuadCopterFrame2DPainter &other);
                ~QuadCopterFrame2DPainter();

                void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

                QuadCopterFrame2D* m_copter;
        };
};

