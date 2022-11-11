#pragma once

#include "SFML_EditorWidget.h"


class PID: public QSFML::Objects::CanvasObject
{
public:
    PID(const std::string &name = "PID",
              CanvasObject *parent = nullptr);
    PID(const PID &other);
    PID(float p, float i, float d, float d1 = 0);
    ~PID();
    CLONE_FUNC(PID)

    static void setDeltaT(float t);
    static float getDeltaT();

    void setPSetting(float p);
    float getPSetting() const;

    void setISetting(float i);
    float getISetting() const;
    void setIBoundry(float radius);
    float getIBoundry() const;

    void setDSetting(float d);
    float getDSetting() const;

    void setD1Setting(float d1);
    float getD1Setting() const;

    void set(float p, float i, float d, float d1 = 0);
    void reset();

    float calculate(float input);
    float getInput() const;
    float getOutput() const;

    float getP() const;
    float getI() const;
    float getD() const;
    float getD1() const;

    void setChartSize(const sf::Vector2f &size);
    void setChartPos(const sf::Vector2f &pos);
    void setChartPointCount(size_t maxPoints);

    void update() override;

private:
    void addPoint(QSFML::Objects::LineChart *chart,
                  float point);

    float m_pSetting;
    float m_iSetting;
    float m_dSetting;
    float m_d1Setting;
    float m_iBounrdy;

    float m_p;
    float m_i;
    float m_d;
    float m_d1;

    float m_input;
    //float m_lastInput;
    //float m_lastChange;
    float m_output;


    QSFML::Objects::LineChart *m_pChart;
    QSFML::Objects::LineChart *m_iChart;
    QSFML::Objects::LineChart *m_dChart;
    QSFML::Objects::LineChart *m_d1Chart;
    QSFML::Objects::LineChart *m_outChart;
    size_t m_maxChartPoints;

    static float m_deltaT;



};



