#ifndef BUTTON_H
#define BUTTON_H

class Button
{
public:
  Button(int inputPin) :
    buttonPin(inputPin),
    isWasPressed(false)
  {}
  bool clicked()
  {
    if(digitalRead(buttonPin) && isWasPressed == false)
    {
      isWasPressed = true;
      return true;
    }
    else if(digitalRead(buttonPin) == 0)
    {
      isWasPressed = false;
    }
    return false;
  }
  
private:
  int buttonPin;
  bool isWasPressed;
};

#endif // BUTTON_H
