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



void MainWindow::on_actionEditor_triggered()
{
    const QString caption = QMessageBox::tr(
        "<h3>About %1</h3>"
        "<p>This program uses Qt version %2.</p>"
        ).arg(EDITOR_NAME).arg(QLatin1String(QT_VERSION_STR));

    const QString text = QMessageBox::tr(
        "<p>Copyright (C) Toksisitee 2021 <meypcm@gmail.com></p>"
        "<p>Github: <a href=\"http://github.com/Toksisitee/\">Toksisitee</a></p>"
        "<p><a href=\"https://github.com/Toksisitee/PopSoundEditor/\">Open Source Repository</a></p>"
        "<p>Version: %1 %2 %3</p>"
        ).arg(QStringLiteral(EDITOR_VERSION),
              QStringLiteral(EDITOR_DATE),
              QStringLiteral(EDITOR_BUILD));

    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About %1").arg(EDITOR_NAME));
    msgBox->setText(caption);
    msgBox->setInformativeText(text);

    QPixmap pm(QCoreApplication::applicationDirPath() + "\\editor.png");
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);

    msgBox->show();
}


void MainWindow::on_actionQt_triggered()
{
    QString translatedTextAboutQtCaption;
    translatedTextAboutQtCaption = QMessageBox::tr(
        "<h3>About Qt</h3>"
        "<p>This program uses Qt version %1.</p>"
        ).arg(QLatin1String(QT_VERSION_STR));
    //: Leave this text untranslated or include a verbatim copy of it below
    //: and note that it is the authoritative version in case of doubt.
    const QString translatedTextAboutQtText = QMessageBox::tr(
        "<p>Qt is a C++ toolkit for cross-platform application "
        "development.</p>"
        "<p>Qt provides single-source portability across all major desktop "
        "operating systems. It is also available for embedded Linux and other "
        "embedded and mobile operating systems.</p>"
        "<p>Qt is available under multiple licensing options designed "
        "to accommodate the needs of our various users.</p>"
        "<p>Qt licensed under our commercial license agreement is appropriate "
        "for development of proprietary/commercial software where you do not "
        "want to share any source code with third parties or otherwise cannot "
        "comply with the terms of GNU (L)GPL.</p>"
        "<p>Qt licensed under GNU (L)GPL is appropriate for the "
        "development of Qt&nbsp;applications provided you can comply with the terms "
        "and conditions of the respective licenses.</p>"
        "<p>Please see <a href=\"http://%2/\">%2</a> "
        "for an overview of Qt licensing.</p>"
        "<p>Copyright (C) %1 The Qt Company Ltd and other "
        "contributors.</p>"
        "<p>Qt and the Qt logo are trademarks of The Qt Company Ltd.</p>"
        "<p>Qt is The Qt Company Ltd product developed as an open source "
        "project. See <a href=\"http://%3/\">%3</a> for more information.</p>"
        ).arg(QStringLiteral("2021"),
              QStringLiteral("qt.io/licensing"),
              QStringLiteral("qt.io"));
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About Qt"));
    msgBox->setText(translatedTextAboutQtCaption);
    msgBox->setInformativeText(translatedTextAboutQtText);

    QPixmap pm(QLatin1String(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);

    msgBox->exec();
}

