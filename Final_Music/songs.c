#include "Timer.h"
#include "open_interface.h"
#include "songs.h"

//    oi_t* sensor = io_alloc();
//    oi_init(sensor);


static uint8_t badLandingSpotNumNotes = 5;
static uint8_t badLandingSpotNotes[5] = {72,76,72,76,72} ;
static uint8_t badLandingSpotDurations[5] = {8,8,8,8,8};

static uint8_t goodLandingSpotNumNotes = 53;
static uint8_t goodLandingSpotNotes[53] = {55,60,64,60,60,60,60,65,65,55,65,55,60,64,55,
        57,59,60,56,58,60,55,60,64,62,64,62,65,
        62,65,65,65,65,64,60,60,60,58,62,64,
        60,55,55,60,55,65,59,65,60,60,60,60,60};
static uint8_t goodLandingSpotDurations[53] = {18, 18, 18, 18, 18, 18,  9,  9, 18, 18, 45,  9, 54, 54, 54, 9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45, 9,
                                               54, 54, 54,  9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45, 9, 54, 54, 54, 18, 18,  9,  9, 54, 54, 54, 18};

static uint8_t scanningNumNotes = 1;
static uint8_t scanningNotes[1] = {32};
static uint8_t scanningDurations[1] = {16};


void load_songs(uint8_t songs_title){

    switch(songs_title){

        case 0:
              oi_loadSong(0, goodLandingSpotNumNotes, goodLandingSpotNotes, goodLandingSpotDurations);
              oi_play_song(0);
              break;

        case 1:
              oi_loadSong(1,badLandingSpotNumNotes, badLandingSpotNotes, badLandingSpotDurations);
              oi_play_song(1);
              break;

        case 2:
              oi_loadSong(2, scanningNumNotes, scanningNotes, scanningDurations);
              oi_play_song(2);
              break;

        default:

            break;

}
}
