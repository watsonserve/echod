#include "stdafx.h"

guid_t genUUID_4()
{
    unsigned char i;
    guid_t ret;

    for (i = 0; i < 4; i++)
    {
        ret.int32[i] = random();
    }
    return ret;
}

uint64_t now()
{
    struct timespec tsp;
    if (clock_gettime(CLOCK_REALTIME, &tsp))
        return 0;
    return tsp.tv_sec * 1000 + tsp.tv_nsec / 1000000;
}

int create_thread(thread_function_t static_route, void *form)
{
    pthread_t tid;
    register int err = 0;

    err = pthread_create(&tid, NULL, static_route, form);
    if (!err)
    {
        err = pthread_detach(tid);
    }
    return err;
}
