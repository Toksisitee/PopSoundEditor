#include "Drums.h"

CDrums::CDrums()
{
    m_pBuffer = nullptr;
    m_nBufferLength = 0;
    m_Audio = nullptr;
}

void CDrums::FillTable()
{
    int32_t rows = this->GetEntries();
    int32_t columns = 6;

    m_TableWidget->clear();
    m_TableWidget->setRowCount(rows);
    m_TableWidget->setColumnCount(columns);

    m_TableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Index"));
    m_TableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset"));
    m_TableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("HeaderSize"));
    m_TableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("DataSize"));
    m_TableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Name"));
    m_TableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("SampleRate"));

    m_TableWidget->setColumnWidth(0, 15); // Index
    m_TableWidget->setColumnWidth(1, 50); // Offset
    m_TableWidget->setColumnWidth(2, 65); // HeaderSize
    m_TableWidget->setColumnWidth(3, 56); // DataSize
    m_TableWidget->setColumnWidth(4, 100); // Name
    m_TableWidget->setColumnWidth(5, 70); // SampleRate

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
    }
}

void CDrums::Create(const QString &file)
{


}

void CDrums::Export(uint32_t index)
{


}

void CDrums::Play(uint32_t index)
{
    auto entry = m_Bank.Entry[index].second;
    auto offset = m_Bank.Entry[index].first;

    QAudioFormat format;
    format.setSampleRate(entry.SampleRate);
    format.setChannelCount(2);
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
