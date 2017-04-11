#include"NetworkUtil.h"
#include "Table.h"
#include<fstream>
#include<iostream>
#define down 3
int currentClock=0;
table routerTable;
table savedCopy;
char *myIPAddress;
NetworkUtil nc;
SeenTable lastseen;
void loader(char* ipAddress,char *fileName)
{


    char a[15] ;
    char b[15];
    int c;

    ifstream  topo;
    topo.open(fileName);
    string st;
    routerTable.insertEntry(myIPAddress,myIPAddress,0);
    savedCopy.insertEntry(myIPAddress,myIPAddress,0);
    while(topo>>a>>b>>c)
    {
        if(strcmp(a,ipAddress)==0)
        {
            routerTable.insertEntry(b,b,c);
            savedCopy.insertEntry(b,b,c);
            lastseen.insertEntry(b,0);
        }
        else if(strcmp(b,ipAddress)==0)
        {
            routerTable.insertEntry(a,a,c);
            savedCopy.insertEntry(a,a,c);
            lastseen.insertEntry(a,0);
        }


    }
    topo.close();
    topo.open(fileName);
    while(topo>>a>>b>>c)
    {
            if(!routerTable.isAvailable(a))
            {
                routerTable.insertEntry(a,UNDEFINED,INFINITY);
                savedCopy.insertEntry(a,UNDEFINED,INFINITY);
                lastseen.insertEntry(a,link_down);

            }
            if(!routerTable.isAvailable(b))
            {
                routerTable.insertEntry(b,UNDEFINED,INFINITY);
                savedCopy.insertEntry(b,UNDEFINED,INFINITY);
                lastseen.insertEntry(b,link_down);

            }
    }

    printf("-----------------Initial Router Table of %s-----------------\n",myIPAddress);
    routerTable.printTable();
    printf("-----------------++++++++++++++------------------------\n");
    topo.close();
    return ;


}


void sendTable()
{
    entry * allEntry=savedCopy.getTable();
    char *table_as_string=routerTable.getasString();

    for(int i=0;i<savedCopy.getLength();i++)
    {
        if((strcmp(allEntry[i].destination,allEntry[i].next_hop)==0)&&(strcmp(allEntry[i].destination,myIPAddress)!=0))
        {
            nc.write(allEntry[i].destination,table_as_string);
        }
    }
}
void parser(char *message,int bytes)
{

    int commandType=0;
    char ip1[15];
    char ip2[15];
    int length;
    char messageToPass[bufferSize];


    char temp[100];
    char temp2[100];
    int i,j;


        for(i=0,j=0;i<4;i++,j++)
        {
            temp[j]=message[i];
        }
        temp[j]='\0';
        commandType=0;
        //command parsing end
    if(strcmp(temp,"show")==0)
        {

            commandType=1;
            //Source ip parsing starts
            for(i=4,j=0;i<8;i++,j++)
            {
                temp[j]=message[i];
            }
            temp[j]=0;

            inet_ntop(AF_INET,temp,ip1,sizeof(ip1));
            if(strcmp(ip1,myIPAddress)==0)
            {
                printf("-----------------Router Table of %s-----------------\n",myIPAddress);
                routerTable.printTable();
                printf("-----------------++++++++++++++------------------------\n");
            }

        }
        else if(strcmp(temp,"send")==0)
        {
            commandType=2;
            inet_ntop(AF_INET,message+4,ip1,sizeof(ip1));
            inet_ntop(AF_INET,message+8,ip2,sizeof(ip2));

            length=(message[13]<<8)|(message[12]);
            for(i=14,j=0;i<bytes;i++,j++)
            {
                messageToPass[j]=message[i];
            }
            messageToPass[j]=0;


            if(strcmp(ip2,myIPAddress)==0)
            {
                printf("Packet Reached Destination(printed by %s)\n",myIPAddress);

            }
            else
            {
             entry *all=routerTable.getTable();
                for(i=0;i<routerTable.getLength();i++)
                {
                    if(strcmp(all[i].destination,ip2)==0)
                    {
                        if(strcmp(all[i].next_hop,UNDEFINED)==0)
                        {
                            printf("Packet can't be forwarded, link down(printed by %s)\n",myIPAddress);
                        }
                        else
                        {
                            nc.write(all[i].next_hop,message);
                            printf("Packet Forwraded To %s (printed by %s)\n",all[i].next_hop,myIPAddress);
                        }


                    }
                }

            }



            //printf("message %s\n",message);
        }
        else if((temp[0]=='c')&&(temp[1]=='l')&&(temp[2]='k'))
        {
            commandType=3;
            currentClock++;
           // printf("Now Clock: %s\n",message);
            seenEntry * all=lastseen.getAll();
            for(int k=0;k<lastseen.getLength();k++)
            {
                if((all[k].lastSeen<=currentClock-down)&&(all[k].lastSeen!=link_down))
                {
                    printf("-------%s link down----------\n",all[k].ip);
                    routerTable.setInfinity(all[k].ip);
                    all[k].lastSeen=link_down;
                }
            }

            sendTable();

        }
        else if((temp[0]=='c')&&(temp[1]=='o')&&(temp[2]='s'))
        {
            commandType=4;
            //Source ip parsing starts
            inet_ntop(AF_INET,message+4,ip1,sizeof(ip1));
            inet_ntop(AF_INET,message+8,ip2,sizeof(ip2));
            int cost=(message[13]<<8)|(message[12]);

                entry* allEntry=routerTable.getTable();
                for(i=0;i<routerTable.getLength();i++)
                {

                    if(
                          ((strcmp(allEntry[i].destination,ip2)==0)&&(strcmp(allEntry[i].next_hop,ip2)==0)&&(strcmp(ip2,myIPAddress)!=0))||
                          ((strcmp(allEntry[i].destination,ip1)==0)&&(strcmp(allEntry[i].next_hop,ip1)==0)&&(strcmp(ip1,myIPAddress)!=0))
                       )
                    {
                        allEntry[i].setCost(cost);

                    }
                }
                entry* savedEntry=savedCopy.getTable();
                for(i=0;i<savedCopy.getLength();i++)
                {

                    if(
                          ((strcmp(savedEntry[i].destination,ip2)==0)&&(strcmp(savedEntry[i].next_hop,ip2)==0)&&(strcmp(ip2,myIPAddress)!=0))||
                          ((strcmp(savedEntry[i].destination,ip1)==0)&&(strcmp(savedEntry[i].next_hop,ip1)==0)&&(strcmp(ip1,myIPAddress)!=0))
                       )
                    {
                        savedEntry[i].setCost(cost);


                    }
                }


        }
        else
        {
            table received_table(message);
            entry *mentry=routerTable.getTable();
            entry *rentry=received_table.getTable();
            int hereCost=0;
            char *hereNextHop;

            for(int i=0;i<routerTable.getLength();i++)
            {

                    if(strcmp(mentry[i].destination,nc.readFrom())==0)
                    {
                        hereNextHop=mentry[i].next_hop;
                        break;
                    }

            }

            entry* sTable=savedCopy.getTable();
            for(i=0;i<savedCopy.getLength();i++)
            {
                if(strcmp(sTable[i].destination,nc.readFrom())==0)
                {
                    hereCost=sTable[i].cost;
                }

            }
            //printf("cost:%d hop:%s\n",hereCost,hereNextHop);

            for(int j=0;j<received_table.getLength();j++)
            {
                for(int i=0;i<routerTable.getLength();i++)
                {

                    int s=(strcmp(mentry[i].destination,rentry[j].destination)==0);
                    int c=(mentry[i].cost>rentry[j].cost+hereCost);
                    int split=(strcmp(rentry[j].next_hop,myIPAddress)!=0);
                    int fUpdate=(strcmp(mentry[i].next_hop,nc.readFrom())==0);

                    if(s&&((c&&split)||fUpdate))
                    {
                        strcpy(mentry[i].next_hop,nc.readFrom());
                        if(rentry[j].cost+hereCost>=INFINITY)
                        {
                            mentry[i].setCost(INFINITY);
                        }
                        else
                        {
                            mentry[i].setCost(rentry[j].cost+hereCost);
                        }

                    }


                }
            }
            if(lastseen.getLastSeen(nc.readFrom())==link_down)
            {
                printf("+++++++++++%s link up++++++++++\n",nc.readFrom());
                int temp=savedCopy.getCost(nc.readFrom());
                if(routerTable.getCost(nc.readFrom())>temp)
                {
                    routerTable.setCost(nc.readFrom(),temp);
                    strcpy(routerTable.getEntry(nc.readFrom()).next_hop,nc.readFrom());
                }

            }
            lastseen.increaseSeen(nc.readFrom(),currentClock);
        }
}



int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("Provide ip and topo file\n");
        exit(1);
    }
    myIPAddress=argv[1];
    loader(argv[1],argv[2]);
    nc.openSocket(argv[1]);
    while(1)
    {
        char * receivedMeassage=nc.read();
        parser(receivedMeassage,nc.get_bytes_received());

    }

}

