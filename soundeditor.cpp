/*
    Copyright © 2021 Toksisitee <meypcm@gmail.com>
    PopSoundEditor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    PopSpriteEditor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#include "soundeditor.h"
#include <QApplication>

CSoundEditor::CSoundEditor()
{
    m_Type = BankType::Unknown;
    m_Settings = new QSettings("editor.ini", QSettings::IniFormat);
    QDir().mkdir("sounds");
    QDir().mkdir("drums");
    QDir().mkdir("music");
}

void CSoundEditor::Initialize()
{
    m_strEditorPath = qApp->applicationDirPath();

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
