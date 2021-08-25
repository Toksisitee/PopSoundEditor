#ifndef COMMON_H
#define COMMON_H

#include <QtCore/QtGlobal>
#include <QDataStream>
#include <QIODevice>
#include <QFile>
#include <QTableWidget>

enum BankType
{
    Sound,
    Drums,
    Music
};

template <typename T>
struct Bank {
    uint32_t Entries;
    QVector<QPair<uint32_t, T>> Entry;
};

template <typename T>
class CBank
{
public:
    virtual         ~CBank() {};
    struct Bank<T>  m_Bank;

    virtual bool    Load(const QString& filename)
    {
        m_Bank.Entries = 0;
        m_Bank.Entry.clear();
        if (m_pBuffer) {
            delete[] m_pBuffer;
        }

        QFile file(filename);
        if (file.open(QFile::ReadOnly))
        {
            file.seek(0);
            m_nBufferLength = file.size();
            m_pBuffer = new char[m_nBufferLength];
            file.read(m_pBuffer, file.size());
            file.close();

            if (m_pBuffer != NULL)
            {
                char* pBuffer = m_pBuffer;
                m_Bank.Entries = *(reinterpret_cast<uint32_t*>(pBuffer));
                if (m_Bank.Entries == 0)
                {
                    this->Clear();
                    return false;
                }

                pBuffer += sizeof(uint32_t);
                for (uint32_t i = 0; i < m_Bank.Entries; i++)
                {
                    auto offset = *(reinterpret_cast<uint32_t*>(pBuffer));
                    m_Bank.Entry.push_back(std::make_pair(offset, *(reinterpret_cast<T*>(&m_pBuffer[offset]))));
                    pBuffer += sizeof(uint32_t);
                }
                return true;
            }
            return false;
        }
        return false;
    }

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

// http://soundfile.sapp.org/doc/WaveFormat/
struct WAVE
{
    char		RIFF[4];        // RIFF Header      Magic header
    uint32_t    ChunkSize;      // RIFF Chunk Size
    char		WAVE[4];        // WAVE Header

    char		fmt[4];         // FMT header
    uint32_t	Subchunk1Size;  // Size of the fmt chunk
    uint16_t	AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t	NumChannels;    // Number of channels 1=Mono 2=Sterio
    uint32_t	SampleRate;     // Sampling Frequency in Hz
    uint32_t	ByteRate;       // bytes per second
    uint16_t	BlockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t	BitsPerSample;  // Number of bits per sample
    char		Subchunk2ID[4]; // "data"  string
    uint32_t	Subchunk2Size;  // Sampled data length
};

#endif // COMMON_H
