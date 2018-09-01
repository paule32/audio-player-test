// provided by paule32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termcap.h>

#include <vlc/vlc.h>
#include <vlc/libvlc.h>

// -----------------------------
// clear console text screen ...
// -----------------------------
void clear_screen(void)
{
    char buf[1024];
    char *str;

    tgetent(buf, getenv("TERM"));
    str = tgetstr("cl", NULL);
    fputs(str, stdout);
}

// ----------------------------------------
// entry function for application start ...
// ----------------------------------------
int main(int argc, char **argv)
{
    libvlc_instance_t      * audio_inst;
    libvlc_media_player_t  * audio_mp;
    libvlc_media_t         * audio_m;

    clear_screen();

    if (argc < 2) {
        fprintf(stderr,"usage: player <stream>\n");
        return 1;
    }

    char const *vlc_argv[] =
    {
        "--no-xlib",  // tell VLC to not use Xlib
    };
    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    audio_inst = libvlc_new(vlc_argc,vlc_argv);
    if (audio_inst == NULL) {
        fprintf(stdout,"vlc init error.\n");
        return 1;
    }

    audio_m    = libvlc_media_new_location(audio_inst,argv[1]);
    audio_mp   = libvlc_media_player_new_from_media(audio_m);
                 libvlc_media_release(audio_m);

    struct libvlc_audio_output_t *audio_info;
    audio_info = libvlc_audio_output_list_get(audio_inst);

    if (audio_info == NULL) {
        fprintf(stderr,"no audio infos.\n");
        goto error;
    }

    fprintf(stdout,"please select output:\n\n");
    int audio_num = 0;
    char *audio_output[10];

    while (audio_info) {
        audio_output[audio_num] = (char*) malloc(strlen(audio_info->psz_name));  strcpy(
        audio_output[audio_num] , audio_info->psz_name);

        fprintf(stdout,"%d - %-10s: %s\n",
        audio_num++,
        audio_info->psz_name,
        audio_info->psz_description);

        if (!audio_info->p_next)
        break;

        audio_info = audio_info->p_next;
    }

    int user_act = 0;

    fprintf(stdout,"%d - Exit.\n\n",audio_num);
    fprintf(stdout,":> ");
    scanf("%d",&user_act);

    // exit ?
    if (user_act == audio_num)
    goto ende;

    fprintf(stdout,"user select: %s\n\n", audio_output[user_act]);

    libvlc_audio_output_set(
    audio_mp,
    audio_output[user_act]);

    // device(s)
    struct          libvlc_audio_output_device_t *audio_devinfo;
    audio_devinfo = libvlc_audio_output_device_list_get(audio_inst,audio_output[user_act]);
    char *audio_device[10];
    int   audio_dec =   0;

    if (audio_devinfo == NULL) {
        fprintf(stdout,"no device found.\n");
        goto error;
    }

    fprintf(stdout,"please select output device:\n");
    while (audio_devinfo) {
        audio_device[audio_num] = (char*) malloc(strlen(audio_devinfo->psz_device));  strcpy(
        audio_device[audio_num] , audio_devinfo->psz_device);

        fprintf(stdout,"%d - %-10s: %s\n",
        audio_dec++,
        audio_devinfo->psz_device,
        audio_devinfo->psz_description);

        if (!audio_devinfo->p_next)
        break;

        audio_devinfo = audio_devinfo->p_next;
    }

    fprintf(stdout,"%d - Exit.\n\n",audio_num);
    fprintf(stdout,":> ");
    scanf("%d",&user_act);
    audio_dec = user_act ;

    libvlc_audio_output_device_set(
    audio_mp,
    audio_output[audio_num] ,
    audio_device[audio_dec]);

//     int volume; volume =
//     libvlc_audio_get_volume(audio_mp);  printf("\n-->> %d\n",volume);
//     libvlc_audio_set_volume(audio_mp,90);

    // -----------------
    // start playing ...
    // -----------------
    libvlc_media_player_play(audio_mp);
    sleep(60);

    error:
    ende:
    libvlc_media_player_stop(audio_mp);
    libvlc_media_player_release(audio_mp);

    libvlc_audio_output_device_list_release(audio_devinfo);
    libvlc_audio_output_list_release(audio_info);

    libvlc_release(audio_inst);

    return 0;
}
