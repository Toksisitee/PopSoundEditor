#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Editor.h"
#include "soundeditor.h"

CSoundEditor Editor;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tableWidget);
    Editor.SetQTableWidgetPtr(ui->tableWidget);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowTitle(QString("v%1.%2.%3 %4").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH).arg(EDITOR_BUILD));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Open Sound Bank
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Sound File", Editor.GetFilePath(), "Sound File (*.SDT)");
    if (!fileName.isEmpty())
    {
        Editor.WriteSetting("LastOpenPath", fileName);
        if (Editor.m_Sound.Load(fileName)) {
            Editor.SetFilePath(fileName);
            Editor.m_Sound.FillTable();
            Editor.m_Type = BankType::Sound;
            this->setWindowTitle(QString("%1 - %2").arg(fileName).arg(EDITOR_BUILD));
        }
    }
}


// Open Drums Bank
void MainWindow::on_actionOpen_2_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Drums File", Editor.GetFilePath(), "Drums File (*.SDT)");
    if (!fileName.isEmpty())
    {
        Editor.WriteSetting("LastOpenPath", fileName);
        if (Editor.m_Drums.Load(fileName)) {
            Editor.SetFilePath(fileName);
            Editor.m_Drums.FillTable();
            Editor.m_Type = BankType::Drums;
            this->setWindowTitle(QString("%1 - %2").arg(fileName).arg(EDITOR_BUILD));
        }
    }
}


// Open Music Bank
void MainWindow::on_actionOpen_3_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Music File", Editor.GetFilePath(), "Music File (*.SDT)");
    if (!fileName.isEmpty())
    {
        Editor.WriteSetting("LastOpenPath", fileName);
        if (Editor.m_Music.Load(fileName)) {
            Editor.SetFilePath(fileName);
            Editor.m_Music.FillTable();
            Editor.m_Type = BankType::Music;
            this->setWindowTitle(QString("%1 - %2").arg(fileName).arg(EDITOR_BUILD));
        }
    }
}

// Play Sound
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

// Extract All Sounds
void MainWindow::on_actionExtract_triggered()
{
    QErrorMessage msg;
    bool errorBank = false;

    switch (Editor.m_Type)
    {
    case Sound:
        if (Editor.m_Sound.GetEntries() == 0)
            errorBank = true;

        for (int i = 0; i < Editor.m_Sound.GetEntries(); i++)
            Editor.m_Sound.Export(i);
        break;
    case Drums:
        if (Editor.m_Drums.GetEntries() == 0)
            errorBank = true;

        for (int i = 0; i < Editor.m_Drums.GetEntries(); i++)
            Editor.m_Drums.Export(i);
        break;
    case Music:
        if (Editor.m_Music.GetEntries() == 0)
            errorBank = true;

        for (int i = 0; i < Editor.m_Music.GetEntries(); i++)
            Editor.m_Music.Export(i);
        break;
    default:
        msg.showMessage("Please open a SDT bank first!");
        msg.exec();
        break;
    }

    if (errorBank)
    {
        msg.showMessage("Bank does not contain any sound files!");
        msg.exec();
    }
}

// Create Sounds Bank
void MainWindow::on_actionCreate_Bank_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save SDT Bank", Editor.GetFilePath(), "SDT Bank (*.SDT)");
    if (!fileName.isEmpty())
    {
       Editor.m_Sound.Create(fileName);
    }
}

// Create Drums Bank
void MainWindow::on_actionCreate_Bank_2_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save SDT Bank", Editor.GetFilePath(), "SDT Bank (*.SDT)");
    if (!fileName.isEmpty())
    {
       Editor.m_Drums.Create(fileName);
    }
}

// Create Music Bank
void MainWindow::on_actionCreate_Bank_3_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save SDT Bank", Editor.GetFilePath(), "SDT Bank (*.SDT)");
    if (!fileName.isEmpty())
    {
       Editor.m_Music.Create(fileName);
    }
}
