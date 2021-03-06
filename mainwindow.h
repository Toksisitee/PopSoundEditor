#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionOpen_2_triggered();

    void on_actionOpen_3_triggered();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_actionExtract_triggered();

    void on_actionCreate_Bank_triggered();

    void on_actionCreate_Bank_3_triggered();

    void on_actionCreate_Bank_2_triggered();

    void on_actionEditor_triggered();

    void on_actionQt_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
