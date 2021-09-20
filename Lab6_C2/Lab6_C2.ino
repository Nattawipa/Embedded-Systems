//6188089 - Nattawipa Saetae

#define POT 33
#define MIN 0
#define MAX 4095

#include "SoundData.h"
#include "src/XT_DAC_Audio/XT_DAC_Audio.h"

XT_Wav_Class Sound(sample);     
                                      
XT_DAC_Audio_Class DacAudio(25,0);    

uint32_t DemoCounter = 0; 
              
int aValue;

void setup() 
{
  Serial.begin(115200);    
  Serial.println(A0);           
}

void loop() 
{
  aValue = analogRead(POT);
  aValue = map(aValue, MIN, MAX, 0, 100);

  DacAudio.FillBuffer();
  DacAudio.DacVolume = aValue; 
                 
  if(Sound.Playing == false)       
    DacAudio.Play(&Sound);  
         
  //Serial.println(DemoCounter++);        
}
