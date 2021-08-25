#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "soundeditor.h"

CSoundEditor Editor;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Sound
void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Sound File", Editor.GetFilePath(), "Sound File (*.SDT)");
    if (!file.isEmpty())
    {
        Editor.WriteSetting("LastOpenPath", file);
        if (Editor.m_Sound.Load(file)) {
            Editor.SetFilePath(file);
            Editor.m_Sound.FillTable(ui->tableWidget);
        }
    }
}


// Drums
void MainWindow::on_actionOpen_2_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Drums File", Editor.GetFilePath(), "Drums File (*.SDT)");
    if (!file.isEmpty())
    {
        Editor.WriteSetting("LastOpenPath", file);
        if (Editor.m_Drums.Load(file)) {
            Editor.SetFilePath(file);
            Editor.m_Drums.FillTable(ui->tableWidget);
        }
    }
}


// Music
void MainWindow::on_actionOpen_3_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Music File", Editor.GetFilePath(), "Music File (*.SDT)");
    if (!file.isEmpty())
    {
        Editor.WriteSetting("LastOpenPath", file);
        if (Editor.m_Music.Load(file)) {
            Editor.SetFilePath(file);
            Editor.m_Music.FillTable(ui->tableWidget);
        }
    }
}

