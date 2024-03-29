#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include "pinsconfig.h"
#include <SoftwareSerial.h>

#define DFPLAYER_UART_FRAME_SIZE 0x08

class SoundsPlayer
{
public:
  enum TRACK
  {
    NO_TRACK = 0,
    ON = 1,
    OFF,
    SWING1,
    SWING2,
    SWING3,
    SWING4,
    SWING5,
    SWING6,
    SWING7,
    SWING8,
    STRIKE1,
    STRIKE2,
    STRIKE3,
    IDLE,
    NUM_OF_TRACKS = IDLE
  };

  SoundsPlayer() :
    softwareSerial(SOUNDS_PLAYER_RX_PIN, SOUNDS_PLAYER_TX_PIN),
    lastPlayedTrack(SoundsPlayer::NO_TRACK)
  {
    memset(commandBuffer, 0x00, DFPLAYER_UART_FRAME_SIZE);
  }

  void init()
  {
    softwareSerial.begin(9600);
    delay(1000);
    setVolume(25);
  }

  void playTrack(SoundsPlayer::TRACK track)
  {
    sendCommand(PLAY_TRACK, track);
    lastPlayedTrack = track;
  }

  void playRandomSwingTrack()
  {
    playRandomTrackFromRangeWithIdleAtEnd(SoundsPlayer::SWING1, SoundsPlayer::SWING8);
  }

  void playRandomStrikeTrack()
  {
    playRandomTrackFromRangeWithIdleAtEnd(SoundsPlayer::STRIKE1, SoundsPlayer::STRIKE3);
  }

  void loopTrack(SoundsPlayer::TRACK track)
  {
    delay(trackDurationInMs[lastPlayedTrack]);
    sendCommand(LOOP_TRACK, track);
  }

  void setVolume(byte volume)
  {
    sendCommand(SET_VOLUME, volume);
  }

  void stop()
  {
    sendCommand(STOP, 0);
  }

private:
  SoftwareSerial softwareSerial;
  TRACK lastPlayedTrack;
  uint8_t commandBuffer[DFPLAYER_UART_FRAME_SIZE];
  const int trackDurationInMs[SoundsPlayer::NUM_OF_TRACKS] =
  {
    0,    //NO_TRACK
    1400, //ON
    660,  //OFF
    530,  //SWING1
    660,  //SWING2
    900,  //SWING3
    560,  //SWING4
    610,  //SWING5
    900,  //SWING6
    560,  //SWING7
    710,  //SWING8
    560,  //STRIKE1
    610,  //STRIKE2
    850   //STRIKE3
  };

  enum COMMANDS
  {
    PLAY_TRACK = 0x03,
    LOOP_TRACK = 0x08,
    SET_VOLUME = 0x06,
    STOP = 0x0E
  };

  void sendCommand(byte command, byte argument)
  {
    commandBuffer[0] = (byte)0x7E;
    commandBuffer[1] = (byte)0xFF;
    commandBuffer[2] = (byte)0x06;
    commandBuffer[3] = command;
    commandBuffer[4] = (byte)0x00;
    commandBuffer[5] = (byte)0x00;
    commandBuffer[6] = argument;
    commandBuffer[7] = (byte)0xEF;

    softwareSerial.write(commandBuffer, DFPLAYER_UART_FRAME_SIZE);
  }

  void playRandomTrackFromRangeWithIdleAtEnd(SoundsPlayer::TRACK begin, SoundsPlayer::TRACK end)
  {
    int randomSwingTrack = random(begin, end + 1);
    playTrack(randomSwingTrack);
    loopTrack(SoundsPlayer::IDLE);
  }
};

#endif // SOUNDSPLAYER_H
