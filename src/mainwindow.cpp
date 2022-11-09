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

void MainWindow::setupAIModel()
{
    CanvasSettings settings;
    //settings.timing.frameTime = 100;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
    m_aiModelCanvas = new Canvas(ui->ai_viewport,settings);

    DefaultEditor *defaultEditor = new DefaultEditor();

    m_aiModelCanvas->addObject(defaultEditor);
}
void MainWindow::setupSimulation()
{
    CanvasSettings settings;
    //settings.timing.frameTime = 100;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300,100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0;
    m_simulationCanvas = new Canvas(ui->simulationViewport,settings);

    DefaultEditor *defaultEditor = new DefaultEditor("Editor",sf::Vector2f(2000,600));

    m_simulationCanvas->addObject(defaultEditor);

    AI_Trainer *trainer = new AI_Trainer();
    //motor->setRotation(M_PI_2 * 0.5f);
    m_simulationCanvas->addObject(trainer);
}

