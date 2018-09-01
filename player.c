// provided by paule32
#include <stdio.h>
#include <stdlib.h>

#include <vlc/vlc.h>
#include <vlc/libvlc.h>

int main(int argc, char **argv)
{
    libvlc_instance_t      * audio_inst;
    libvlc_media_player_t  * audio_mp;
    libvlc_media_t         * audio_m;

    audio_inst = libvlc_new(0,NULL);
    audio_m    = libvlc_media_new_location(audio_inst,"<dein stream>");
    audio_mp   = libvlc_media_player_new_from_media(audio_m);
                 libvlc_media_release(audio_m);

    libvlc_media_player_play(audio_mp);
    sleep(10);

    libvlc_media_player_release(audio_mp);
    libvlc_release(audio_inst);

    return 0;
}
