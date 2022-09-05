
#pragma once

#include <unistd.h>
#include <sys/time.h>
#include <iostream>

# define FPS60_FRAMETIME_MS 15800
# define FPS60_SLEEPTIME_MS 15650 // remove usleep time cost to be straight at 60FPS
static int            frameCap(void)
{
    static struct timeval  last_time = {.tv_sec=0, .tv_usec=0};
    static int             fps = 0;
    struct timeval         time;

    struct timeval         before_frame_time;
    gettimeofday(&before_frame_time, NULL);

    time = before_frame_time;

    if (last_time.tv_sec == 0)
        last_time = time;

    if (time.tv_sec > last_time.tv_sec)
    {
        int r = fps;
        last_time = time;
        fps = 0;
		return r;
    }
    
    struct timeval         after_frame_time;
    gettimeofday(&after_frame_time, NULL);

    if (after_frame_time.tv_sec == before_frame_time.tv_sec)
    {
        if (after_frame_time.tv_usec - before_frame_time.tv_usec < FPS60_FRAMETIME_MS)
            usleep(FPS60_SLEEPTIME_MS - (after_frame_time.tv_usec - before_frame_time.tv_usec));
    }
    else
    {
        if ((after_frame_time.tv_usec + 1000000) - before_frame_time.tv_usec < FPS60_FRAMETIME_MS) 
            usleep(FPS60_SLEEPTIME_MS - ((after_frame_time.tv_usec + 1000000) - before_frame_time.tv_usec));
    }
    fps++;
	return -1;
}


