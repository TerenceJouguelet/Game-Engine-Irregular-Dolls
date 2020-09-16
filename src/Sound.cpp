#include "Sound.h"

Sound::Sound(): sound(nullptr){
    this->first_play = false;
}


Sound::Sound(string sound_name){
    this->assignSound(sound_name);
    this->first_play = false;
}

map<string, tuple<Mix_Chunk*, int, int>> Sound::global_sound = {};

void Sound::initializeGlobalsSound(){
    Sound::global_sound = {
        { "Pulse_1", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/tan00.wav"),                0, 20) },
        { "Pulse_2", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_tan01.wav"),             6, 20) },
        { "Pulse_3", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_tan02.wav"),             7, 20) },

        { "Player_Shoot", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/plst00.wav"),          1, 30) },
        { "Player_Death", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/pldead00.wav"),        1, 64) },
        { "Enemy_Death", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_enep00.wav"),        2, 30) },
        { "Bullet_Death", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_enep00.wav"),       4, 15) },
        { "Slash", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_slash.wav"),/*-----------*/2, 100) },
        { "Another_Path", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_kira00.wav"),       3, 64) },
        { "Spell_Finish", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_cardget.wav"),      5, 100) },
        { "Damage", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/graze.wav"),                 4, 64) },
        { "Spell_Card_Activate", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_cat00.wav"), 5, 64) },
        { "Selection", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_ok00.wav"),            0, 90) },
        { "Ok", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/決定2.wav"),                     1, 90) },
        { "Pause", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_pause.wav"),               1, 64) },

        { "Blabla_Satori", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/blabla_satori.wav"),               0, 100) }
        //{ "Pause", tuple<Mix_Chunk*, int, int>(Mix_LoadWAV("data/sons/se_pause.wav"),               0, 64) },
    };
}

void Sound::assignSound(string sound_name){
    this->high_cadence = false;


    if(sound_name == "None"){
        this->sound = nullptr;
        //this->sound = Mix_LoadWAV("data/sons/rien.wav");
    }
    else{
        map<string, tuple<Mix_Chunk*, int, int>>::iterator it_on_sound = Sound::global_sound.find(sound_name);
        if(it_on_sound != Sound::global_sound.end()){
            this->sound = get<0>(it_on_sound->second);
            this->channel = get<1>(it_on_sound->second);
            this->sound->volume = get<2>(it_on_sound->second);
            //cout << it_on_sound->first << '\t' << it_on_sound->second << endl;
        }
        else{
            cout << "Sound " << sound_name << " not found" << endl;
            throw SoundNotFoundException();
        }
    }
    /*
    else{
        cout << "Sound " << sound_name << " not found" << endl;
        throw SoundNotFoundException();
    }

    if(this->sound == nullptr && sound_name != "None"){
        throw IncorrectPathForSoundException();
    }*/
}

void Sound::playSound(){
    if(this->sound != nullptr){
        //cout << this->current_channel << endl;
        if(this->first_play){
            Mix_HaltChannel(this->channel);
        }
        Mix_PlayChannel( this->channel, this->sound, 0 );
        this->first_play = true;
    }
    else{
        //cout << "Son non defini" << endl;
    }
}

Sound::~Sound(){
    //Mix_FreeChunk(this->sound);
}












Music::Music(): music(nullptr){
    this->first_play = false;
}


Music::Music(string music_name){
    this->music_name = music_name;
    this->assignMusic(music_name);
    this->first_play = false;
}

map<string, Mix_Music*> Music::global_music = {};

void Music::initializeGlobalsMusic(){
    Music::global_music = {
        { "Main_Theme", Mix_LoadMUS("data/Music/Automaton of the East.ogg") },
        { "Stage_Theme", Mix_LoadMUS("data/Music/Blowitch - Crimson Strings.ogg") },
        { "Game_Over_Theme", Mix_LoadMUS("data/Music/Player's Score.ogg") },
        { "Hallow_Palace", Mix_LoadMUS("data/Music/Blowitch - The Hallow Palace.ogg") }
    };
}

void Music::assignMusic(string music_name){
    this->music_name = music_name;
    if(music_name == "None"){
        //this->music = Mix_LoadWAV("data/sons/rien.wav");
    }
    else{
        map<string, Mix_Music*>::iterator it_on_music = Music::global_music.find(music_name);
        if(it_on_music != Music::global_music.end()){
            this->music = it_on_music->second;
            //cout << it_on_music->first << '\t' << it_on_music->second << endl;
        }
        else{
            cout << "Music " << music_name << " not found" << endl;
            throw MusicNotFoundException();
        }
    }
    /*
    else{
        cout << "Music " << music_name << " not found" << endl;
        throw SoundNotFoundException();
    }

    if(this->music == nullptr && music_name != "None"){
        throw IncorrectPathForSoundException();
    }*/
}

void Music::playMusic(){
    if(this->music != nullptr && this->music_name != "None"){
        if(this->first_play){
        }
        Mix_PlayMusic(this->music, -1);
        this->first_play = true;
    }
}


void Music::fadeOut(int ms){
    if(this->music != nullptr){
        Mix_FadeOutMusic(ms);
    }
}


void Music::changeMusic(string music_name){
    this->assignMusic(music_name);
}

void Music::changeAndPlayMusic(string music_name){
    this->assignMusic(music_name);
    this->playMusic();
}


void Music::pause(){
    Mix_PauseMusic();
}

void Music::resume(){
    Mix_ResumeMusic();
}

Music::~Music(){
    //Mix_FreeChunk(this->music);
}
