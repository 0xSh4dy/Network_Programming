#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netdb.h>
#include<arpa/inet.h>
#define PORT "9999"

void* get_in_addr(struct sockaddr* sa){
    if(sa->sa_family==AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);

}
int main(){
    struct addrinfo addrStruct,*servinfo,*p;
    int status,socketfd;
    struct sockaddr_storage incomingAddr;

    memset(&addrStruct,0,sizeof(addrStruct));
    addrStruct.ai_family = AF_INET6;
    addrStruct.ai_socktype = SOCK_DGRAM;
    addrStruct.ai_flags=  AI_PASSIVE;
    status = getaddrinfo(NULL,PORT,&addrStruct,&servinfo);
    if(status!=0){
        fprintf(stderr,"%s\n",gai_strerror(status));
        return 2;
    }
    for(p=servinfo;p!=NULL;p=p->ai_next){
        socketfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(socketfd==-1){
            perror("listener:socket");
            continue;
        }
        if(bind(socketfd,p->ai_addr,p->ai_addrlen)==-1){
            close(socketfd);
            perror("bind");
            continue;
        }
        break;
    }
    if(p==NULL){
        fprintf(stderr,"Failed to bind sockets\n");
        return 2;
    }
    freeaddrinfo(servinfo);
    socklen_t len = sizeof(incomingAddr);
    puts("Listener waiting to receive packets")    ;
    char buf[100];
    int count;
    count = recvfrom(socketfd,buf,99,0,(struct sockaddr*)&incomingAddr,&len);
    if(count==-1){
        perror("recv from");
        exit(1);
    }
    buf[count]='\0';
    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop(incomingAddr.ss_family,get_in_addr((struct sockaddr*)&incomingAddr),ipstr,sizeof(ipstr));
    printf("Received %s from %s\n",buf,ipstr);
    return 0;
}