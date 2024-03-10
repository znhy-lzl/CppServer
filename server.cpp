#include "util.h"
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd < 0, "socket create failed: ");

    /* 初始化服务端地址及端口 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(30000);

    /* bind */
    int ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    errif(ret < 0, "socket bind failed: ");

    /* listen */
    errif(listen(sockfd, SOMAXCONN) < 0, "listen failed: ");

    /* use to store client addr */
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    memset(&client_addr, 0, client_addr_len);

    /* accept */
    int conn_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    errif(conn_fd < 0, "accept failed: ");

    char client_ip_str[INET_ADDRSTRLEN];
    const char *client_ip = inet_ntop(AF_INET, &client_addr.sin_addr, client_ip_str, INET_ADDRSTRLEN);
    unsigned short client_port = ntohs(client_addr.sin_port);
    printf("A new connection is established: conn_fd: %d, ip: %s, port: %d\n", conn_fd, client_ip, client_port);

    while (true) {
        char buf[1024] = {0};
        ssize_t read_bytes = read(conn_fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            printf("message from client fd %d: %s\n", conn_fd, buf);
            write(conn_fd, buf, sizeof(buf));
        } else if (read_bytes == 0) {
            printf("client fd %d disconnected\n", conn_fd);
            close(conn_fd);
            break;
        } else if (read_bytes == -1) {
            close(conn_fd);
            errif(true, "socket read error: ");
        }
    }

    close(sockfd);

    return 0;
}
