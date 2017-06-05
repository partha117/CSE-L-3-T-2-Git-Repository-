/*
 * GyroTest.c
 *
 * Created: 16-May-17 10:08:05 PM
 * Author : ASUS
 */ 
 #define MPU6050_GETATTITUDE 1
#include <avr/io.h>
#include "mpu6050.h"
#include <stdio.h>
#include <string.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include "uart.c"

int16_t ax = 0;
int16_t ay = 0;
int16_t az = 0;
int16_t gx = 0;
int16_t gy = 0;
int16_t gz = 0;
int xPos=0;
int yPos=0;
int xVal=0;
int yVal=0;
unsigned int maze[16]=
{   0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111,
0b1111111111111111
};

int playerPositionX=0;
int playerPositionY=0;
void calcPosition(void);
unsigned char checkCollision(int x,int y);
int main(void)
{
    /* Replace with your application code */
    mpu6050_init();
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	xPos=playerPositionX;
	yPos=playerPositionY;
	int  tempX,tempY=0;
	while(1)
	{
		    calcPosition();
			tempX=8-xPos;
			tempY=8-yPos;
			//printf("from here %d\n",yPos);
			//if(checkCollision(tempX,tempY))
			//{
				//printf("from inside here %d\n",yPos);
				playerPositionX=tempX;
				playerPositionY=tempY;
			//}
		printf("player X: %d Y: %d\n",playerPositionX,playerPositionY);
			_delay_ms(500);
			//drawMaze(maze);
			//drawPlayer(playerPositionX,playerPositionY);

		    
	}
	
}
unsigned char checkCollision(int x,int y)
{
	unsigned char temp;
	//printf("From chack collision x:%d y:%d\n",x,y);
	if((x<16)&&(y<16))
	{
		
		temp=(maze[x]&&(1<<(y)));
		//printf("iside1 maze %d temp %d\n",maze[x],temp);
		if(temp)
		{
			//printf("iside2\n");
			return 1;
		}
	}
	//printf("from maze %d\n",maze[x]);
	return 0;
}
void calcPosition()
{
	mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
	xVal=(ax/2048);
	yVal=(ay/2048);
	//printf(" yval %d ay %d\n",yVal,ay);
	int tempX,tempY;
	tempY=tempX=0;
	if((xVal!=xPos)||(yVal!=yPos))
	{
		if(xVal>xPos)
		{
			xPos++;
			tempX=8-xPos;
			tempY=8-yPos;
			if(!(checkCollision(tempX,tempY)))
			{
				xPos--;
			}
			//printf("Current x: %d xval %d\n",xPos,xVal);
		}
		else if(xVal<xPos)
		{
			xPos--;
			tempX=8-xPos;
			tempY=8-yPos;
			if(!(checkCollision(tempX,tempY)))
			{
				xPos++;
			}
			//printf("Current x: %d xval %d\n",xPos,xVal);
		}
		if(yVal>yPos)
		{
			yPos++;
			tempX=8-xPos;
			tempY=8-yPos;
			if(!(checkCollision(tempX,tempY)))
			{
				yPos--;
			}
			//printf("Current : %d\n",yPos);
		}
		else if(yVal<yPos)
		{
			yPos--;
			tempX=8-xPos;
			tempY=8-yPos;
			if(!(checkCollision(tempX,tempY)))
			{
				yPos++;
			}
			//printf("Current x: %d\n",yPos);
		}
		//printf("Current Position: X %d ::y %d\n",xPos,yPos);
	}
}

