#ifndef DRUMS_H
#define DRUMS_H

#include "Common.h"

struct TbDrumEntry
{
    uint32_t    HeaderSize;
    uint32_t    DataSize;
    char        Name[16];
    uint16_t    SampleRate;
    uint8_t     BitsPerSample;
    char        Pad[11];
};

class CDrums : public CBank
{
public:
    CDrums();
    Bank<TbDrumEntry> m_Bank;
    bool    Load(const QString& filename);
    void    Create(const QString& filename);
    void    Export(uint32_t index);
    void    Play(uint32_t index);
    void    FillTable(QTableWidget* table);
    int32_t GetEntries() const;
protected:
    void    Clear();
};

#endif // DRUMS_H
