#include <cstdio>
#include<iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include<inttypes.h>
#include"Table.h"
#include<string>
using namespace std;
void print(char * st);
void initialize(char *buffer);

int main(){

	int sockfd;
	int bind_flag;
	int bytes_received;
	unsigned int  addrlen;
	char buffer[4000];
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;




	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(4747);
	//client_address.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET,"192.168.10.1", &server_address.sin_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bind_flag = bind(sockfd, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
	if(bind_flag==0)
	{
        printf("successful bind\n");
	}
	else
	{
        printf("%d Error in binding\n",bind_flag);
        perror("bind");
	}
    addrlen=sizeof(client_address);
	while(true){

        //initialize(buffer);

		bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &client_address, &addrlen);

		//print(buffer);
		if(bytes_received==-1)
		{
            perror("recvfrom");
            printf("error %s\n",strerror(errno));
		}
		else
		{
            buffer[bytes_received]=0;
            printf("byte %d size :%d\n",bytes_received,strlen(buffer));
            table msg(buffer);
//            int j=msg.getLength();
//            entry* temp=msg.getTable();
//            for(int i=0;i<j;i++)
//            {
//               // entry *r=msg->getEntry(i);
//                printf("%s %s %d\n",temp[i].destination,temp[i].next_hop,temp[i].cost);
//                //cout<<temp[i].destination<<"   "<<temp[i].next_hop<<"  "<<temp[i].cost<<endl;
//
//            }
		}



		printf("[%s:%hu]: %s \n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);




       // printf("%" PRIu32 "\n",a);

		printf("%hu\n",client_address.sin_port);
		printf("%u\n",client_address.sin_addr.s_addr);
		if(strcmp(buffer,"shutdown")==0)break;

	}

	return 0;

}
void initialize(char *buffer)
{
    int i;
    for(i=0;i<1024;i++)
    {
        buffer[i]=0;

    }
}
void print(char * st)
{
  printf("printing\n");
  for(int i=0;(i<1024)&&(st[i]!='\n');i++)
  {
        printf("%d\n",st[i]);

  }
}
