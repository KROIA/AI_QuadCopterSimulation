#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QuadCopter2D.h"
using namespace QSFML;
using namespace QSFML::Objects;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Components::DrawableVector::setInvertedYAxis(false);
    setupAIModel();
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
    m_simulationCanvas = new Canvas(ui->simulationViewport,settings);

    DefaultEditor *defaultEditor = new DefaultEditor();

    m_simulationCanvas->addObject(defaultEditor);

    QuadCopter2D *copter = new QuadCopter2D();
    //motor->setRotation(M_PI_2 * 0.5f);
    m_simulationCanvas->addObject(copter);
}

