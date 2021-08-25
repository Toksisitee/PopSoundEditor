#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("PopSoundEditor");
    a.setOrganizationDomain("https://github.com/Toksisitee");
    a.setOrganizationName("Toksisitee");
    a.setApplicationName("PopSoundEditor");
    a.setApplicationVersion("v1.0.0");
    MainWindow w;
    w.show();
    return a.exec();
}
