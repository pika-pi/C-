#include<stdio.h>
#include<pthread.h>//多线程
#include<fcntl.h> //读取文件
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>

#define PORT 50000
void* process(void* arg){
    int c_sockfd = *((int *)arg);
    printf("c_sockft=%d\n",c_sockfd);
    char c;
    while(1){
        if(read(c_sockfd,&c,1) == 0 ){
            break;
        }
        printf("%c",c);
    }
    printf("client over!\n");
}

int main(void){
    struct sockaddr_in s_addr,c_addr;
    int addrlen = sizeof(struct sockaddr_in);
    //创建socket描述符
    int s_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(s_sockfd == -1){
        perror("socket error!");
        exit(1);
    }

    //绑定地址和端口
    memset(&s_addr , 0 , addrlen);//将创建的结构体清空，初次创建未赋值时，内容是随机的。
    memset(&c_addr,0,addrlen);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = inet_addr("192.168.66.227");
    //设置地址复用
    int opt=1;
    setsockopt(s_sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //绑定
    if(bind(s_sockfd,(const struct sockaddr *)&s_addr,addrlen) == -1){
        perror("bind error!");
        exit(1);
    }
    printf("bind successful !\n");
    //监听
    if(listen(s_sockfd,10) == -1){
        perror("listen error!\n");
        exit(1);
    }
    printf("listen successful \n");
    pthread_t ptid;
    while(1){
        //接受请求
        int c_sockfd = accept(s_sockfd,(struct sockaddr *)&c_addr,&addrlen);
        if(c_sockfd == -1){
            perror("accept error!");
            exit(1);
        }
        printf("received client : ip=%s,port=%s\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
        //处理请求
        if(pthread_create(&ptid,NULL,process,&c_sockfd)== -1){
            perror("thread_create error!");
            exit(1);
        }
    }
    return 0;
}