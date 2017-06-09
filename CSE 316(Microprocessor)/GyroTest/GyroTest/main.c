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
	DDRB=0xFF;
	PORTB=0x00;
	xPos=playerPositionX;
	yPos=playerPositionY;
	int  tempX,tempY=0;
	while(1)
	{
		    calcPosition();
			tempX=8-xPos;
			tempY=8-yPos;
			
			if(playerPositionX!=tempX)
			{
				printf("portb %d\n",PORTB);
				if(PORTB&(1<<PB0))
				{
					PORTB=PORTB&(~(1<<PB0));
					
				}
				else
				{
					PORTB=PORTB|(1<<PB0);
				}
				if(tempX>playerPositionX)
				{
					PORTB=PORTB&(~(1<<PB1));
				}
				else
				{
					PORTB=PORTB|(1<<PB0);
				}
			}
			if(playerPositionY!=tempY)
			{
				if(PORTB&(1<<PB2))
				{
					PORTB=PORTB&(~(1<<PB2));
				}
				else
				{
					PORTB=PORTB|(1<<PB2);
				}
				if(tempY>playerPositionY)
				{
					PORTB=PORTB&(~(1<<PB3));
				}
				else
				{
					PORTB=PORTB|(1<<PB3);
				}
				playerPositionX=tempX;
				playerPositionY=tempY;
			}
			
		printf("player X: %d Y: %d\n",playerPositionX,playerPositionY);
			_delay_ms(500);
			//drawMaze(maze);
			//drawPlayer(playerPositionX,playerPositionY);

		    
	}
	
}
unsigned char boundary(int val)
{
	if((val>=0)&&(val<16))
	{
		return 1;
	}
	return 0;
}
void calcPosition()
{
	mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
	xVal=(ax/2048);
	yVal=(ay/2048);
	//printf(" yval %d ay %d\n",yVal,ay);
	if((xVal!=xPos)||(yVal!=yPos))
	{
		if(xVal>xPos)
		{
			
			if(boundary(8-xPos-1))
			{
				xPos++;
			}
			//printf("Current x: %d xval %d\n",xPos,xVal);
		}
		else if(xVal<xPos)
		{
			
			if(boundary(8-xPos+1))
			{
				xPos--;
			}
			//printf("Current x: %d xval %d\n",xPos,xVal);
		}
		if(yVal>yPos)
		{
			if(boundary(8-yPos-1))
			{
				yPos++;
			}
			//printf("Current : %d\n",yPos);
		}
		else if(yVal<yPos)
		{
			if(boundary(8-yPos+1))
			{
				yPos--;
			}
			
			//printf("Current x: %d\n",yPos);
		}
		//printf("Current Position: X %d ::y %d\n",xPos,yPos);
	}
}

