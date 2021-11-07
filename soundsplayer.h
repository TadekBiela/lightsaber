#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include "pinsconfig.h"
#include <SoftwareSerial.h>

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
  }

  void init()
  {
    softwareSerial.begin(9600);
    delay(1000);
    setVolume(1);
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

private:
  SoftwareSerial softwareSerial;
  TRACK lastPlayedTrack;
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
    SET_VOLUME = 0x06
  };

  void sendCommand(byte command, byte argument)
  {
    softwareSerial.write((byte)0x7E);
    softwareSerial.write((byte)0xFF);
    softwareSerial.write((byte)0x06);
    softwareSerial.write(command);
    softwareSerial.write((byte)0x00);
    softwareSerial.write((byte)0x00);
    softwareSerial.write(argument);
    softwareSerial.write((byte)0xEF);
  }

  void playRandomTrackFromRangeWithIdleAtEnd(SoundsPlayer::TRACK begin, SoundsPlayer::TRACK end)
  {
    int randomSwingTrack = random(begin, end + 1);
    playTrack(randomSwingTrack);
    loopTrack(SoundsPlayer::IDLE);
  }
};

#endif // SOUNDSPLAYER_H
