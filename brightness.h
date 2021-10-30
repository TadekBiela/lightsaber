class Brightness
{
public:
  const float MIN = 0.01;
  const float MAX = 1;

  Brightness(
    int inputPin,
    int inputAnalogMin,
    int inputAnalogMax) :
    brightnessPin(inputPin),
    analogMin(inputAnalogMin),
    analogMax(inputAnalogMax)
  {}

  float level()
  {
    int analogInput = analogRead(brightnessPin);
    float brightnessLevel = ((float)(analogInput) / (float)(analogMax));
    return brightnessLevel < MIN ? MIN : brightnessLevel;
  }

private:
  int brightnessPin;
  int analogMin;
  int analogMax;
};