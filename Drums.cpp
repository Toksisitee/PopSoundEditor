/*
    Copyright © 2021 Toksisitee <meypcm@gmail.com>
    PopSoundEditor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    PopSpriteEditor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "Common.h"


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

void CDrums::Create(const QString &fileName)
{
    QVector<Wav2Entry> vec;

    QDir directory(QDir::currentPath() + "\\drums\\");
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

            QString filePath = QDir::currentPath() + "\\drums\\" + fullName;
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
        msg.showMessage("Could not find any WAV files inside the drums directory!");
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
        char szPad[11] = { 0 };

        for (int32_t i = 0; i < vec.size(); i++)
        {
            file.write(reinterpret_cast<const char*>(&data_offset), 4);
            data_offset += HeaderSize + vec[i].Wav.Subchunk2Size;
        }

        for (int32_t i = 0; i < vec.size(); i++)
        {
            file.write(reinterpret_cast<const char*>(&HeaderSize), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.Subchunk2Size), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Name), 16);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.SampleRate), 2);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.BitsPerSample), 1);
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.NumChannels), 1);
            file.write(reinterpret_cast<const char*>(&szPad), sizeof(szPad));
            file.write(reinterpret_cast<const char*>(&vec[i].Wav.NumChannels), 1);
            file.write(reinterpret_cast<const char*>(&vec[i].Data[0]), vec[i].Wav.Subchunk2Size);
        }

        foreach (auto ptr, vec) {
            delete[] ptr.Data;
        }

        QMessageBox msgBox;
        msgBox.setText(QString("Successfully created Drums SDT bank containing %1 sounds.").arg(numSounds));
        msgBox.exec();
        file.close();
    }
}

void CDrums::Export(uint32_t index)
{
    auto entry = m_Bank.Entry[index].second;
    auto offset = m_Bank.Entry[index].first;
    const auto numChannels = 2;

    QFile file(QDir::currentPath() + "\\drums\\" + QString::number(index) + "_" + entry.Name + ".wav");
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
        wav.NumChannels = numChannels;
        wav.SampleRate = entry.SampleRate;
        wav.ByteRate = (entry.SampleRate * numChannels * entry.BitsPerSample) / 8;
        wav.BlockAlign = (numChannels * entry.BitsPerSample) / 8;
        wav.BitsPerSample = entry.BitsPerSample;
        wav.Subchunk2Size = entry.DataSize;

        file.write(reinterpret_cast<const char*>(&wav), sizeof(WAVE));
        file.write(reinterpret_cast<const char*>(&m_pBuffer[offset + entry.HeaderSize]), entry.DataSize);
        file.close();
    }
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

void CDrums::Verify()
{
    QErrorMessage msg;
    bool errorMsg = false;

    if (m_Bank.Entry[0].second.SampleRate != 22050) {
        errorMsg = true;
    }
    else
    {
        auto c = m_pBuffer[m_Bank.Entry[0].first + 8 + 14];
        if (c != '\0') {
            errorMsg = true;
        }
    }

    if (errorMsg) {
        msg.showMessage("Verify failed!<br>Bank is potentially not a Music Bank!");
        msg.exec();
    }
}
