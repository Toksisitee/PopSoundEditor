#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>

#include "Sound.h"
#include "Drums.h"
#include "Music.h"

CSound g_Sound;
CDrums g_Drums;
CMusic g_Music;
QString g_strLastPath;
QSettings g_Settings("editor.ini", QSettings::IniFormat);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tableWidget);

    //QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE\\Bullfrog Productions Ltd\\Populous: The Beginning", QSettings::NativeFormat);
    //g_strLastPath = registry.value("InstallPath", qApp->applicationDirPath()).toString();

    g_strLastPath = g_Settings.value("LastOpenPath", qApp->applicationDirPath()).toString();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Sound
void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Sound File", g_strLastPath, "Sound File (*.SDT)");
    if (!file.isEmpty())
    {
        g_Settings.setValue("LastOpenPath", file);
        g_strLastPath = file;
        g_Sound.Load(file);
        g_Sound.FillTable(ui->tableWidget);
    }
}


// Drums
void MainWindow::on_actionOpen_2_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Drums File", g_strLastPath, "Drums File (*.SDT)");
    if (!file.isEmpty())
    {
        g_Settings.setValue("LastOpenPath", file);
        g_strLastPath = file;
        g_Drums.Load(file);
        g_Drums.FillTable(ui->tableWidget);
    }
}


// Music
void MainWindow::on_actionOpen_3_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Music File", g_strLastPath, "Music File (*.SDT)");
    if (!file.isEmpty())
    {
        g_Settings.setValue("LastOpenPath", file);
        g_strLastPath = file;
        g_Music.Load(file);
        g_Music.FillTable(ui->tableWidget);
    }
}

