#include "Drums.h"

CDrums::CDrums()
{
    m_pBuffer = nullptr;
    m_nBufferLength = 0;
}

void CDrums::FillTable(QTableWidget* table)
{
    int32_t rows = this->GetEntries();
    int32_t columns = 6;

    table->clear();
    table->setRowCount(rows);
    table->setColumnCount(columns);

    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Index"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Offset"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("HeaderSize"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("DataSize"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("Name"));
    table->setHorizontalHeaderItem(5, new QTableWidgetItem("SampleRate"));

    table->setColumnWidth(0, 15); // Index
    table->setColumnWidth(1, 50); // Offset
    table->setColumnWidth(2, 65); // HeaderSize
    table->setColumnWidth(3, 56); // DataSize
    table->setColumnWidth(4, 100); // Name
    table->setColumnWidth(5, 70); // SampleRate

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

}

int32_t CDrums::GetEntries() const
{
    return m_Bank.Entries;
}

void CDrums::Clear()
{
    delete[] m_pBuffer;
    m_pBuffer = nullptr;
    m_Bank.Entry.clear();
    m_nBufferLength = 0;
}
