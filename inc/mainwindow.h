#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SFML_EditorWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        void setupAIModel();
        void setupSimulation();

        Ui::MainWindow *ui;
        QSFML::Canvas *m_aiModelCanvas;
        QSFML::Canvas *m_simulationCanvas;
};
#endif // MAINWINDOW_H
