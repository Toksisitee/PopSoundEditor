#include "Music.h"

CMusic::CMusic()
{
    m_pBuffer = nullptr;
    m_nBufferLength = 0;
}

bool CMusic::Load(const QString& filename)
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
                m_Bank.Entry.push_back(std::make_pair(offset, *(reinterpret_cast<TbMusicEntry*>(&m_pBuffer[offset]))));
                pBuffer += sizeof(uint32_t);
            }
            return true;
        }
        return false;
    }
    return false;
}

void CMusic::FillTable(QTableWidget* table)
{

}

void CMusic::Create(const QString &file)
{


}

void CMusic::Export(uint32_t index)
{


}

void CMusic::Play(uint32_t index)
{

}

int32_t CMusic::GetEntries() const
{
    return m_Bank.Entries;
}

void CMusic::Clear()
{
    delete[] m_pBuffer;
    m_pBuffer = nullptr;
    m_Bank.Entry.clear();
    m_nBufferLength = 0;
}
