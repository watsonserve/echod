#include "stdafx.h"

int main(int argc, const char *argv[])
{
    int len, err;
    unsigned char *addr;
    const char *req;
    char data[MAX_GRAM_SIZ];
    struct udp_cli_t cli;
    struct resp_res_t resp;

    // 缺乏参数
    if (argc < 2)
    {
        return 0;
    }

    // 取最后一个参数，并获取其长度
    req = argv[argc - 1];
    len = strnlen(req, MAX_GRAM_SIZ);

    // 输入内容长度超过最大长度限制，报错
    if (MAX_GRAM_SIZ <= len)
    {
        fprintf(stderr, "ERROR: max 496byte\n");
        return 1;
    }

    if (udp_cli_init(&cli))
    {
        perror("upd client init");
    }

    err = udp_cli_send(&cli, req, len);
    if (err)
    {
        perror("setsockopt SO_BROADCAST");
    }

    resp.data = data;
    len = udp_cli_recv(&cli, &resp);
    data[len] = 0;

    addr = (unsigned char *)&resp.srv_addr.sin_addr;
    printf(
        "IP: %u.%u.%u.%u, lost: %d, RTT: %d\n%s\n",
        addr[0], addr[1], addr[2], addr[3], resp.lost, resp.rtt, data
    );
    return 0;
}
