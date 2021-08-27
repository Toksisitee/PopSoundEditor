#include "mainwindow.h"

#include "Editor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName(EDITOR_NAME);
    a.setOrganizationDomain("https://github.com/Toksisitee");
    a.setOrganizationName("Toksisitee");
    a.setApplicationName(EDITOR_NAME);
    a.setApplicationVersion("v1.0.0");
    MainWindow w;
    w.show();
    return a.exec();
}
