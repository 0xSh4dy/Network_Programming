#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#define PORT "10001"
#define HOSTNAME "127.0.0.1"
#define MAXSIZE 50
void *get_in_addr(struct sockaddr* sa){
    if(sa->sa_family==AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(){
    struct addrinfo addrStruct,*res,*p;
    int status,socketfd,bytecount;
    char ipstr[INET6_ADDRSTRLEN];
    char data[MAXSIZE];
    memset(&addrStruct,0,sizeof(addrStruct));
    addrStruct.ai_family = AF_UNSPEC;
    addrStruct.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(HOSTNAME,PORT,&addrStruct,&res);
    if(status!=0){
        fprintf(stderr,"%s", gai_strerror(status));
        return 0;
    }
    for(p=res;p!=NULL;p=p->ai_next){
        socketfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(socketfd==-1){
            perror("client-socket");
            continue;
        }
        if(connect(socketfd,p->ai_addr,p->ai_addrlen)==-1){
            close(socketfd);
            perror("client-connect");
            continue;
        }
        break;
    }
    if(p==NULL){
        fprintf(stderr,"Client failed to connect\n");
        return 2;
    }
    inet_ntop(p->ai_family,get_in_addr((struct sockaddr*)p->ai_addr),ipstr,sizeof(ipstr));
    printf("Connecting to %s\n",ipstr);
    freeaddrinfo(res);
    bytecount = recv(socketfd,data,MAXSIZE,0);
    if(bytecount==-1){
        perror("receive");
        exit(1);
    }
    data[bytecount]='\0';
    fprintf(stdout,"Received data: %s\n",data);
    close(socketfd);
    return 0;
}