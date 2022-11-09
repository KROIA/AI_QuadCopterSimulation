#pragma once

#include "SFML_EditorWidget.h"

class Force
{
    public:
        Force();
        Force(const sf::Vector2f &pos,
              const sf::Vector2f &force,
              float torque);
        Force(const Force &other);

        const Force &operator=(const Force &other);
        const Force operator+(const Force &other);
        const Force operator-(const Force &other);
        const Force &operator+=(const Force &other);
        const Force &operator-=(const Force &other);
        const Force operator*(float scalar);
        const Force &operator*=(float scalar);

        void setActingPoint(const sf::Vector2f &pos);
        const sf::Vector2f &getActingPoint() const;

        void setForceVector(const sf::Vector2f &force);
        const sf::Vector2f &getForceVector() const;
        void setForce(float force);
        float getForce() const;

        void setTorque(float torque);
        float getTorque() const;
        float getTorque(const sf::Vector2f &onPos) const;



        //Force getOnPosition(const sf::Vector2f &pos);

        static Force getSum(const std::vector<Force> &forcefield,
                            const sf::Vector2f &onPos);

        //float getGlobalTorque() const;
    private:


        sf::Vector2f m_actingPoint;
        sf::Vector2f m_force;
        float m_torque;  // + is counter clockwise

};

class ForcePainter: public QSFML::Components::Drawable
{
    public:
        ForcePainter(const std::string &name = "ForcePainter");
        ForcePainter(const ForcePainter &other);
        CLONE_FUNC(ForcePainter)

        void setColor(const sf::Color &color);
        void setScale(float scale);
        void setForce(const Force &force);
        void draw(sf::RenderTarget& target,
                  sf::RenderStates states) const override;

    private:
        float m_scale;
        Force m_force;
        sf::Color m_color;
        QSFML::Components::DrawableVector m_vec;

};
