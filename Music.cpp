/*
    Copyright © 2021 Toksisitee <meypcm@gmail.com>
    PopSoundEditor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    PopSoundEditor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#include "Music.h"

CMusic::CMusic()
{
    m_pBuffer = nullptr;
    m_nBufferLength = 0;
    m_Audio = nullptr;
}

void CMusic::FillTable()
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

void CMusic::Create(const QString &fileName)
{
    QErrorMessage msg;
    msg.showMessage("Functionality not implemented yet!");
    msg.exec();

// Imcomplete impl.
#if 0
    QVector<MP22Entry> vec;

    QDir directory(QDir::currentPath() + "\\music\\");
    QStringList files = directory.entryList(QStringList() << "*.mp2" << "*.MP2", QDir::Files);

    foreach(QString mp2Name, files)
    {
        bool isDigit = false;
        int removePos = mp2Name.indexOf('_');
        QStringRef tmp(&mp2Name, 0, removePos);
        int index = tmp.toInt(&isDigit, 10);

        if (isDigit)
        {
            MP22Entry entry;
            entry.Index = index;

            QString fullName = mp2Name;
            mp2Name.remove(0, removePos + 1);
            mp2Name.chop(4);
            strncpy(entry.Name, mp2Name.toLatin1().data(), sizeof(entry.Name));

            QString filePath = QDir::currentPath() + "\\music\\" + fullName;
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly))
            {
                char* pBuffer = new char[file.size()];
                file.seek(0);
                file.read(pBuffer, file.size());
                file.close();

                entry.Data = new char[file.size()];// - 4 - 20 - sizeof(TbMusicEntry)];
                entry.Size = file.size();
                memcpy(&entry.Data[0], &pBuffer[0], file.size());
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
        const MP22Entry& fsound,
        const MP22Entry& ssound)
    {
        return (fsound.Index < ssound.Index);
    });

    auto it = std::unique(vec.begin(), vec.end(), [](
        const MP22Entry& fsound,
        const MP22Entry& ssound)
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
        uint16_t SampleRate = 22050;

        for (int32_t i = 0; i < vec.size(); i++)
        {
            file.write(reinterpret_cast<const char*>(&data_offset), 4);
            data_offset += HeaderSize + vec[i].Size;
        }

        for (int32_t i = 0; i < vec.size(); i++)
        {
            file.write(reinterpret_cast<const char*>(&HeaderSize), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Size), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Name), 8);
            file.write(reinterpret_cast<const char*>(&Unknown), 4);
            file.write(reinterpret_cast<const char*>(&Unknown), 4);
            file.write(reinterpret_cast<const char*>(&SampleRate), 2);
            file.write(reinterpret_cast<const char*>(&Unknown), 4);
            file.write(reinterpret_cast<const char*>(&Unknown), 4);
            file.write(reinterpret_cast<const char*>(&Unknown), 4);
            file.write(reinterpret_cast<const char*>(&vec[i].Data[0]), vec[i].Size);
        }

        foreach (auto ptr, vec) {
            delete[] ptr.Data;
        }

        QMessageBox msgBox;
        msgBox.setText(QString("Successfully created SDT bank containing %1 sounds.").arg(numSounds));
        msgBox.exec();
        file.close();
    }
#endif
}

void CMusic::Export(uint32_t index)
{
    auto entry = m_Bank.Entry[index].second;
    auto offset = m_Bank.Entry[index].first;

    QFile file(QDir::currentPath() + "\\music\\" + QString::number(index) + "_" + entry.Name + ".mp2");
    if (file.open(QIODevice::WriteOnly))
    {
        offset += sizeof(TbMusicEntry);
        file.write(reinterpret_cast<const char*>(&m_pBuffer[offset + sizeof(TbMusicEntry)]), entry.DataSize);
        file.close();
    }
}

void CMusic::Play(uint32_t index)
{
    QErrorMessage msg;
    msg.showMessage("Music tracks cannot be played within the editor!\nYou may only listen to exported tracks.");
    msg.exec();
}

void CMusic::Verify()
{
    QErrorMessage msg;
    bool errorMsg = false;

    if (m_Bank.Entry[0].second.SampleRate != 22050) {
        errorMsg = true;
    }
    else
    {
        auto c = m_pBuffer[m_Bank.Entry[0].first + 8 + sizeof(m_Bank.Entry[0].second.Name) + 1];//qstrlen(m_Bank.Entry[0].second.Name)];
        if (c == '\0') {
            errorMsg = true;
        }
    }

    if (errorMsg) {
        msg.showMessage("Verify failed!<br>Bank is potentially not a Music Bank!");
        msg.exec();
    }
}
