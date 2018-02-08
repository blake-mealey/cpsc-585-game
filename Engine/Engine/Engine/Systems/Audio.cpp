#pragma once
#include "Audio.h"


Audio::Audio() {

    result = FMOD::System_Create(&soundSystem);
    if (result != FMOD_OK) {
    }

    // result = soundSystem->getVersion(&version);
    if (result != FMOD_OK) {
    }

    result = soundSystem->init(32, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK) {
    }

    result = soundSystem->createStream("content/sounds/peel_out.wav", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
    result = sound->getNumSubSounds(&numsubsounds);


    if (numsubsounds) {
        sound->getSubSound(0, &soundToPlay);
    } else {
        soundToPlay = sound;
    }
    result = soundSystem->playSound(soundToPlay, 0, false, &channel);

}

Audio::~Audio() {
    result = sound->release();
    result = soundSystem->close();
    result = soundSystem->release();
}

void Audio::playSound() {
    result = soundSystem->update();
}