#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

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

    void on_actionOpen_Sound_triggered();

    void on_actionOpen_Drums_triggered();

    void on_actionOpen_Music_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
