#define _GLIBCXX_USE_CXX11_ABI 0
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include<iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include"Table.h"
#include<string>
#include<iostream>
using namespace std;

int main(int argc, char *argv[]){

	int sockfd;
	int bind_flag;
	int sent_bytes;
	char buffer[1024];
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	if(argc != 2){
		//printf("%s <ip address>\n", argv[0]);
		exit(1);
	}




	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(4747);
	//server_address.sin_addr.s_addr = inet_addr("192.168.10.100");
	inet_pton(AF_INET,"192.168.10.1",&server_address.sin_addr);

	client_address.sin_family = AF_INET;
	client_address.sin_port = htons(4747);

	//client_address.sin_addr.s_addr = inet_addr(argv[1]);
	inet_pton(AF_INET,argv[1],&client_address.sin_addr);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind_flag = bind(sockfd, (struct sockaddr*) &client_address, sizeof(sockaddr_in));


	if(bind_flag==0)
	{
        //printf("successful bind\n");
	}
	else
	{
        //printf("%d Error in binding\n",bind_flag);
        perror("bind");
	}
    int a,d;
    char  b[50];
    char c[50];
    printf("now here\n");
	while(true){
		scanf("%d ",&a);
		table msg;
		//printf("before\n");
		for(int i=0;i<a;i++)
		{
            //fgets (b, 50, stdin);
            //fgets (c, 50, stdin);
            cin>>b>>c>>d;
            //printf("now %s %s %d\n",b,c,d);
            msg.insertEntry(b,c,d);
		}

		char* tmp = msg.getasString();
		printf("did well\n%s\n",tmp);

		sent_bytes=sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
		if(sent_bytes==-1)
		{
            perror("sendto");
		}
		else
		{
            perror("sendto");
            printf("data sent bytes %d  size :%d\n",sent_bytes,strlen(tmp));
		}
		free(tmp);

		if(!strcmp(buffer, "shutdown")) break;
	}

	close(sockfd);

	return 0;

}

