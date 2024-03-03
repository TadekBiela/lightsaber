#ifndef MONO_BUTTON_H
#define MONO_BUTTON_H

/*
 * Remember to connect 1k ohm pull-down resistor
 * between buttonPin and ground.
 */

class MonoButton
{
public:
  MonoButton(int inputPin) :
    buttonPin(inputPin)
  {}

  bool clicked() const
  {
    return digitalRead(buttonPin) == 1 ? true : false ;
  }

private:
  int buttonPin;
};

#endif // MONO_BUTTON_H
