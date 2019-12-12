#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define BUF_SIZ 4096

typedef void *(*thread_function_t)(void *);

struct send_gram_t
{
    int sockfd;
    int len;
    const char *req;
};

void* sender(void *args)
{
    register int sockfd, len, outtime;
    register struct send_gram_t *form;
    register socklen_t socklen;
    struct sockaddr_in broad_addr;
    const char *req;

    socklen = sizeof(struct sockaddr_in);
    form = (struct send_gram_t *)args;
    sockfd = form->sockfd;
    len = form->len;
    req = form->req;

    memset((void *)&broad_addr, 0, socklen);
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_port = 0x0700;
    broad_addr.sin_addr.s_addr = 0xFFFFFFFF;

    for (outtime = 2; outtime < 8; outtime++)
    {
        if (-1 == sendto(sockfd, req, len, 0, (struct sockaddr *)&broad_addr, socklen))
        {
            perror("sendto");
            break;
        }
        sleep(outtime);
    }
    exit(1);
    return NULL;
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

int main(int argc, const char *argv[])
{
    socklen_t socklen;
    int sockfd, len, err, optval = 1;
    unsigned char *addr;
    const char *req;
    char resp[BUF_SIZ];
    struct sockaddr_in broad_addr, srv_addr;
    struct send_gram_t form;

    if (argc < 2)
    {
        return 0;
    }

    req = argv[argc - 1];
    len = strnlen(req, BUF_SIZ);

    if (BUF_SIZ <= len)
    {
        return 1;
    }

    socklen = sizeof(struct sockaddr_in);
    memset((void *)&srv_addr, 0, socklen);

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 == sockfd)
    {
        perror("socket");
        return 1;
    }

    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)))
    {
        perror("setsockopt SO_BROADCAST");
        return 1;
    }

    form.sockfd = sockfd;
    form.len = len;
    form.req = req;
    err = create_thread(sender, (void *)&form);
    if (err) {
        perror("thread");
        return 1;
    }

    len = recvfrom(sockfd, (void *)resp, BUF_SIZ, 0, (struct sockaddr *)&srv_addr, &socklen);
    if (-1 == len)
    {
        perror("recvfrom");
        return 1;
    }

    addr = (unsigned char *)&srv_addr.sin_addr;
    resp[len] = 0;
    printf("%u.%u.%u.%u: %s\n", addr[0], addr[1], addr[2], addr[3], resp);
    return 0;
}
