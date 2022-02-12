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
    AxisZ(0)
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
  const int strikeThreshold = 29000;
  const int swingThreshold = 20000;
  SoundsPlayer* soundsPlayerPtr;
  int16_t AxisX;
  int16_t AxisY;
  int16_t AxisZ;

  void readCurrentAxisValues()
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 12, true);

    AxisX = Wire.read()<<8|Wire.read();
    AxisY = Wire.read()<<8|Wire.read();
    AxisZ = Wire.read()<<8|Wire.read();
    Serial.print("X: " );Serial.println(AxisX);
    Serial.print("Y: " );Serial.println(AxisY);
    Serial.print("Z: " );Serial.println(AxisZ);
    Serial.println("---------------------------------");
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
  }

  bool isStrikeMove() const
  {
    return ( strikeThreshold < abs(AxisX) ||
             strikeThreshold < abs(AxisY) ||
             strikeThreshold < abs(AxisZ) );
  }

  bool isSwingMove() const
  {
    return ( swingThreshold < abs(AxisX) ||
             swingThreshold < abs(AxisY) ||
             swingThreshold < abs(AxisZ) );
  }
};

#endif // MOVEDETECTOR_H
