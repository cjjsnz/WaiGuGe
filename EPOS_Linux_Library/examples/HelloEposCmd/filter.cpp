#include "filter.h"

static float lpf_numerator_coef[2][3]={{0.3913,0.7827,0.3913},{0.1311,0.2622,0.1311}};
static float lpf_denominator_coef[2][3]={{1.0000,0.3695,0.1958},{1.0000,-0.7478,0.2722}};

static float hpf_numerator_coef[2][3]={{0.8371,-1.6742,0.8371},{0.9510,-1.8299,0.9150}};
static float hpf_denominator_coef[2][3]={{1.0000,-1.6475,0.7009},{1.0000,-1.8227,0.8372}};

static float ahf_numerator_coef_50HZ[2][6]={
    {0.9522,-1.5407,0.9522,0.8158,-0.8045,0.0855},
    {0.5869,-1.1146,0.5869,1.0499,-2.0000,1.0499}};
static float ahf_denominator_coef_50HZ[2][6]={
    {1.0000,-1.5395,0.9056,1.0000,-1.1187,0.3129},
    {1.0000,-1.8844,0.9893,1.0000,-1.8991,0.9892}};
static float ahf_output_gain_coef_50HZ[2]={1.3422,1.4399};


static float ahf_numerator_coef_60HZ[2][6]={
    {0.9528,-1.3891,0.9528,0.8272,-0.7225,0.0264},
    {0.5824,-1.0810,0.5824,1.0736,-2.0000,1.0736}};
static float ahf_denominator_coef_60HZ[2][6]={
    {1.0000,-1.3880,0.9066,1.0000,-0.9739,0.2371},
    {1.0000,-1.8407,0.9894,1.0000,-1.8584,0.9891}};
static float ahf_output_gain_coef_60HZ[2]={1.3430,1.4206};

enum FILTER_TYPE{
    FILTER_TYPE_LOWPASS=0,
    FILTER_TYPE_HIGHPASS};


class FILTER_2nd{
   private:
     float states[2];
     float num[3];
     float den[3];
   public:
    void init(FILTER_TYPE ftype,int sampleFreq){
      states[0]=0;
      states[1]=0;
      if(ftype == FILTER_TYPE_LOWPASS){
        if(sampleFreq==SAMPLE_FREQ_500HZ){
          for(int i=0;i<3;i++){
            num[i]=lpf_numerator_coef[0][i];
            den[i]=lpf_denominator_coef[0][i];
          }
        }
        else if(sampleFreq==SAMPLE_FREQ_1000HZ){
          for(int i=0;i<3;i++){
            num[i]=lpf_numerator_coef[1][i];
            den[i]=lpf_denominator_coef[1][i];
          }
        }
      }
      else if(ftype==FILTER_TYPE_HIGHPASS){
        if(sampleFreq==SAMPLE_FREQ_500HZ){
          for(int i=0;i<3;i++){
            num[i]=hpf_numerator_coef[0][i];
            den[i]=hpf_denominator_coef[0][i];
          }
        }
      
       else if(sampleFreq==SAMPLE_FREQ_1000HZ){
          for(int i=0;i<3;i++){
            num[i]=hpf_numerator_coef[1][i];
            den[i]=hpf_denominator_coef[1][i];
          }
        }  
      }
    }
    float update(float input){
    float tmp=(input-den[1]*states[0]-den[2]*states[1]/den[0]);
    float output=num[0]*tmp+num[1]*states[0]+num[2]*states[1];
    states[1]=states[0];
    states[0]=tmp;
    return output;

    }
};   



class FILTER_4th{
   private:
     float states[4];
     float num[6];
     float den[6];
     float gain;
  public:
     void init(int sampleFreq,int humFreq){
        gain=0;
        for(int i=0;i<4;i++){
          states[i]=0;
        }
        if(humFreq==NOTCH_FREQ_50HZ){
          if(sampleFreq=SAMPLE_FREQ_500HZ){
            for(int i=0;i<6;i++){
              num[i]=ahf_numerator_coef_50HZ[0][i];
              den[i]=ahf_denominator_coef_50HZ[0][i];
            }
          gain=ahf_output_gain_coef_50HZ[0];
          }
          else if(sampleFreq=SAMPLE_FREQ_1000HZ){
            for(int i=0;i<6;i++){
              num[i]=ahf_numerator_coef_50HZ[1][i];
              den[i]=ahf_denominator_coef_50HZ[1][i];

            }
          }
          gain=ahf_output_gain_coef_50HZ[1];
          }
        else if(humFreq==NOTCH_FREQ_60HZ){
          if(sampleFreq=SAMPLE_FREQ_500HZ){
            for(int i=0;i<6;i++){
            num[i]=ahf_numerator_coef_60HZ[0][i];
              den[i]=ahf_denominator_coef_60HZ[0][i];
            }
            
          }
          gain=ahf_output_gain_coef_60HZ[0];
        }
           else if(sampleFreq=SAMPLE_FREQ_1000HZ){
            for(int i=0;i<6;i++){
              num[i]=ahf_numerator_coef_60HZ[1][i];
              den[i]=ahf_denominator_coef_60HZ[1][i];
            }        
          gain=ahf_output_gain_coef_60HZ[1];
        }
      }
    }


     }

}