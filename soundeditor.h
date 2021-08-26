#ifndef SOUNDEDITOR_H
#define SOUNDEDITOR_H

#include <QSettings>

#include "Sound.h"
#include "Drums.h"
#include "Music.h"

class CSoundEditor
{
public:
    CSoundEditor();
    ~CSoundEditor();

    CSound      m_Sound;
    CDrums      m_Drums;
    CMusic      m_Music;
    void        WriteSetting(QString key, QString val);
    void        SetFilePath(QString& path);
    QString     GetFilePath() const;
    void        SetQTableWidgetPtr(QTableWidget* const table);

private:
    QString     m_strFilePath;
    QSettings*  m_Settings;
};


#endif // SOUNDEDITOR_H
