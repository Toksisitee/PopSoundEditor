#ifndef MUSIC_H
#define MUSIC_H

#include "Common.h"

struct TbMusicEntry
{
    uint32_t    HeaderSize;
    uint32_t    DataSize;
    char        Name[8];
    uint32_t    Unknown1;
    uint32_t    Unknown2;
    uint16_t    SampleRate;
    uint32_t    Unknown3; // always 0?
    uint32_t    Unknown4; // used
    uint32_t    Unknown5; // always 0?
};


class CMusic : public CBank<TbMusicEntry>
{
public:
    CMusic();
    void    Create(const QString& filename) override;
    void    Export(uint32_t index) override;
    void    Play(uint32_t index) override;
    void    FillTable(QTableWidget* table) override;
    int32_t GetEntries() const override;
protected:
    void    Clear() override;
};

#endif // MUSIC_H
