#include"EMGData.h"
CEMGData::CEMGData()
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<30;j++)
        {
            m_dRawDataAve[i].push_back(0);
        }
    }
}
CEMGData::~CEMGData()
{
}

void CEMGData::GetRawData(double* rawdata)
{
    for(int i=0;i<4;i++)
    {
        m_dRawData[i].push_back(rawdata[i]);

    }
    if (m_dRawData[0].size()==10)
    {
        GetEMGDataAve();
        for(int i=0;i<4;i++)
        {
            m_dRawData[i].clear();
        }
    }
}
void CEMGData::GetRawDataFilter(double* rawdata)
{
    for(int i=0;i<4;i++)
    {
        m_dEMGDataAvePre[i]=kalman_filter[i].update(rawdata[i]);
    }
}
void CEMGData::GetEMGDataAve()
{
    for(int i=0;i<4;i++)
    {
        m_dRawDataAve[i].push_back(GetAve<std::vector<double>>(m_dRawData[i]));
        m_dRawDataAve[i].erase(m_dRawDataAve[i].begin());
        m_dEMGDataAvePre[i]=GetAve<std::vector<double>>(m_dRawDataAve[i]);
    }
    m_dEMGDataAveCount++;
};
template<typename T>
double CEMGData::GetAve(T&t)const
{
    double sum=0;
    int cnt=0;
    for(auto i:t)
    {
        sum +=i;
        cnt++;
    }
    return sum/cnt;
}




