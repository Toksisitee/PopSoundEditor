#include "Sound.h"

CSound::CSound()
{
    m_pBuffer = nullptr;
    m_nBufferLength = 0;
    m_Audio = nullptr;
}

void CSound::FillTable()
{
    int32_t rows = this->GetEntries();
    int32_t columns = 11;

    m_TableWidget->clear();
    m_TableWidget->setRowCount(rows);
    m_TableWidget->setColumnCount(columns);

    m_TableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Index"));
    m_TableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset"));
    m_TableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("HeaderSize"));
    m_TableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("DataSize"));
    m_TableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Name"));
    m_TableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("SampleRate"));
    m_TableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("BitsPerSample"));
    m_TableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("NumChannels"));
    m_TableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Unknown"));
    m_TableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("LoopStart"));
    m_TableWidget->setHorizontalHeaderItem(10, new QTableWidgetItem("LoopEnd"));

    m_TableWidget->setColumnWidth(0, 15); // Index
    m_TableWidget->setColumnWidth(1, 50); // Offset
    m_TableWidget->setColumnWidth(2, 65); // HeaderSize
    m_TableWidget->setColumnWidth(3, 56); // DataSize
    m_TableWidget->setColumnWidth(4, 100); // Name
    m_TableWidget->setColumnWidth(5, 70); // SampleRate
    m_TableWidget->setColumnWidth(6, 85); // BitsPerSample
    m_TableWidget->setColumnWidth(7, 83); // NumChannels
    m_TableWidget->setColumnWidth(8, 60); // Unknown
    m_TableWidget->setColumnWidth(9, 60); // LoopStart
    m_TableWidget->setColumnWidth(10, 60); // LoopEnd

    for (int i = 0; i < this->GetEntries(); i++)
    {
        auto entry = m_Bank.Entry[i].second;
        m_TableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        QString offset = QString::asprintf("%02X", m_Bank.Entry[i].first);
        m_TableWidget->setItem(i, 1, new QTableWidgetItem(offset));
        m_TableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(entry.HeaderSize)));
        m_TableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(entry.DataSize)));
        m_TableWidget->setItem(i, 4, new QTableWidgetItem(entry.Name));
        m_TableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(entry.SampleRate)));
        m_TableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(entry.BitsPerSample)));
        m_TableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(entry.NumChannels)));
        m_TableWidget->setItem(i, 8, new QTableWidgetItem(QString::number(entry.Unknown)));
        m_TableWidget->setItem(i, 9, new QTableWidgetItem(QString::number(entry.LoopStart)));
        m_TableWidget->setItem(i, 10, new QTableWidgetItem(QString::number(entry.LoopEnd)));
    }
}

void CSound::Create(const QString &file)
{

}

void CSound::Export(uint32_t index)
{

}

void CSound::Play(uint32_t index)
{
//  auto sampleRate = m_TableWidget->item(index, 5)->text().toInt();
//  auto bitsPerSample = m_TableWidget->item(index, 6)->text().toInt();
//  auto numChannels = m_TableWidget->item(index, 7)->text().toInt();

    auto entry = m_Bank.Entry[index].second;
    auto offset = m_Bank.Entry[index].first;

    QAudioFormat format;
    format.setSampleRate(entry.SampleRate / 2);
    format.setChannelCount(entry.NumChannels);
    format.setSampleSize(entry.BitsPerSample);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        QErrorMessage msg;
        msg.showMessage("Audio format not supported!");
        msg.exec();
        return;
    }

    QBuffer* qBuffer = new QBuffer;
    char* buffer = new char[entry.DataSize];
    memcpy(&buffer[0], &m_pBuffer[offset + entry.HeaderSize], entry.DataSize);
    qBuffer->open(QIODevice::ReadWrite);
    qBuffer->seek(0);
    qBuffer->write(buffer, entry.DataSize);
    qBuffer->seek(0);
    delete[] buffer;

    if (m_Audio) delete m_Audio;
    m_Audio = new QAudioOutput(format);

    QObject::connect(m_Audio, &QAudioOutput::stateChanged, [this, qBuffer](QAudio::State state)
    {
        switch (state)
        {
            case QAudio::IdleState:
            case QAudio::StoppedState:
                delete m_Audio;
                delete qBuffer;
                m_Audio = nullptr;
                break;
            default:
                break;
        }
    });

    m_Audio->start(qBuffer);
}
