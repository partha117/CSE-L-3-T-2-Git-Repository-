#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LIST_INIT_SIZE 2
#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999

int listMaxSize;
int * list;
int length;
int shrink(void);

void initializeList()
{
	listMaxSize = LIST_INIT_SIZE;
	list = (int*)malloc(sizeof(int)*listMaxSize) ;
	length = 0 ;
}

int searchItem(int item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

int insertItem(int newitem)
{
	int * tempList ;
	if(list==NULL)
    {

        list = (int*)malloc(sizeof(int)*LIST_INIT_SIZE) ;

    }
	else if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        free(list) ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	}

	list[length] = newitem ; //store new item
	length++ ;
	return SUCCESS_VALUE ;
}


int deleteItemAt(int position) //version 2, do not preserve order of items
{
	if ( position >= length ) return NULL_VALUE;
	list[position] = list[length-1] ;
	length-- ;
	shrink();
	return SUCCESS_VALUE ;
}


int deleteItem(int item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return NULL_VALUE;
	deleteItemAt(position) ;
	return SUCCESS_VALUE ;
}

void printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    if(list==NULL)
    {

        printf("Current size: 0, current length: %d\n",length);
    }
    else
    {
        printf("Current size: %d, current length: %d\n", listMaxSize, length);
    }
}
int getLength(void)
{
    return length;
}
int insertItemAt(int pos, int item)
{
    if(pos>listMaxSize)
    {
        return NULL_VALUE;
    }
    else if(length+1==listMaxSize)
    {
        int*tempList;
        listMaxSize = 2 * listMaxSize ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ;
        }
        free(list) ;
        list = tempList ;

    }

    if(pos<length)
    {

        int i;
        for(i=length;i>=pos;i--)
        {
            list[i+1]=list[i];
        }
        list[pos]=item;
        length++;
        return SUCCESS_VALUE;

    }
    else if(pos==length)
    {
        list[pos]=item;
        length++;
        return SUCCESS_VALUE;
    }


}
int shrink(void)
{
    if(listMaxSize<=LIST_INIT_SIZE)
    {
        return NULL_VALUE;
    }
    for(;(length<=(listMaxSize/2))&&(listMaxSize>=LIST_INIT_SIZE);)//for loop used to be used in deleteAll
    {

        int *tempList;
        listMaxSize = listMaxSize/2 ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ;
        }
        free(list) ;
        list = tempList ;

    }
    return SUCCESS_VALUE;
}
int deleteLast(void)
{
    int c;
    if(length==0)
    {
        return NULL_VALUE;
    }
    c=list[length-1];

    length--;
    shrink();
    return c;
}
int clear()
{

    free(list);
    list=0;
    length=0;

    return SUCCESS_VALUE;

}
int deleteAll()
{
    length=0;
    shrink();
    listMaxSize = LIST_INIT_SIZE;

    return SUCCESS_VALUE;
}
int removeDuplicate(int item)
{
    int c=0,l;
    l=getLength();
    int i;
    for(i=0;i<length;i++)
    {
        if((list[i]==item)&&(c==0))
        {
            c=1;
        }
        else if((list[i]==item)&&(c>0))
        {
            int j;
            c++;
            for(j=i;j<length;j++)
            {
                list[j]=list[j+1];
            }
            deleteLast();

        }
    }
    shrink();
    if((c==0)||(c==1))
    {
        return NULL_VALUE;
    }
    return SUCCESS_VALUE;
}
int hasDuplicate(int item)
{
    int c=0,l;
    l=getLength();
    int i;
    for(i=0;i<length;i++)
    {
        if((list[i]==item)&&(c==0))
        {
            c=1;
        }
        else if((list[i]==item)&&(c>0))
        {
            return 2;

        }
    }
    return c;
}




