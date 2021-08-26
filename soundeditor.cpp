#include "soundeditor.h"
#include <QApplication>

CSoundEditor::CSoundEditor()
{
    m_Settings = new QSettings("editor.ini", QSettings::IniFormat);

    //QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE\\Bullfrog Productions Ltd\\Populous: The Beginning", QSettings::NativeFormat);
    //path = registry.value("InstallPath", qApp->applicationDirPath()).toString();
    QString path = m_Settings->value("LastOpenPath", qApp->applicationDirPath()).toString();
    this->SetFilePath(path);
}

CSoundEditor::~CSoundEditor()
{
    delete m_Settings;
}

void CSoundEditor::WriteSetting(QString key, QString val)
{
    m_Settings->setValue(key, val);
}

void CSoundEditor::SetFilePath(QString &path)
{
    m_strFilePath = path;
}

QString CSoundEditor::GetFilePath() const
{
    return m_strFilePath;
}

void CSoundEditor::SetQTableWidgetPtr(QTableWidget* const table)
{
    m_Sound.SetQTableWidgetPtr(table);
    m_Drums.SetQTableWidgetPtr(table);
    m_Music.SetQTableWidgetPtr(table);
}
