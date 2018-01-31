#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>

typedef FMOD::Sound* SoundClass;

class Audio {
public:
    Audio();
    ~Audio();
    void Audio::playSound();

private:
    FMOD::System *soundSystem;
    FMOD::Sound *sound, *soundToPlay;
    FMOD::Channel *channel = 0;
    FMOD_RESULT result;
    unsigned int version;
    void  *extradriverdata = 0;
    int numsubsounds;
};