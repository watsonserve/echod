#include "stdafx.h"

socklen_t SOCK_LEN = sizeof(struct sockaddr_in);

void* sender(void *args)
{
    register int sockfd, len, outtime;
    register struct req_form_t *form;
    struct sockaddr_in broad_addr;
    struct udp_gram_t req_buf;

    form = (struct req_form_t *)args;
    sockfd = form->sockfd;
    len = form->len;

    // 填写数据报头部
    req_buf.head.uuid = form->uuid;
    req_buf.head.len = len;
    req_buf.head.zero = 0;
    // 填写报文数据
    memcpy(req_buf.data, form->req, len);
    free(form);

    // 设置广播地址
    memset((void *)&broad_addr, 0, SOCK_LEN);
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_port = 0x0700;
    broad_addr.sin_addr.s_addr = 0xFFFFFFFF;

    req_buf.head.count = 0;
    // len = 报文长度
    len += sizeof(struct gram_head_t);
    for (outtime = 2; outtime < 8; outtime++)
    {
        req_buf.head.timestamp = now();
        if (-1 == sendto(sockfd, (void *)&req_buf, len, 0, (struct sockaddr *)&broad_addr, SOCK_LEN))
        {
            perror("sendto");
            break;
        }
        sleep(outtime);
        req_buf.head.count++;
    }

    exit(1);
    return NULL;
}

int udp_cli_init(struct udp_cli_t *self)
{
    self->count = 0;
    self->uuid = genUUID_4();
    // 建立socket
    self->sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 == self->sockfd)
    {
        return -1;
    }
    return 0;
}

int udp_cli_send(struct udp_cli_t *self, const char *req, unsigned short len)
{
    register struct req_form_t *form;
    int optval;

    form = malloc(sizeof(struct req_form_t));
    // 填写待发送数据表单
    form->sockfd = self->sockfd;
    form->uuid = self->uuid;
    form->len = len;
    form->req = req;

    // 设置广播模式
    optval = 1;
    if (-1 == setsockopt(self->sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)))
    {
        return errno;
    }

    return create_thread(sender, (void *)form);
}

int udp_cli_recv(struct udp_cli_t *self, struct resp_res_t *resp)
{
    socklen_t socklen;
    struct udp_gram_t response;
    register unsigned int len, recv_buf_siz;
    register struct sockaddr_in *srv_addr;

    socklen = SOCK_LEN;
    recv_buf_siz = sizeof(struct udp_gram_t);
    srv_addr = &(resp->srv_addr);

    do
    {
        memset((void *)srv_addr, 0, SOCK_LEN);
        len = recvfrom(self->sockfd, (void *)&response, recv_buf_siz, 0, (struct sockaddr *)srv_addr, &socklen);
    }
    while (memcmp(response.head.uuid.ch, self->uuid.ch, sizeof(guid_t)));
    close(self->sockfd);

    resp->lost = response.head.count;
    resp->rtt = now() - response.head.timestamp;
    // len = 返回数据长度
    len -= sizeof(struct gram_head_t);
    if (resp->data_len < len)
    {
        len = resp->data_len;
    }
    memcpy(resp->data, response.data, len);
    return len;
}
