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
    char filename[100]={0};
    char c;
    //查找第一个空格
    while(1){
        if(read(c_sockfd,&c,1) == 0){
            break;
        }
        if(c == ' '){
            break;
        }
    }
    //存文件名
    int i=0;
    while(1){
        if(read(c_sockfd,&c,1) ==0 ){
            break;
        }
        if(c == ' '){
            break;
        }
        filename[i++] = c;
    }
    printf("filename = %s\n",filename);
    //取后缀名
    char suffix[5]={0};
    i = strlen(filename)-1;
    while (1) {
        if(filename[i]=='.'){
            break;
        }
        i--;
    }
    i++;
    int j=0;
    while(1){
        c = filename[i];
        if(c == 0) break;

        i++;suffix[j++]=c;
    }
    //得到完整路径
    char path[100] ={0};
    sprintf(path,"/home/ren/Documents/Cfile%s",filename);
    printf("path=%s\n",path);
    //查找文件是否存在
    int fd = open(path,O_RDONLY);
    printf("fd=%d",fd);
    if(fd == -1){//文件不存在，返回404
        write(c_sockfd,"HTTP/1.1 404 Not Found\n\n",24);
        write(c_sockfd,"<h1>Resource Not Found!</h1>",28);
        shutdown(c_sockfd,SHUT_RDWR);//关闭与客户端的链接
        //close(c_sockfd);   shutdown完成四次挥手，数据可以保证发送到浏览器，close会中断数据发送
        pthread_exit(NULL);
    }
    //发送响应头
    write(c_sockfd,"HTTP/1.1 200 OK\n\n",15);
    //告诉浏览器返回的文件是什么类型
    if(strcmp(suffix,"html") == 0 || strcmp(suffix,"txt") == 0){
        write(c_sockfd ,"Content-Type: text/html; charset=UTF-8\n\n",40);
    }else if(strcmp(suffix,"jpg")==0 || strcmp(suffix,"jpeg")==0|| strcmp(suffix,"png")==0|| strcmp(suffix,"gif")==0){
        write(c_sockfd, "Content-Type: image/jpeg\n\n",26);
    }
    //发送响应体
    char buffer[1024];
    while(1){
        memset(buffer,0,sizeof(buffer));
        int len = read(fd,buffer,1024);
        write(c_sockfd,buffer,len);
        if(len < 1024){
            break;
        }
    }
    close(fd);
    shutdown(c_sockfd,SHUT_RDWR);
    pthread_exit(NULL);
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
        printf("received client : ip=%s,port=%d\n",inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
        //处理请求
        if(pthread_create(&ptid,NULL,process,&c_sockfd)== -1){
            perror("thread_create error!");
            exit(1);
        }
    }
    return 0;
}
