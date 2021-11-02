#ifndef PINSCONFIG_H
#define PINSCONFIG_H

#define ACTIVATION_BUTTON_PIN (16)
#define CHANGE_COLOR_PIN (5)
#define BRIGHTNESS_PIN (0)
#define LED_STRIP_DATA_PIN (4)

static void setupConfiguratedPins()
{
  pinMode(ACTIVATION_BUTTON_PIN, INPUT);
  pinMode(CHANGE_COLOR_PIN, INPUT);
}

#endif // PINSCONFIG_H
