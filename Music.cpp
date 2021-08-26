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

void CMusic::Create(const QString &file)
{


}

void CMusic::Export(uint32_t index)
{
    auto entry = m_Bank.Entry[index].second;
    auto offset = m_Bank.Entry[index].first;

    QFile file(QDir::currentPath() + "\\music\\" + QString::number(index) + "_" + entry.Name + ".mp2");
    if (file.open(QIODevice::WriteOnly))
    {
        offset += sizeof(TbMusicEntry);
        file.write(reinterpret_cast<const char*>(&m_pBuffer[offset]), entry.DataSize);
        file.close();
    }
}

void CMusic::Play(uint32_t index)
{
    QErrorMessage msg;
    msg.showMessage("Music tracks cannot be played within the editor!\nYou may only listen to exported tracks.");
    msg.exec();
}
