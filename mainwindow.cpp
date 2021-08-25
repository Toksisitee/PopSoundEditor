#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Sound.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tableWidget);

    CSound sound;
    QString filename = "C:\\Users\\T\\AppData\\Roaming\\pop\\Sound\\Soundd2.sdt";
    sound.Load(filename);
    sound.FillTable(ui->tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{

}


void MainWindow::on_actionOpen_Sound_triggered()
{

}


void MainWindow::on_actionOpen_Drums_triggered()
{

}


void MainWindow::on_actionOpen_Music_triggered()
{

}

