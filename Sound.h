#ifndef CSOUNDEDITOR_H
#define CSOUNDEDITOR_H

#include <QtCore/QtGlobal>
#include <QDataStream>
#include <QIODevice>
#include <QFile>
#include <QTableWidget>

template <typename T>
struct Bank {
    uint32_t Entries;
    QVector<QPair<uint32_t, T>> Entry;
};


// http://soundfile.sapp.org/doc/WaveFormat/
struct WAVE
{
    char		RIFF[4];		// RIFF Header      Magic header
    uint32_t    ChunkSize;		// RIFF Chunk Size
    char		WAVE[4];		// WAVE Header

    char		fmt[4];			// FMT header
    uint32_t	Subchunk1Size;	// Size of the fmt chunk
    uint16_t	AudioFormat;	// Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t	NumChannels;	// Number of channels 1=Mono 2=Sterio
    uint32_t	SampleRate;		// Sampling Frequency in Hz
    uint32_t	ByteRate;		// bytes per second
    uint16_t	BlockAlign;		// 2=16-bit mono, 4=16-bit stereo
    uint16_t	BitsPerSample;	// Number of bits per sample
    char		Subchunk2ID[4]; // "data"  string
    uint32_t	Subchunk2Size;	// Sampled data length
};

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

struct TbDrumEntry
{
    uint32_t    HeaderSize;
    uint32_t    DataSize;
    char        Name[16];
    uint16_t    SampleRate;
    uint8_t     BitsPerSample;
    char        Pad[11];
};

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

class CBank
{
public:
    virtual         ~CBank() {};
    virtual bool    Load(const QString& filename) = 0;
    virtual void    Create(const QString& filename) = 0;
    virtual void    Export(uint32_t index) = 0;
    virtual void    Play(uint32_t index) = 0;
    virtual void    FillTable(QTableWidget* table) = 0;
    virtual int32_t GetEntries() const = 0;
protected:
    virtual void    Clear() = 0;
    char*           m_pBuffer;
    uint32_t        m_nBufferLength;
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


#endif // CSOUNDEDITOR_H
