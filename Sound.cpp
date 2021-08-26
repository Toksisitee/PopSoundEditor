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

void CSound::Create(const QString& fileName)
{
    QVector<Wav2Entry> vec;

    QDir directory(QDir::currentPath() + "\\sounds\\");
    QStringList files = directory.entryList(QStringList() << "*.wav" << "*.WAV", QDir::Files);

    foreach(QString wavName, files)
    {
        bool isDigit = false;
        int removePos = wavName.indexOf('_');
        QStringRef tmp(&wavName, 0, removePos);
        int index = tmp.toInt(&isDigit, 10);

        if (isDigit)
        {
            Wav2Entry entry;
            entry.Index = index;

            QString fullName = wavName;
            wavName.remove(0, removePos + 1);
            wavName.chop(4);
            strncpy(entry.Name, wavName.toLatin1().data(), sizeof(entry.Name));

            QString filePath = QDir::currentPath() + "\\sounds\\" + fullName;
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly))
            {
                char* pBuffer = new char[file.size()];
                file.seek(0);
                file.read(pBuffer, file.size());
                file.close();

                memcpy(&entry.Wav, &pBuffer[0], sizeof(WAVE));
                entry.Data = new char[entry.Wav.Subchunk2Size];
                memcpy(&entry.Data[0], &pBuffer[sizeof(WAVE)], entry.Wav.Subchunk2Size);
                delete[] pBuffer;
            }
            else
            {
                QErrorMessage msg;
                msg.showMessage(QString("Can not open WAV file!<br>%1").arg(filePath));
                msg.exec();
                return;
            }

            vec.push_back(entry);
        }
    }

    if (vec.isEmpty())
    {
        QErrorMessage msg;
        msg.showMessage("Could not find any WAV files inside the sounds directory!");
        msg.exec();
        return;
    }

    qSort(vec.begin(), vec.end(), [](
        const Wav2Entry& fsound,
        const Wav2Entry& ssound)
    {
        return (fsound.Index < ssound.Index);
    });

    auto it = std::unique(vec.begin(), vec.end(), [](
        const Wav2Entry& fsound,
        const Wav2Entry& ssound)
    {
        return fsound.Index == ssound.Index;
    });

    vec.erase(it, vec.end());
    auto numSounds = vec.size();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        auto data_offset = 4 + (numSounds * 4);
        file.write(reinterpret_cast<const char*>(&numSounds), 4);

        // This data is lost when extracting.
        uint32_t HeaderSize = 40;
        uint32_t Unknown = 0;
        uint32_t LoopStart, LoopEnd;

        for (int32_t i = 0; i < vec.size(); i++)
        {
            file.write(reinterpret_cast<const char*>(&data_offset), 4);
            data_offset += HeaderSize + vec[i].Wav.Subchunk2Size;
        }

        for (int32_t i = 0; i < vec.size(); i++)
        {
            // Hardcoded values
            LoopStart = 0;
            LoopEnd = 0;

            switch (i)
            {
            case 2:
                LoopStart = 24058;
                LoopEnd = 70386;
                break;
            case 84:
                LoopStart = 54922;
                LoopEnd = 315666;
                break;
            case 392:
                LoopStart = 15490;
                LoopEnd = 76546;
                break;
            case 439:
                LoopStart = 17210;
                LoopEnd = 91250;
                break;
            }

            vec[i].Wav.SampleRate *= 2;
            file.write(reinterpret_cast<const char*>(&HeaderSize), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.Subchunk2Size), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Name), 16);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.SampleRate), 2);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.BitsPerSample), 1);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.NumChannels), 1);
            file.write(reinterpret_cast<const char*>(&Unknown), 4);
            file.write(reinterpret_cast<const char*>(&LoopStart), 4);
            file.write(reinterpret_cast<const char*>(&LoopEnd), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Data[0]), vec[i].Wav.Subchunk2Size);
        }

        foreach (auto ptr, vec) {
            delete[] ptr.Data;
        }

        QMessageBox msgBox;
        msgBox.setText(QString("Successfully created SDT bank containing %1 sounds.").arg(numSounds));
        msgBox.exec();
        file.close();
    }
}

void CSound::Export(uint32_t index)
{
    auto entry = m_Bank.Entry[index].second;
    auto offset = m_Bank.Entry[index].first;

    QFile file(QDir::currentPath() + "\\sounds\\" + QString::number(index) + "_" + entry.Name + ".wav");
    if (file.open(QIODevice::WriteOnly))
    {
        WAVE wav;
        strncpy(wav.RIFF, "RIFF", 4);
        strncpy(wav.WAVE, "WAVE", 4);
        strncpy(wav.fmt, "fmt ", 4);
        strncpy(wav.Subchunk2ID, "data", 4);
        wav.ChunkSize = sizeof(WAVE) + entry.DataSize - 8;
        wav.Subchunk1Size = 16;
        wav.AudioFormat = 1;
        wav.NumChannels = entry.NumChannels;
        wav.SampleRate = entry.SampleRate / 2;
        wav.ByteRate = (entry.SampleRate * entry.NumChannels * entry.BitsPerSample) / 8;
        wav.BlockAlign = (entry.NumChannels * entry.BitsPerSample) / 8;
        wav.BitsPerSample = entry.BitsPerSample;
        wav.Subchunk2Size = entry.DataSize;

        file.write(reinterpret_cast<const char*>(&wav), sizeof(WAVE));
        file.write(reinterpret_cast<const char*>(&m_pBuffer[offset + entry.HeaderSize]), entry.DataSize);
        file.close();
    }
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
