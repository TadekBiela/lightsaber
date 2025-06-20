#ifndef MOVEDETECTOR_H
#define MOVEDETECTOR_H

#include<Wire.h>
#include<MPU6050_light.h>

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
    mpu.begin();
    delay(1000);
    mpu.calcGyroOffsets();
  }

  void checkChanges()
  {
    readCurrentAxisValues();
    playMoveSoundIfRequired();
  }

private:
  const int strikeThreshold = 230;
  const int swingThreshold = 100;
  MPU6050 mpu(Wire);
  SoundsPlayer* soundsPlayerPtr;
  int16_t AxisX;
  int16_t AxisY;
  int16_t AxisZ;
  int16_t prevAxisX;
  int16_t prevAxisY;
  int16_t prevAxisZ;

  void readCurrentAxisValues()
  {
    mpu.update();

    AxisX = mpu.getAccX();
    AxisY = mpu.getAccY();
    AxisZ = mpu.getAccZ();
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
