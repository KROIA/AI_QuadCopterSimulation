#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AI_trainer.h"

using namespace QSFML;
using namespace QSFML::Objects;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pidCopter = nullptr;
    m_pidSliderScale = 0.1;
    m_pidSliderScale_I = 0.0001;
    Components::DrawableVector::setInvertedYAxis(false);
   // setupAIModel();
    setupSimulation();


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::setupAIModel()
{
    CanvasSettings settings;
    //settings.timing.frameTime = 100;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
    m_aiModelCanvas = new Canvas(ui->ai_viewport,settings);

    DefaultEditor *defaultEditor = new DefaultEditor();

    m_aiModelCanvas->addObject(defaultEditor);
}*/
void MainWindow::setupSimulation()
{
//#define AI_CONTROLL
    CanvasSettings settings;
    //settings.timing.frameTime = 100;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
#ifdef AI_CONTROLL
    settings.timing.frameTime = 0;
#endif


    m_simulationCanvas = new Canvas(ui->simulationViewport,settings);
    ui->targetFrameTime_label->setText(QString::number(settings.timing.frameTime)+"ms");
    ui->targetFrameTimer_slider->setValue(settings.timing.frameTime);

    DefaultEditor *defaultEditor = new DefaultEditor("Editor",sf::Vector2f(2000,600));

    m_simulationCanvas->addObject(defaultEditor);

#ifdef AI_CONTROLL
    AI_Trainer *trainer = new AI_Trainer();
    m_simulationCanvas->addObject(trainer);
    connect(ui->toggleDisplay_pushButton, &QPushButton::clicked,
            trainer, &AI_Trainer::toggleDisplay);
#else
    //QuadCopter2D *copter = new QuadCopter2D();
    //m_simulationCanvas->addObject(copter);
    m_pidCopter = new PID_Controller();
    m_simulationCanvas->addObject(m_pidCopter);

    if(m_pidCopter)
    {
        PID *anglePID = m_pidCopter->getAnglePid();
        ui->angleP_slider->setValue((int)(anglePID->getPSetting() / m_pidSliderScale));
        ui->angleI_slider->setValue((int)(anglePID->getISetting() / m_pidSliderScale_I));
        ui->angleD_slider->setValue((int)(anglePID->getDSetting() / m_pidSliderScale));
        ui->angleD1_slider->setValue((int)(anglePID->getD1Setting() / m_pidSliderScale));

        PID *heightPID = m_pidCopter->getHeightPid();
        ui->heightP_slider->setValue((int)(heightPID->getPSetting() / m_pidSliderScale));
        ui->heightI_slider->setValue((int)(heightPID->getISetting() / m_pidSliderScale_I));
        ui->heightD_slider->setValue((int)(heightPID->getDSetting() / m_pidSliderScale));
        ui->heightD1_slider->setValue((int)(heightPID->getD1Setting() / m_pidSliderScale));
        //qDebug() << "PID "<<(int)(heightPID->getPSetting() / m_pidSliderScale);
    }
#endif
}





void MainWindow::on_targetFrameTimer_slider_valueChanged(int value)
{
    QSFML::CanvasSettings::Timing timing = m_simulationCanvas->getTiming();
    timing.frameTime = value;
    ui->targetFrameTime_label->setText(QString::number(value)+"ms");
    m_simulationCanvas->setTiming(timing);
}


void MainWindow::on_angleP_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setAnglePSetting((float)value*m_pidSliderScale);
    ui->angleP_label->setText(QString::number((float)value*m_pidSliderScale));
}
void MainWindow::on_angleI_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setAngleISetting((float)value*m_pidSliderScale_I);
    ui->angleI_label->setText(QString::number((float)value*m_pidSliderScale_I));
}
void MainWindow::on_angleD_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setAngleDSetting((float)value*m_pidSliderScale);
    ui->angleD_label->setText(QString::number((float)value*m_pidSliderScale));
}
void MainWindow::on_angleD1_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setAngleD1Setting((float)value*m_pidSliderScale);
    ui->angleD1_label->setText(QString::number((float)value*m_pidSliderScale));
}

void MainWindow::on_heightP_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setHeightPSetting((float)value*m_pidSliderScale);
    ui->heightP_label->setText(QString::number((float)value*m_pidSliderScale));
}
void MainWindow::on_heightI_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setHeightISetting((float)value*m_pidSliderScale_I);
    ui->heightI_label->setText(QString::number((float)value*m_pidSliderScale_I));
}
void MainWindow::on_heightD_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setHeightDSetting((float)value*m_pidSliderScale);
    ui->heightD_label->setText(QString::number((float)value*m_pidSliderScale));
}
void MainWindow::on_heightD1_slider_valueChanged(int value)
{
    if(!m_pidCopter) return;
    m_pidCopter->setHeightD1Setting((float)value*m_pidSliderScale);
    ui->heightD1_label->setText(QString::number((float)value*m_pidSliderScale));
}

void MainWindow::on_resetPID_pushButton_clicked()
{
    if(!m_pidCopter) return;
    m_pidCopter->resetPID();
}


void MainWindow::on_reset_pushButton_clicked()
{
    if(!m_pidCopter) return;
    m_pidCopter->reset();
}







