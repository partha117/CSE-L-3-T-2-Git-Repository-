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
       // cout<<"ip1 :"<<a<<"   ip2 :"<<b<<"   cost :"<<c<<endl;

        if(strcmp(a,ipAddress)==0)
        {
            routerTable.insertEntry(b,b,c);
            savedCopy.insertEntry(b,b,c);
            lastseen.insertEntry(b,link_down);
        }
        else if(strcmp(b,ipAddress)==0)
        {
            routerTable.insertEntry(a,a,c);
            savedCopy.insertEntry(a,a,c);
            lastseen.insertEntry(a,link_down);
        }


    }
    topo.close();
//    topo.open(fileName);
//    while(topo>>a>>b>>c)
//    {
//       // cout<<"ip1 :"<<a<<"   ip2 :"<<b<<"   cost :"<<c<<endl;
//
//
//
//
//            if(!routerTable.isAvailable(a))
//            {
//                routerTable.insertEntry(a,UNDEFINED,INFINITY);
//                savedCopy.insertEntry(a,UNDEFINED,INFINITY);
//                lastseen.insertEntry(a,link_down);
//
//            }
//            if(!routerTable.isAvailable(b))
//            {
//                routerTable.insertEntry(b,UNDEFINED,INFINITY);
//                savedCopy.insertEntry(b,UNDEFINED,INFINITY);
//                lastseen.insertEntry(b,link_down);
//
//            }
//    }




    routerTable.printTable();

    topo.close();
    return ;


}


void sendTable()
{
    entry * allEntry=savedCopy.getTable();
    char *table_as_string=routerTable.getasString();
    for(int i=0;i<routerTable.getLength();i++)
    {
        if((strcmp(allEntry[i].destination,allEntry[i].next_hop)==0)&&(strcmp(allEntry[i].destination,myIPAddress)!=0))
        {
            printf("It is %s send table to: %s\n",myIPAddress,allEntry[i].destination);
            nc.write(allEntry[i].destination,table_as_string);

        }
    }
    //printf("out\n");
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
   // char command[4];
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
                routerTable.printTable();
            }

        }
        else if(strcmp(temp,"send")==0)
        {
            commandType=2;
            //Source ip parsing starts
            for(i=4,j=0;i<8;i++,j++)
            {
                temp[j]=message[i];
            }
            temp[j]=0;

            inet_ntop(AF_INET,temp,ip1,sizeof(ip1));
           // printf("ip1 %s\n",ip1);
        //Source ip parsing end
            //destination ip starts
            for(i=8,j=0;i<12;i++,j++)
            {
                temp[j]=message[i];
            }
            temp[j]=0;
            inet_ntop(AF_INET,temp,ip2,sizeof(ip2));
           // printf("ip2 %s\n",ip2);
            //destination ip parsing end

            //Length parsing starts

            length=(message[13]<<8)|(message[12]);

           // printf("length %s\n",length);
            //Length parsing end

            //message parsing starts
            for(i=14,j=0;i<bytes;i++,j++)
            {
                messageToPass[j]=message[i];
            }
            messageToPass[j]=0;



            //printf("message %s\n",message);
        }
        else if((temp[0]=='c')&&(temp[1]=='l')&&(temp[2]='k'))
        {
            commandType=3;
            currentClock++;
            printf("Now Clock: %s\n",message);
            seenEntry * all=lastseen.getAll();
            for(int k=0;k<lastseen.getLength();k++)
            {
                if((all[k].lastSeen<=currentClock-down)&&(all[k].lastSeen!=link_down))
                {
                    printf("%s link down\n",all[k].ip);
                    routerTable.setCost(all[k].ip,INFINITY);
                    printf("<<<<<<<<<<<<<<<<<<<<<<From Link Down\n");
                    routerTable.printTable();
                    all[k].lastSeen=link_down;
                }
            }
            sendTable();

        }
        else if((temp[0]=='c')&&(temp[1]=='o')&&(temp[2]='s'))
        {
            commandType=4;
            //Source ip parsing starts
            for(i=4,j=0;i<8;i++,j++)
            {
                temp[j]=message[i];
            }
            temp[j]=0;

            inet_ntop(AF_INET,temp,ip1,sizeof(ip1));
           // printf("ip1 %s\n",ip1);
        //Source ip parsing end
            //destination ip starts
            for(i=8,j=0;i<12;i++,j++)
            {
                temp[j]=message[i];
            }
            temp[j]=0;
            inet_ntop(AF_INET,temp,ip2,sizeof(ip2));
           // printf("ip2 %s\n",ip2);
            //destination ip parsing end

            //cost parsing starts

            int cost=(message[13]<<8)|(message[12]);


            if(strcmp(ip1,myIPAddress)==0)
            {
                entry* allEntry=routerTable.getTable();
               // printf("Before %s\n",routerTable.getasString());
                for(i=0;i<routerTable.getLength();i++)
                {
                    if(strcmp(allEntry[i].destination,ip2)==0)
                    {
                        allEntry[i].setCost(cost);
                       // printf("After %s\n",routerTable.getasString());
                        return;
                    }
                }
            }

        }
        else
        {
            table received_table(message);
            printf("it is %s received Table from :%s\n",myIPAddress,nc.readFrom());
            entry *mentry=routerTable.getTable();
            entry *rentry=received_table.getTable();
            int hereCost=0;
            char *hereNextHop;
            for(int i=0;i<routerTable.getLength();i++)
            {

                    if(strcmp(mentry[i].destination,nc.readFrom())==0)
                    {
                        hereCost=mentry[i].cost;
                        hereNextHop=mentry[i].next_hop;
                    }

            }

            for(int j=0;j<received_table.getLength();j++)
            {
                for(int i=0;i<routerTable.getLength();i++)
                {
                    if((strcmp(mentry[i].destination,rentry[j].destination)==0)&&(mentry[i].cost>rentry[j].cost+hereCost)&&(strcmp(rentry[j].next_hop,myIPAddress)!=0))
                    {

                        printf("++++++++++ip %s\n",myIPAddress);
                        routerTable.printTable();
                        printf("here cost %d\n",hereCost);
                        printf("++++++++++ip %s\n",nc.readFrom());
                        received_table.printTable();
                        strcpy(mentry[i].next_hop,hereNextHop);
                        printf("Next hop:%s \n",mentry[i].next_hop);
                        mentry[i].cost=rentry[j].cost+hereCost;
                    }

                }
            }
            printf("------------------------Start Now Router Table---------------------\n");
            routerTable.printTable();
            printf("------------------------End  Now Router Table---------------------\n");
            if(lastseen.getLastSeen(nc.readFrom())==link_down)
            {
                printf("%s link up\n",nc.readFrom());
                int temp=savedCopy.getCost(nc.readFrom());
                if(routerTable.getCost(nc.readFrom())>temp)
                {
                    routerTable.setCost(nc.readFrom(),temp);
                    strcpy(routerTable.getEntry(nc.readFrom()).next_hop,nc.readFrom());
                }

            }
            lastseen.increaseSeen(nc.readFrom(),currentClock);
            //printf("now last seen: %d",lastseen.getLastSeen(nc.readFrom()));
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

//    myIPAddress="192.168.10.1";
//    loader(myIPAddress,"topo.txt");
//    nc.openSocket(myIPAddress);


    while(1)
    {
        char * receivedMeassage=nc.read();
        parser(receivedMeassage,nc.get_bytes_received());
       //printf("%s\n",receivedMeassage);
    }



}

