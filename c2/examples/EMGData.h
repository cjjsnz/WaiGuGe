#pragma once
#include <vector>
#include"kalman_Filter.h"
enum FingerStatus{ FIN_FWD,FIN_HLT,FIN_BCK,FIN_CUR};
class CEMGData
{
public:
    long m_lHigh[4]={100,160,120,100};
    long m_lLow[4]={80,90,120,50};
    long m_lWeak[4]={20,40,40,30};
    FingerStatus m_fsFignerStatus[4]={FIN_HLT,FIN_HLT,FIN_HLT,FIN_HLT};
    std::vector<double>m_dRawData[4];
    std::vector<double>m_dRawDataAve[4];
    double m_dEMGDataAve[4]={0,0,0,0};
    double m_dEMGDataAvePre[4]={0,0,0,0};
    long m_dEMGDataAveCount=0;
    short m_sCount=5;
    kalman_filter_unidimension kalman_filter[4]{{0.01,0.01},{0.01,0.01},{0.01,0.01},{0.01,0.01}};
    CEMGData();
    ~CEMGData();
    void GetRawData(double*);
    void GetRawDataFilter(double*);

    void GetEMGDataAve();
    template<typename T> double GetAve(T&t)const;
    float emgdata[10];
};


