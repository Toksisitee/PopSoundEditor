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
    Editor.SetQTableWidgetPtr(ui->tableWidget);
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
            Editor.m_Sound.FillTable();
            Editor.m_Type = BankType::Sound;
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
            Editor.m_Drums.FillTable();
            Editor.m_Type = BankType::Drums;
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
            Editor.m_Music.FillTable();
            Editor.m_Type = BankType::Music;
        }
    }
}


void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    QErrorMessage msg;

    switch (Editor.m_Type)
    {
    case Sound:
        Editor.m_Sound.Play(index.row());
        break;
    case Drums:
        Editor.m_Drums.Play(index.row());
        break;
    case Music:
        Editor.m_Music.Play(index.row());
        break;
    default:
        msg.showMessage("Unknown SDT Bank type!");
        msg.exec();
        break;

    }
}

