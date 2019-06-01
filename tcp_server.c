/*服务端TCP程序一般流程
 *1、创建socket
 *2、绑定端口和ip
 *3、监听socket
 *4、接收客户端的请求
 *5、从缓冲区中读取数据
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 5000

#define BACKLOG 5

int main()
{
    int listen_fd;
    int accept_fd;
    char buf[1024] = {0};//读写缓冲区
    struct sockaddr_in server_addr;
    //需要获取客户端相关信息
    struct sockaddr_in client_addr;
    socklen_t client_len;
    client_len = sizeof(client_addr);

    //创建socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    //绑定端口和ip
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //server_addr.sin_addr.s_addr = inet_addr(IP);

    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return 2;
    }

    //监听socket
    if (listen(listen_fd, BACKLOG) < 0) {
        perror("listen");
        return 3;
    }

    while(1) {
        //接收客户端的请求
        if ((accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len)) < 0) {
            perror("accept");
            return 4;
        }
        //打印客户端信息
        printf("connected with ip: %s: port:%d\n", 
            inet_ntop(AF_INET, &client_addr.sin_addr, buf, 1024), ntohs(client_addr.sin_port));

        while (1) {
            memset(buf, 0, sizeof(buf));
            //从缓冲区中读取数据
            ssize_t size = read(accept_fd, buf, sizeof(buf) - 1);
            if (size > 0)
                printf("client send: %s\n", buf);
            else if (size == 0) {
                printf("read done!\n");
                close(accept_fd);
                break;
            }
            else {
                perror("read");
                close(accept_fd);
                break;
            }   
        }
    }

    close(listen_fd);
    return 0;
}