/*客户端TCP程序一般流程
 *1、创建socket
 *2、向服务端发起连接
 *3、向缓冲区中写入数据
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


int main()
{
    int client_fd;
    char buf[1024] = {0};//读写缓冲区
    struct sockaddr_in client_addr;
    ssize_t size;

    //创建socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    //填充ip端口信息
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //向服务端发起连接
    if (connect(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("connect");
        return 2;
    }

    while(1) {
        memset(buf, 0, sizeof(buf));
        //从标准输入获取字符串
        if (!gets(buf)) {
            perror("gets");
            return 3;
        }
        if (strcmp(buf, "quit") == 0)
            break;
        if (write(client_fd, buf, strlen(buf)) != strlen(buf)) {
            perror("write");
            return 4;
        }
    }

    close(client_fd);
    return 0;
}
