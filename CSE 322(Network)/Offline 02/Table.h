#include<string.h>
#include<stdlib.h>
#define max_size 15
#define NOT_SEEN -1
#define link_down -10
#define ip_length 15
using namespace std;
class entry
{
    public:
        char* destination;
        char* next_hop;
        int cost;
        entry()
        {
        };
        entry(char* a,char* b,int c)
        {
            destination=(char*)malloc((sizeof(char))*ip_length);
            next_hop=(char*)malloc((sizeof(char))*ip_length);
            strcpy(destination,a);
            strcpy(next_hop,b);
            cost=c;
        }
        void setCost(int c)
        {
            cost=c;
        }
        ~entry()
        {
        };
};
class table
{
    entry tableEntry[max_size];
    int length;
    char str[15];
    char*converter(int a)
    {

        char*str=(char*)malloc((sizeof(char))*ip_length);
        sprintf(str, "%d", a);
        //printf("in converter %s\n",str);
        return str;
    }

public:

    table()
    {
        length=0;

    }
    table(char *all)
    {

//        for(int i=0;i<length;i++)
//        {
//            free(tableEntry[i].destination);
//            free(tableEntry[i].next_hop);
//        }
        length=0;
        char *a,*b,*c;
        char* token = strtok(all, " ");
        int i=1;
        while (token)
        {
            if(i==1)
            {
                a=token;
                i++;
            }
            else if(i==2)
            {
                b=token;
                i++;
            }
            else if(i==3)
            {
                c=token;
                //printf("%s %s %s\n",a,b,c);
                insertEntry(a,b,atoi(c));
                i=1;
                //printf("now length: %d\n",length);
            }

            //insertEntry(a,b,atoi(c));
            token = strtok(NULL, " ");
           // //printf("now token %s",token);
        }
    }
    bool isAvailable(char *destination)
    {
        for(int i=0;i<length;i++)
        {
            if(strcmp(tableEntry[i].destination,destination)==0)
            {
                return true;
            }
        }
        return false;
    }
    int getCost(char *destination)
    {
        for(int i=0;i<length;i++)
        {
            if(strcmp(tableEntry[i].destination,destination)==0)
            {
                return tableEntry[i].cost;
            }
        }
        return INFINITY;

    }
    void setCost(char *destination,int df)
    {
        for(int i=0;i<length;i++)
        {
            if(strcmp(tableEntry[i].destination,destination)==0)
            {
                tableEntry[i].setCost(df);
            }
        }
    }
    void setInfinity(char *destination)
    {
        for(int i=0;i<length;i++)
        {
            if(strcmp(tableEntry[i].next_hop,destination)==0)
            {
                tableEntry[i].setCost(INFINITY);
                strcpy(tableEntry[i].next_hop,UNDEFINED);
               // printf("It is %s",tableEntry[i].next_hop);
            }
        }
    }
    entry getEntry(char *destination)
    {
        entry Entry;
        for(int i=0;i<length;i++)
        {
            if(strcmp(tableEntry[i].destination,destination)==0)
            {
                Entry= tableEntry[i];
            }
        }
        return Entry;

    }
    void setEntry(int i,char* a,char* b,int c)
    {

        tableEntry[i].destination=(char*)malloc((sizeof(char))*ip_length);
        tableEntry[i].next_hop=(char*)malloc((sizeof(char))*ip_length);
        strcpy(tableEntry[i].destination,a);
        strcpy(tableEntry[i].next_hop,b);
        tableEntry[i].cost=c;
    }
    void insertEntry(char* a,char* b,int c)
    {
        //////printf("in insert %d %d %d\n",strlen(a),strlen(b),c);
        ////printf("length %d\n",length);
        tableEntry[length].destination=(char*)malloc((sizeof(char))*strlen(a));
        tableEntry[length].next_hop=(char*)malloc((sizeof(char))*strlen(b));
        strcpy(tableEntry[length].destination,a);
       // printf("%s %s\n",a,tableEntry[length].destination);
        strcpy(tableEntry[length].next_hop,b);
        //printf("%s %s\n",b,tableEntry[length].next_hop);
        tableEntry[length].cost=c;
        //printf("%d %d\n",c,tableEntry[length].cost);
        length++;
        //printf("%d ln\n",length);
    }
    ~table()
    {
//        for(int i=0;i<length;i++)
//        {
//            free(tableEntry[i].destination);
//            free(tableEntry[i].next_hop);
//        }
        //printf("Now destorying\n");
    }
    int getLength()
    {
        return length;
    }
    entry* getTable()
    {
        return tableEntry;
    }
    entry getEntry(int i)
    {
        return tableEntry[i];
    }
    char * getasString()
    {
        char* temp=(char*)malloc((sizeof(char))*1024);
        int l,cp=0;
        temp[0]=0;
        ////printf("here1025 %d \n",length);
        for(int i=0;i<length;i++)
        {
            ////printf("entering\n");
            strcat(temp,tableEntry[i].destination);
            ////printf("destination %s  %s\n",temp,tableEntry[i].destination);
            l=strlen(temp);
            temp[l]=' ';
            temp[l+1]='\0';
            ////printf("space %s ended\n",temp);
            strcat(temp,tableEntry[i].next_hop);
            ////printf("next hop %s %s\n",temp,tableEntry[i].next_hop);
            l=strlen(temp);
            temp[l]=' ';
            temp[l+1]='\0';
            char *temp1=converter(tableEntry[i].cost);
            ////printf("cost checking %s",temp1);

            strcat(temp,temp1);
            free(temp1);
            ////printf("cost after %s %d\n",temp,tableEntry[i].cost);
            l=strlen(temp);
            temp[l]=' ';
            temp[l+1]='\0';
            ////printf("here\n");

        }
        return temp;
    }
    void printTable()
    {

        for(int i=0;i<length;i++)
        {
            printf("%s   %s   %d\n",tableEntry[i].destination,tableEntry[i].next_hop,tableEntry[i].cost);
        }
    }

};
class seenEntry
{

    public:
    char * ip;
    int lastSeen;
    seenEntry()
    {
        ip=NULL;
        lastSeen=0;
    }
    seenEntry(char *st)
    {
        ip=(char*)malloc((sizeof(char))*strlen(st));
        strcpy(ip,st);
        lastSeen=0;
    }
    void setEntry(char *st,int k)
    {
        ip=(char*)malloc((sizeof(char))*strlen(st));
        strcpy(ip,st);
        lastSeen=k;

    }
    void setSeen(int k)
    {
        lastSeen=k;
    }
};
class SeenTable
{
    seenEntry all[max_size];
    int length;

    public:
    SeenTable()
    {
        length=0;

    }
    int insertEntry(char *st,int l)
    {
        all[length].setEntry(st,l);
        length++;
    }
    void increaseSeen(char *st,int l)
    {
        for(int i=0;i<length;i++)
        {
            if(strcmp(st,all[i].ip)==0)
            {
            //printf("updating %s\n",all[i].ip);
                all[i].setSeen(l);
                return ;
            }
        }
    }
    int getLastSeen(char *st)
    {
        for(int i=0;i<length;i++)
        {
            if(strcmp(st,all[i].ip)==0)
            {

                return all[i].lastSeen;
            }
        }
    }
    int getLength()
    {
        return length;
    }
    seenEntry* getAll()
    {
        return all;
    }
};

