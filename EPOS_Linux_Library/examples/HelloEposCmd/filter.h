#pragma once
#ifndef _EMGFILTERS_H
#define _EMGFILTERS_H
enum NOTCH_FREQUENCY{NOTCH_FREQ_50HZ=50,NOTCH_FREQ_60HZ=60};
enum SAMPLE_FREQUENCY{SAMPLE_FREQ_500HZ=500,SAMPLE_FREQ_1000HZ=10000};
class EMGFilters{
public:
  void init(SAMPLE_FREQUENCY sampleFReq,
            NOTCH_FREQUENCY  notchFreq,
            bool             enableNotchFilter =true,
            bool             enableLowpassFilter=true,
            bool             enableHighpassFilter=true);
  int update(int inputValue);

private:
SAMPLE_FREQUENCY m_sampleFreq;
NOTCH_FREQUENCY  m_notchFreq;
bool             m_bypassEnabled;
bool             m_notchFilterEnabled;
bool             m_lowpassFilterEnabled;
bool             m_highpassFilterEnabled;
};



#endif