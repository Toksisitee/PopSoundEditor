#include "Sound.h"

CSound::CSound()
{
    m_pBuffer = nullptr;
    m_nBufferLength = 0;
}

void CSound::FillTable(QTableWidget* table)
{
    int32_t rows = this->GetEntries();
    int32_t columns = 11;

    table->clear();
    table->setRowCount(rows);
    table->setColumnCount(columns);

    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Index"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("HeaderSize"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("DataSize"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("Name"));
    table->setHorizontalHeaderItem(5, new QTableWidgetItem("SampleRate"));
    table->setHorizontalHeaderItem(6, new QTableWidgetItem("BitsPerSample"));
    table->setHorizontalHeaderItem(7, new QTableWidgetItem("NumChannels"));
    table->setHorizontalHeaderItem(8, new QTableWidgetItem("Unknown"));
    table->setHorizontalHeaderItem(9, new QTableWidgetItem("LoopStart"));
    table->setHorizontalHeaderItem(10, new QTableWidgetItem("LoopEnd"));

    table->setColumnWidth(0, 15); // Index
    table->setColumnWidth(1, 50); // Offset
    table->setColumnWidth(2, 65); // HeaderSize
    table->setColumnWidth(3, 56); // DataSize
    table->setColumnWidth(4, 100); // Name
    table->setColumnWidth(5, 70); // SampleRate
    table->setColumnWidth(6, 85); // BitsPerSample
    table->setColumnWidth(7, 83); // NumChannels
    table->setColumnWidth(8, 60); // Unknown
    table->setColumnWidth(9, 60); // LoopStart
    table->setColumnWidth(10, 60); // LoopEnd

    for (int i = 0; i < this->GetEntries(); i++)
    {
        auto entry = m_Bank.Entry[i].second;
        table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        QString offset = QString::asprintf("%02X", m_Bank.Entry[i].first);
        table->setItem(i, 1, new QTableWidgetItem(offset));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(entry.HeaderSize)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(entry.DataSize)));
        table->setItem(i, 4, new QTableWidgetItem(entry.Name));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(entry.SampleRate)));
        table->setItem(i, 6, new QTableWidgetItem(QString::number(entry.BitsPerSample)));
        table->setItem(i, 7, new QTableWidgetItem(QString::number(entry.NumChannels)));
        table->setItem(i, 8, new QTableWidgetItem(QString::number(entry.Unknown)));
        table->setItem(i, 9, new QTableWidgetItem(QString::number(entry.LoopStart)));
        table->setItem(i, 10, new QTableWidgetItem(QString::number(entry.LoopEnd)));
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

}

int32_t CSound::GetEntries() const
{
    return m_Bank.Entries;
}
