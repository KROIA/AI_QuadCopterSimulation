#include "SimulationSettings.h"
#include "PID.h"

float SimulationSettings::m_deltaT = 0.1;


void SimulationSettings::setDeltaT(float deltaT)
{
    m_deltaT = deltaT;
    if(m_deltaT <= 0)
        m_deltaT = 0.000001;
    PID::setDeltaT(m_deltaT);
}
float SimulationSettings::getDeltaT()
{
    return m_deltaT;
}
