#include <cstdio>
#include<iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include<inttypes.h>
void print(char * st);
void initialize(char *buffer);
void parser(char *buffer,int bytes);
//don't touch this
int commandType=0;
char command[4];
char ip1[15];
char ip2[15];
char length[3];
char message[1024];
//
int main(){

	int sockfd;
	int bind_flag;
	int bytes_received;
	unsigned int  addrlen;
	char buffer[1024];
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
    char temp[100];
    char temp2[100];
    int i,j;





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

        initialize(buffer);

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
            printf("byte %d\n",bytes_received);
		}
//		parser(buffer,bytes_received);
//		if(commandType==1)
//		{
//            printf("command %s  %d \n",command,strlen(command));
//		}
//		else if(commandType==2)
//		{
//            printf("command %s ip1 %s ip2 %s length %s message %s\n",command,ip1,ip2,length,message);
//		}
        //command parsing starts

        for(i=0,j=0;i<4;i++,j++)
        {
            command[j]=buffer[i];
        }
        command[j]='\0';
        commandType=0;
        //command parsing end
        if(strcmp(command,"show")==0)
        {

            commandType=1;
            //Source ip parsing starts
            for(i=4,j=0;i<8;i++,j++)
            {
                temp[j]=buffer[i];
            }
            temp[j]=0;

            inet_ntop(AF_INET,temp,ip1,sizeof(ip1));
            //printf("ip1 %s\n",ip1);
        //Source ip parsing end

        }
        else if(strcmp(command,"send")==0)
        {
            commandType=2;
            //Source ip parsing starts
            for(i=4,j=0;i<8;i++,j++)
            {
                temp[j]=buffer[i];
            }
            temp[j]=0;

            inet_ntop(AF_INET,temp,ip1,sizeof(ip1));
           // printf("ip1 %s\n",ip1);
        //Source ip parsing end
            //destination ip starts
            for(i=8,j=0;i<12;i++,j++)
            {
                temp[j]=buffer[i];
            }
            temp[j]=0;
            inet_ntop(AF_INET,temp,ip2,sizeof(ip2));
           // printf("ip2 %s\n",ip2);
            //destination ip parsing end

            //Length parsing starts
            temp[0]=buffer[12];
            temp[1]=buffer[13];
            temp[2]=0;
                inet_ntop(AF_INET,temp,temp2,sizeof(temp2));
            length[0]=temp2[0];
            length[1]=temp2[1];
            length[2]=0;
           // printf("length %s\n",length);
            //Length parsing end

            //message parsing starts
            for(i=14,j=0;i<bytes_received;i++,j++)
            {
                message[j]=buffer[i];
            }
            message[j]=0;

            //printf("message %s\n",message);
        }

        if(commandType==1)
		{
           printf("command show  %s \n",ip1);
		}
		else if(commandType==2)
		{
            printf("command send ip1 %s ip2 %s length %s message %s\n",ip1,ip2,length,message);
		}





		printf("[%s:%hu]: %s \n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
        //message parsing end


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
