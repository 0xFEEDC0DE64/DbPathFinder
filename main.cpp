#include <QApplication>
#include <qglobal.h>
#include <QDateTime>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(QDateTime::currentMSecsSinceEpoch());

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
