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
       // cout<<"ip1 :"<<a<<"   ip2 :"<<b<<"   cost :"<<c<<endl;




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




    routerTable.printTable();

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
          //  printf("It is %s send table to: %s\n",myIPAddress,allEntry[i].destination);
            nc.write(allEntry[i].destination,table_as_string);

        }
    }
   // printf("out\n");
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
                printf("-----------------Ip Table of %s-----------------\n",myIPAddress);
                routerTable.printTable();
                printf("-----------------++++++++++++++------------------------\n");
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
                        nc.write(all[i].next_hop,message);
                        printf("Packet Forwraded To %s (printed by %s)\n",all[i].next_hop,myIPAddress);
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
                    routerTable.setCost(all[k].ip,INFINITY);
                    all[k].lastSeen=link_down;
                   // printf("<<<<<<<<<<<<<<<<<<<<<<From Link Down\n");
                   // routerTable.printTable();

                   entry* check=routerTable.getTable();
                   entry *sc=savedCopy.getTable();
                   for(i=0;i<routerTable.getLength();i++)
                   {
                        if(strcmp(check[i].next_hop,all[k].ip)==0)
                        {

                            check[i].setCost(INFINITY);

                            for(j=0;j<savedCopy.getLength();j++)
                            {
                                if((strcmp(check[i].destination,sc[j].destination)==0)&&(lastseen.getLastSeen(sc[j].next_hop)!=link_down))
                                {
                                    strcpy(check[i].next_hop,sc[j].next_hop);
                                    check[i].setCost(sc[j].cost);
                                }

                            }
                            routerTable.printTable();
                        }
                   }

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

           // printf("Before Updating r\n");
           // routerTable.printTable();
           // printf("Before Updating s\n");
           // savedCopy.printTable();




                entry* allEntry=routerTable.getTable();
               // printf("Before %s\n",routerTable.getasString());
                for(i=0;i<routerTable.getLength();i++)
                {

                    if(
                          ((strcmp(allEntry[i].destination,ip2)==0)&&(strcmp(allEntry[i].next_hop,ip2)==0)&&(strcmp(ip2,myIPAddress)!=0))||
                          ((strcmp(allEntry[i].destination,ip1)==0)&&(strcmp(allEntry[i].next_hop,ip1)==0)&&(strcmp(ip1,myIPAddress)!=0))
                       )
                    {
                        allEntry[i].setCost(cost);
                       // printf("After Updating r\n");
                       // routerTable.printTable();
                       // printf("After %s\n",routerTable.getasString());

                    }
                }
                entry* savedEntry=savedCopy.getTable();
               // printf("Before %s\n",routerTable.getasString());
                for(i=0;i<savedCopy.getLength();i++)
                {

                    if(
                          ((strcmp(savedEntry[i].destination,ip2)==0)&&(strcmp(savedEntry[i].next_hop,ip2)==0)&&(strcmp(ip2,myIPAddress)!=0))||
                          ((strcmp(savedEntry[i].destination,ip1)==0)&&(strcmp(savedEntry[i].next_hop,ip1)==0)&&(strcmp(ip1,myIPAddress)!=0))
                       )
                    {
                        savedEntry[i].setCost(cost);
                       // printf("After Updating 2\n");
                        //savedCopy.printTable();
                       // printf("After %s\n",routerTable.getasString());

                    }
                }


        }
        else
        {
            table received_table(message);
           // printf("it is %s received Table from :%s\n",myIPAddress,nc.readFrom());
            entry *mentry=routerTable.getTable();
            entry *rentry=received_table.getTable();
            int hereCost=0;
            char *hereNextHop;

            for(int i=0;i<routerTable.getLength();i++)
            {

                    if(strcmp(mentry[i].destination,nc.readFrom())==0)
                    {
                        hereNextHop=mentry[i].next_hop;
                        hereCost=mentry[i].cost;
                        break;
                    }

            }
          //  printf("cost:%d hop:%s\n",hereCost,hereNextHop);
//            for(int i=0;i<received_table.getLength();i++)
//            {
//                if(!routerTable.isAvailable(rentry[i].destination))
//                {
//                    routerTable.insertEntry(rentry[i].destination,hereNextHop,hereCost+rentry[i].cost);
//                }
//            }

            for(int j=0;j<received_table.getLength();j++)
            {
                for(int i=0;i<routerTable.getLength();i++)
                {

                    int s=(strcmp(mentry[i].destination,rentry[j].destination)==0);
                    int c=(mentry[i].cost>rentry[j].cost+hereCost);
                    int split=(strcmp(rentry[j].next_hop,myIPAddress)!=0);
                    int fUpdate=(strcmp(mentry[i].next_hop,nc.readFrom())==0);
                    if(s)
                    {
                       // printf("s:%d c:%d split:%d fUpdate:%d mentry :%s rentry:%s\n",s,c,split,fUpdate,mentry[i].destination,rentry[j].destination);
                    }




                   // if((strcmp(mentry[i].destination,rentry[j].destination)==0)&&(mentry[i].cost>rentry[j].cost+hereCost)&&(strcmp(rentry[j].next_hop,myIPAddress)!=0))
                    if(s&&((c&&split)||fUpdate))
                    {

                       // printf("++++++++++ip %s\n",myIPAddress);
                       // routerTable.printTable();
                       // printf("here cost %d\n",hereCost);
                      //  printf("++++++++++ip %s\n",nc.readFrom());
                       // received_table.printTable();
                        strcpy(mentry[i].next_hop,hereNextHop);
                       // printf("Next hop:%s \n",mentry[i].next_hop);
                        if(rentry[j].cost==INFINITY)
                        {
                            mentry[i].setCost(rentry[j].cost);
                        }
                        else
                        {
                            mentry[i].setCost(rentry[j].cost+hereCost);
                        }


                    }


                }
            }
          //  printf("------------------------Start Now Router Table---------------------\n");
         //   routerTable.printTable();
         //   printf("------------------------End  Now Router Table---------------------\n");
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

