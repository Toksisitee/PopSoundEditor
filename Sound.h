#ifndef CSOUND_H
#define CSOUND_H

#include "Common.h"

struct TbSoundEntry
{
    uint32_t    HeaderSize;
    uint32_t    DataSize;
    char        Name[16];
    // note that for music entries SampleRate (44.100) - 15000 seems to be ok (at least in demo)
    uint16_t    SampleRate; // 22050, normal 44100, but it's SampleRate / 2
    uint8_t     BitsPerSample; // 16
    uint8_t     NumChannels;
    uint32_t    Unknown;
    uint32_t    LoopStart;
    uint32_t    LoopEnd;
};

class CSound : public CBank
{
public:
    CSound();
    Bank<TbSoundEntry> m_Bank;
    bool    Load(const QString& filename);
    void    Create(const QString& filename);
    void    Export(uint32_t index);
    void    Play(uint32_t index);
    void    FillTable(QTableWidget* table);
    int32_t GetEntries() const;
protected:
    void    Clear();
};


#endif // CSOUND_H
