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
#define AI_CONTROLL
    CanvasSettings settings;
    //settings.timing.frameTime = 100;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
#ifdef AI_CONTROLL
    settings.timing.frameTime = 0;
#endif

    ui->targetFrameTime_label->setText(QString::number(settings.timing.frameTime)+"ms");
    ui->targetFrameTimer_slider->setValue(settings.timing.frameTime);
    m_simulationCanvas = new Canvas(ui->simulationViewport,settings);

    DefaultEditor *defaultEditor = new DefaultEditor("Editor",sf::Vector2f(2000,600));

    m_simulationCanvas->addObject(defaultEditor);

#ifdef AI_CONTROLL
    AI_Trainer *trainer = new AI_Trainer();
    m_simulationCanvas->addObject(trainer);
    connect(ui->toggleDisplay_pushButton, &QPushButton::clicked,
            trainer, &AI_Trainer::toggleDisplay);
#else
    QuadCopter2D *copter = new QuadCopter2D();
    m_simulationCanvas->addObject(copter);
#endif
}





void MainWindow::on_targetFrameTimer_slider_valueChanged(int value)
{
    QSFML::CanvasSettings::Timing timing = m_simulationCanvas->getTiming();
    timing.frameTime = value;
    ui->targetFrameTime_label->setText(QString::number(value)+"ms");
    m_simulationCanvas->setTiming(timing);
}

