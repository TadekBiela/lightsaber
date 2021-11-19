#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

class Potentiometer
{
public:
  const float MIN = 0.01;
  const float MAX = 1;

  Potentiometer(
    int inputPin,
    int inputAnalogMin,
    int inputAnalogMax) :
    potentiometerPin(inputPin),
    analogMin(inputAnalogMin),
    analogMax(inputAnalogMax)
  {}

  float level()
  {
    int analogInput = analogRead(potentiometerPin);
    float potentiometerLevel = ((float)(analogInput) / (float)(analogMax));
    return potentiometerLevel < MIN ? MIN : potentiometerLevel;
  }

private:
  int potentiometerPin;
  int analogMin;
  int analogMax;
};

#endif // POTENTIOMETER_H
