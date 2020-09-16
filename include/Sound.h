#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <string>
#include <SDL2/SDL_mixer.h>
#include <map>
#include <tuple>

using namespace std;

class Sound{

    private:
        Mix_Chunk *sound;
        int channel;
        bool first_play;
        bool high_cadence;

    public:
        Sound();
        Sound(string sound_name);
        void assignSound(string sound_name);
        void playSound();
        virtual ~Sound();

    public:
        static map<string, tuple<Mix_Chunk*, int, int>> global_sound;  //la map contenant les sons globaux
        static void initializeGlobalsSound();

};

class SoundNotFoundException{};
class IncorrectPathForSoundException{};


class Music{

    private:
        Mix_Music *music;
        string music_name;
        bool first_play;

    public:
        Music();
        Music(string music_name);
        void assignMusic(string music_name);
        void playMusic();
        void fadeOut(int ms);
        void changeMusic(string music_name);
        void changeAndPlayMusic(string music_name);
        void pause();
        void resume();
        virtual ~Music();

    public:
        static map<string, Mix_Music*> global_music;  //la map contenant les musiques globals
        static void initializeGlobalsMusic();

};

class MusicNotFoundException{};
class IncorrectPathForMusicException{};

#endif // SOUND_H
