//$Id$

#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QTimer>

#include "common.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(QString(MAINPATH) + "splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    //splash.raise();

    MainWindow mainwindow;

    QTimer::singleShot(750, &splash, SLOT(close()));
    QTimer::singleShot(400, &mainwindow, SLOT(show()));

    //splash.finish(&mainwindow);

    return a.exec();




/*    QApplication a(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();
    return a.exec();*/
}
