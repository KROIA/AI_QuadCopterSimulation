#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSFML_EditorWidget.h"
#include "PID_Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_targetFrameTimer_slider_valueChanged(int value);
        void on_angleP_slider_valueChanged(int value);
        void on_angleI_slider_valueChanged(int value);
        void on_angleD_slider_valueChanged(int value);
        void on_heightP_slider_valueChanged(int value);
        void on_heightI_slider_valueChanged(int value);
        void on_heightD_slider_valueChanged(int value);

        void on_resetPID_pushButton_clicked();

        void on_reset_pushButton_clicked();

        void on_angleD1_slider_valueChanged(int value);

        void on_heightD1_slider_valueChanged(int value);

        void on_horizontalP_slider_valueChanged(int value);

        void on_horizontalI_slider_valueChanged(int value);

        void on_horizontalD_slider_valueChanged(int value);

        void on_horizontalD1_slider_valueChanged(int value);

    private:
        //void setupAIModel();
        void setupSimulation();

        Ui::MainWindow *ui;
        //QSFML::Canvas *m_aiModelCanvas;
        QSFML::Canvas *m_simulationCanvas;

        PID_Controller *m_pidCopter;
        float m_pidSliderScale;
        float m_pidSliderScale_I;
};
#endif // MAINWINDOW_H
