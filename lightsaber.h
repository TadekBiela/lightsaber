#ifndef LIGHTSABER_H
#define LIGHTSABER_H

#include "blade.h"
#include "button.h"
#include "pinsconfig.h"
#include "soundsplayer.h"

class LightSaber
{
public:
  LightSaber() :
    isActive(false),
    activationButton(ACTIVATION_BUTTON_PIN),
    blade(&soundsPlayer)
  {}

  void init()
  {
    soundsPlayer.init();
    blade.init();
  }

  bool isActivated()
  {
    if(activationButton.clicked())
    {
      isActive = !isActive;
    }

    return isActive;
  }

  void activate()
  {
    blade.activate();
    activateExecutionLoop();
  }

  void deactivate()
  {
    blade.deactivate();
  }
  
private:
  bool isActive;
  Button activationButton;
  SoundsPlayer soundsPlayer;
  Blade blade;

  void activateExecutionLoop()
  {
    while(isActivated())
    {
      blade.checkChanges();
      delay(50);
    }
    
    deactivate();
  }
};

#endif // LIGHTSABER_H
