#ifndef MOVEDETECTOR_H
#define MOVEDETECTOR_H

#include<Wire.h>

class MoveDetector
{
public:
  MoveDetector(SoundsPlayer* soundsPlayer) :
    soundsPlayerPtr(soundsPlayer),
    AxisX(0),
    AxisY(0),
    AxisZ(0),
    prevAxisX(0),
    prevAxisY(0),
    prevAxisZ(0)
  {
  }

  void init()
  {
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B); 
    Wire.write(0);    
    Wire.endTransmission(true);
  }

  void checkChanges()
  {
    readCurrentAxisValues();
    playMoveSoundIfRequired();
  }

private:
  const int MPU = 0x68;
  const int strikeThreshold = 18000;
  const int swingThreshold = 8000;
  SoundsPlayer* soundsPlayerPtr;
  int16_t AxisX;
  int16_t AxisY;
  int16_t AxisZ;
  int16_t prevAxisX;
  int16_t prevAxisY;
  int16_t prevAxisZ;

  void readCurrentAxisValues()
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 12, true);

    AxisX = Wire.read()<<8|Wire.read();
    AxisY = Wire.read()<<8|Wire.read();
    AxisZ = Wire.read()<<8|Wire.read();
  }

  void playMoveSoundIfRequired()
  {
    if(isStrikeMove())
    {
      soundsPlayerPtr->playRandomStrikeTrack();
    }
    else if(isSwingMove())
    {
      soundsPlayerPtr->playRandomSwingTrack();
    }
    prevAxisX = AxisX;
    prevAxisY = AxisY;
    prevAxisZ = AxisZ;
  }

  bool isStrikeMove() const
  {
    return ( strikeThreshold < abs(prevAxisX - AxisX) ||
             strikeThreshold < abs(prevAxisY - AxisY) ||
             strikeThreshold < abs(prevAxisZ - AxisZ) );
  }

  bool isSwingMove() const
  {
    Serial.print("X: " );Serial.println(abs(prevAxisX - AxisX));
    Serial.print("Y: " );Serial.println(abs(prevAxisY - AxisY));
    Serial.print("Z: " );Serial.println(abs(prevAxisZ - AxisZ));
    Serial.println("---------------------------------");
    return ( swingThreshold < abs(prevAxisX - AxisX) ||
             swingThreshold < abs(prevAxisY - AxisY) ||
             swingThreshold < abs(prevAxisZ - AxisZ) );
  }
};

#endif // MOVEDETECTOR_H
