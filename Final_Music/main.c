#include "Timer.h"
#include "open_interface.h"


int main(void)
{

    oi_t* sensor = oi_alloc();
    oi_init(sensor);


     uint8_t badLandingSpotNumNotes = 5;
     uint8_t badLandingSpotNotes[5] = {72,76,72,76,72} ;
     uint8_t badLandingSpotDurations[5] = {8,8,8,8,8};



      oi_loadSong(1,badLandingSpotNumNotes, badLandingSpotNotes, badLandingSpotDurations);
      oi_play_song(1);
//    load_songs(1);









      oi_free(sensor);
	return 0;
}
