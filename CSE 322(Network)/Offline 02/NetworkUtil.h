#include <cstdio>
#include <cstring>
#include <cstdlib>
#include<iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define port 4747
#define bufferSize 1024
#define INFINITY 100
#define UNDEFINED "000.000.000.000"
class NetworkUtil
{
    int sockfd;
    int bind_flag;
    int sent_bytes;
    char buffer[bufferSize];
    struct sockaddr_in myAddress;
    struct sockaddr_in toAddress;
    struct sockaddr_in fromAddress;
    int bytes_received;
    unsigned int  addrlen;
    public:

    NetworkUtil()
    {
    }


int openSocket(char * ipAddress)
{
    if(ipAddress)
    {
        myAddress.sin_family = AF_INET;
        myAddress.sin_port = htons(port);
	//client_address.sin_addr.s_addr = INADDR_ANY;
        inet_pton(AF_INET,ipAddress, &myAddress.sin_addr);

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);

        bind_flag = bind(sockfd, (struct sockaddr*) &myAddress, sizeof(sockaddr_in));
        if(bind_flag==0)
        {
            printf("successful bind\n");
            return 1;
        }
        else
        {
            printf("%d Error in binding\n",bind_flag);
            perror("bind");
            return -1;
        }
    }
    else
    {
        printf("Invalid ipAddress\n");
        return -1;
    }
}
~NetworkUtil()
{
    socketClose();
}

char *read()
{

    addrlen=sizeof(fromAddress);
    bytes_received=recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &fromAddress, &addrlen);
    if(bytes_received==-1)
		{
            perror("recvfrom");
            printf("error %s\n",strerror(errno));
            return NULL;
		}
		else
		{
            buffer[bytes_received]=0;
            return buffer;
        }

}
char * readFrom()
{
    return inet_ntoa(fromAddress.sin_addr);
}
int get_bytes_received()
{
    return bytes_received;
}

int write(char *ipAddress,char *message)
{
    toAddress.sin_family = AF_INET;
	toAddress.sin_port = htons(port);
	//myAddress.sin_addr.s_addr = inet_addr("192.168.10.100");
	inet_pton(AF_INET,ipAddress,&toAddress.sin_addr);
    sent_bytes=sendto(sockfd, message, strlen(message), 0, (struct sockaddr*) &toAddress, sizeof(sockaddr_in));
    if(sent_bytes==-1)
    {
        perror("sendto");
    }
    else
    {

        //printf("data sent bytes %d  size :%lu\n",sent_bytes,strlen(message));
        return sent_bytes;
    }
}
void socketClose()
{
    close(sockfd);
}

};




