#include "mainwindow.h"

#include <QApplication>
#include "neuronalNet.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*for(int i=0; i<100; ++i)
    {
        qDebug() << NeuronalNet::Net::getRandomValue(-0.2,0.1);
        //qDebug() << rand() % 30000;
    }*/
    return a.exec();
}
