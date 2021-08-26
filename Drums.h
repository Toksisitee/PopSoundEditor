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

class CDrums : public CBank<TbDrumEntry>
{
public:
    CDrums();
    void    Create(const QString& fileName) override;
    void    Export(uint32_t index) override;
    void    Play(uint32_t index) override;
    void    FillTable() override;
protected:

};

#endif // DRUMS_H
