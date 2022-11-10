#pragma once

class SimulationSettings
{
public:
    static void setDeltaT(float deltaT);
    static float getDeltaT();
private:
    static float m_deltaT;
};
