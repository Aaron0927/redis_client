#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>

int fs_open(char *ip, unsigned short port)
{
    int fd; 
    int recbytes;
    int sin_size;
    struct sockaddr_in s_add, c_add; 

    printf("Open file!\r\n");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        printf("open fail! \r\n");
        return -1;
    }

    bzero(&s_add, sizeof(struct sockaddr_in)); // 清空结构体
    s_add.sin_family = AF_INET; // 采用IPv4网络协议
    s_add.sin_addr.s_addr= inet_addr(ip); 
    s_add.sin_port = htons(port); 

    if(-1 == connect(fd, (struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        printf("open fail!\r\n");
        return -1;
    }

    printf("open ok!\r\n");                    
    return fd;
}

void fs_read(int fd, char *fname)
{
    char buffer[1024] = "get ";
    char rcv[1024] = {0};
    strcat(buffer, fname);
    strcat(buffer, " \r\n");
    printf("read : %s", buffer);
    send(fd, buffer, sizeof(buffer), 0);

    if (read(fd, rcv, 1024) == -1) 
    {
        printf("read error!\n");
        return;
    }
    printf("read OK!\n");
    printf("RCV : %s\n", rcv);
    return;

}

void fs_write(int fd, char *fname, char *buf)
{
    char buffer[1024] = "set ";
    char rcv[1024] = {0};
    strcat(buffer, fname);
    strcat(buffer, " \"");
    strcat(buffer, buf);
    strcat(buffer, "\"\n");
    printf("write : %s", buffer);
    send(fd, buffer, sizeof(buffer), 0);
    if (read(fd, rcv, 1024) == -1) 
    {
        printf("write error!\n");
        return;
    }
    printf("write OK!\n");
    printf("RCV : %s\n", rcv);
}

void fs_close(int fd)
{
    close(fd);
    printf("file close OK!\n");
}


int main()
{
    int fd = fs_open("127.0.0.1", 6379);
    char filename[20] = "index.html";
    char buf[1024] = "<html><head><title>my home</title></head><body>hello mini-os!</body></html>";    
    fs_write(fd, filename, buf);
    fs_close(fd);
    
    fd = fs_open("127.0.0.1", 6379);
    fs_read(fd, filename);
    fs_close(fd);
    return 0;

}
