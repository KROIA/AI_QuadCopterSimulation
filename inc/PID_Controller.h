#pragma once

#include "SFML_EditorWidget.h"
#include "QuadCopter2D.h"
#include "PID.h"

class PID_Controller: public QObject, public QSFML::Objects::CanvasObject
{
    Q_OBJECT
public:
    PID_Controller(const std::string &name = "PID_Controller",
                  CanvasObject *parent = nullptr);
    PID_Controller(const PID_Controller &other);
    ~PID_Controller();
    CLONE_FUNC(PID_Controller);

    //PID &getLeftPid();
    //PID &getRightPid();

    PID *getAnglePid();
    PID *getHeightPid();

    void reset();
    void update() override;

    float getAngleError() const;
    float getHeightError() const;

public slots:
    void resetPID();
    void setAnglePSetting(float p);
    void setAngleISetting(float i);
    void setAngleDSetting(float d);
    void setAngleD1Setting(float d1);

    void setHeightPSetting(float p);
    void setHeightISetting(float i);
    void setHeightDSetting(float d);
    void setHeightD1Setting(float d1);
private:
    float getBounded(float value, float min=-1, float max=1);

   // PID m_pidLeft;
   // PID m_pidRight;

    QSFML::Objects::LineChart *m_heightPIDChart;
    QSFML::Objects::LineChart *m_anglePIDChart;

    PID *m_anglePID;
    PID *m_heightPID;
    QuadCopter2D *m_copter;
    sf::Vector2f m_startPos;
    bool m_paused;

    float m_targetHeight;
    float m_targetAngle;
};
