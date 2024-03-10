#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("create socket failed: ");
        return -1;
    }

    /* 初始化服务端地址及端口 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(30000);

    /* bind */
    int ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind failed: ");
        return -1;
    }

    /* listen */
    if (listen(sockfd, SOMAXCONN) < 0) {
        perror("listen failed: ");
        return -1;
    }

    /* use to store client addr */
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    memset(&client_addr, 0, client_addr_len);

    /* accept */
    int conn_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (conn_fd < 0) {
        perror("accept failed: ");
        return -1;
    }

    char client_ip_str[INET_ADDRSTRLEN];
    const char *client_ip = inet_ntop(AF_INET, &client_addr.sin_addr, client_ip_str, INET_ADDRSTRLEN);
    unsigned short client_port = ntohs(client_addr.sin_port);
    printf("A new connection is established: conn_fd: %d, ip: %s, port: %d\n", conn_fd, client_ip, client_port);

    sleep(100);

    close(sockfd);

    return 0;
}
