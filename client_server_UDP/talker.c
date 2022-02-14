#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#define TARGET "target_ipv6_address"
#define PORT "9999"

int main(){
    struct addrinfo addrStruct,*servinfo,*p;
    int socketfd,status;
    memset(&addrStruct,0,sizeof(addrStruct));
    addrStruct.ai_family = AF_INET6;
    addrStruct.ai_socktype = SOCK_DGRAM;
    status = getaddrinfo(TARGET,PORT,&addrStruct,&servinfo);
    if(status!=0){
        fprintf(stderr,"%s\n",gai_strerror(status));
        return 2;
    }
    for(p=servinfo;p!=NULL;p=p->ai_next){
        socketfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(socketfd==-1){
            perror("talker-socket");
            continue;
        }
        break;
    }
    if(p==NULL){
        fprintf(stderr,"Failed to create socket\n");
        return 2;
    }
    int bytecount;
    bytecount = sendto(socketfd,"Hello, world",13,0,p->ai_addr,p->ai_addrlen);
    if(bytecount==-1){
        perror("talker:sendto");
        exit(1);
    }
    freeaddrinfo(servinfo);
    printf("Sent %d bytes to %s\n",bytecount,TARGET);
    close(socketfd);
    return 0;
}