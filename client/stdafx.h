#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define BUF_SIZ 512
#define HEAD_SIZ 32
#define MAX_GRAM_SIZ 480

socklen_t SOCK_LEN;

typedef union
{
    uint64_t int64[2];
    uint32_t int32[4];
    unsigned char ch[16];
} guid_t;

typedef void *(*thread_function_t)(void *);

struct gram_head_t
{
    guid_t uuid;
    uint64_t timestamp;
    uint16_t count;
    uint16_t len;
    uint32_t zero;
};

struct udp_gram_t
{
    struct gram_head_t head;
    char data[MAX_GRAM_SIZ];
};

struct req_form_t
{
    int sockfd;
    int len;
    const char *req;
    guid_t uuid;
};

struct resp_res_t
{
    uint16_t lost;
    int16_t data_len;
    uint32_t rtt;
    char *data;
    struct sockaddr_in srv_addr;
};

struct udp_cli_t
{
    int sockfd;
    int32_t count;
    int64_t timestamp;
    int64_t start_time;
    guid_t uuid;
};

void* sender(void *);

guid_t genUUID_4();

uint64_t now();

int create_thread(thread_function_t, void *);

int udp_cli_init(struct udp_cli_t *self);

int udp_cli_send(struct udp_cli_t *self, const char *req, unsigned short len);

int udp_cli_recv(struct udp_cli_t *self, struct resp_res_t *resp);

#endif
