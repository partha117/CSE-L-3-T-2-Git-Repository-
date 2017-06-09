/*
 * Test_Program.c
 *
 * Created: 5/23/2017 11:44:41 PM
 *  Author: MUKTADIR
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

uint16_t maze[2][16] = {
	{
		
		0b1001111111111001,
		0b1000001000000001,
		0b1000001000000001,
		0b1000001000000001,
		0b1000001000000001,
		0b1000001000000001,
		0b0000001000000001,
		0b0000001001111111,
		0b0000001000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1111111111111111
	},
	{
		0b1001111111111001,
		0b1000000010000001,
		0b1000000010000001,
		0b1000000010000001,
		0b1000000010000001,
		0b1000000010000001,
		0b0000000010000001,
		0b0000000010000001,
		0b0000000010000001,
		0b1000000010000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1111111111111111
	}
};
uint16_t check[2][16]={
	
	
		
		{
	0b1111110001111111,
	0b0000000000000001,
	0b0000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1111111110000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000100000001,
	0b1000000100000001,
	0b1000000100000001,
	0b1000000100000001,
	0b0000000100000001,
	0b0000000100000001,
	0b1111111111111111,
	}
	,
	
	{
	0b1111110001111111,
	0b0000000000000001,
	0b0000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1111111111000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b0000000000000001,
	0b0000000000000001,
	0b1111111111111111
	}
};

uint16_t game_over[] ={
	0b1111111100000000,
	0b1000100100000000,
	0b1000100100000000,
	0b1000000100000000,
	0b0000000000000000,
	0b1111111100000000,
	0b0100000000000000,
	0b0010000000000000,
	0b0001000000000000,
	0b0000100000000000,
	0b1111111100000000,
	0b0000000000000000,
	0b1111111100000000,
	0b1000000100000000,
	0b0100001000000000,
	0b0011110000000000
};

uint16_t digit[10][8] = {
	{
		0b00000000,
		0b00111110,
		0b01111111,
		0b01001001,
		0b01010001,
		0b01111111,
		0b00111110,
		0b00000000,
	},
	{
		0b00000000,
		0b00000001,
		0b00010001,
		0b01111111,
		0b01111111,
		0b00000001,
		0b00000001,
		0b00000000
	},
	{
		0b00000000,
		0b00100011,
		0b01100111,
		0b01000101,
		0b01001001,
		0b01111001,
		0b00110001,
		0b00000000
	},
	{
		0b00000000,
		0b00100010,
		0b01100011,
		0b01001001,
		0b01001001,
		0b01111111,
		0b00110110,
		0b00000000
	},
	{
		0b00000000,
		0b00001100,
		0b00010100,
		0b00100100,
		0b01111111,
		0b01111111,
		0b00000100,
		0b00000000
	},
	{
		0b00000000,
		0b01110010,
		0b01110011,
		0b01010001,
		0b01010001,
		0b01011111,
		0b01001110,
		0b00000000
	},
	{
		0b00000000,
		0b00111110,
		0b01111111,
		0b01001001,
		0b01001001,
		0b01101111,
		0b00100110,
		0b00000000
	},
	{
		0b00000000,
		0b01100000,
		0b01100000,
		0b01000111,
		0b01011111,
		0b01111000,
		0b01100000,
		0b00000000
	},
	{
		0b00000000,
		0b00110110,
		0b01111111,
		0b01001001,
		0b01001001,
		0b01111111,
		0b00110110,
		0b00000000
	},
	{
		
		0b00000000,
		0b00110010,
		0b01111011,
		0b01001001,
		0b01001001,
		0b01111111,
		0b00111110,
		0b00000000
	}
};
typedef struct{
	unsigned char x, y;
	unsigned char len;
} obstacle; 
obstacle  obst[10];


#define RED		0x50
#define GREEN	0x60
#define BLUE	0x30

#define SET_ROW(X) PORTB = X; PORTD = X >> 8;
#define SET_COLUMN(C, COLOR) PORTA = C | COLOR;

//param: co-ordinate of top point
void drawVLine(uint16_t x, uint16_t y, unsigned char len, unsigned char color){
	uint16_t X = 1 << (15 - x);
	
	while(len--){
		SET_ROW(X);
		SET_COLUMN(y, color);
		_delay_ms(1);
		X >>= 1;
	}
	PORTA = 0xff;
	
}

//param: co-ordinate of top point
void drawHLine(uint16_t x, uint16_t y, unsigned char len, unsigned char color){
	uint16_t X = 1 << (15 - x);
	
	while(len--){
		SET_ROW(X);
		SET_COLUMN(y, color);
		_delay_ms(1);
		y++;
	}
	PORTA = 0xff;
}

void drawDigit(unsigned char dig, int col){
	unsigned char i;
	uint16_t x = 1 << 8;
	for (i = 0; i < 8; i ++){
		SET_ROW(digit[dig][i]);
		x >>= 1;
		SET_COLUMN((col == 0 ? i : (i + 8)), GREEN);
		_delay_ms(1);
		PORTA = 0xff;
	}
	
}

void draw(uint16_t mat[], unsigned char color){
	unsigned char i;
	
	for (i = 0; i < 16; i++){	
		SET_ROW(mat[i]);
		SET_COLUMN(i, color);
		_delay_ms(1);
		PORTA = 0xff; //turn off all the leds.
	}
	
}

//game settings
#define MAZE 0
#define GAME_OVER 10
unsigned char currentScreen = MAZE;//GAME_OVER;
unsigned char currentMaze = 1;
unsigned char dotR = 14;
unsigned char dotC = 13;
uint16_t score = 0;
unsigned char lives = 3;
unsigned char flag[4];
unsigned char firstEntranceToMaze = 1;///new:
int life=0;
int point=0;
void calPoint(int x,int y)
{
	if(currentMaze==1)
	{
		if(y>5&&x<8)
		{
			point+=5;
		}
		if(y>10&&x<8)
		{
			point+=5;
		}
		if(x>8)
		{
			point+=5;
		}
		if(y>4&&x>8)
		{
			point+=5;
		}
		if(y>10&&x>8)
		{
			point+=5;
		}
	}
	else if(currentMaze==0)
	{
		if(x>4)
		{
			point+=5;
		}
		if(y>8&&x>4)
		{
			point+=5;
		}
		if(y>10&&x>4)
		{
			point+=5;
		}
		point+=25;
		
	}
		
}
int checkLife(int x,int y)
{
	int i;
	if(currentMaze==1)
	{
	for(i=0;i<10;i++)
	{
		
		if((i!=2)&&(i!=9))
		{
			if((obst[i].y==y)&&((x>=obst[i].x)&&(x<obst[i].x+obst[i].len)))
			{
				
				return 0;
			}
		}
		else
		{
			if((obst[i].x==x)&&((y>=obst[i].y)&&(y<obst[i].y+obst[i].len)))
			{
				return 0;
			}
		}
		
	   
	}
	}
	else
	{
		for(i=0;i<2;i++)
		{
		
		if((i!=2))
		{
			if((obst[i].y==y)&&((x>=obst[i].x)&&(x<obst[i].x+obst[i].len)))
			{
				
				return 0;
			}
		}
		else
		{
			if((obst[i].x==x)&&((y>=obst[i].y)&&(y<obst[i].y+obst[i].len)))
			{
				return 0;
			}
		}
		
	   
		}	
	}
	return 1;
}
int checkCollision(int x,int y)
{
	uint16_t temp=check[currentMaze][x];
	//printf("temp %d\n",temp);
	if((x<16)&&(y<16)&&(x>=0)&&(y>=0))
	{
		temp=(temp&(1<<(15-y)));
		if(temp==0)
		{
			return 1;
		}
	}
	//printf("from maze %d\n",maze[x]);
	return 0;
}
int main(void)
{
	uint16_t c = 0;
	unsigned char pinc = 0, t;
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0x00;
	DDRD = 0xFF;
	MCUCSR= 1 << JTD;MCUCSR= 1 << JTD;
	int loop=0;
	c=0;
	unsigned char firstEntranceToMaze0=1;
	
	
    while(1){
		 c++;
		 if(lives==0)
		 {
			 currentScreen=GAME_OVER;
		 }
		 if(((dotR==14)||(dotR==13))&&(dotC==0)&&(currentMaze==1))
		{
			currentMaze=0;
			dotR=1;
			dotC=0;
		}
		if(((dotR==14)||(dotR==13))&&(dotC==0)&&(currentMaze==0))
		{
			currentScreen=GAME_OVER;
		}
		if(currentScreen == MAZE){
			if(loop%20==0)
			{
			
			//checks whether position changed.
			t = PINC;
			if(( t & (1 << PC0) )&&(!flag[0])){
				
				if(checkCollision(dotR+1,dotC))
				{
					dotR++;
					if(!checkLife(dotR,dotC))
					{
						lives--;
					}
					
				}
				
				
				//_delay_ms(150);
				flag[0]=1;
				loop=0;
			}
			if(!( t & ((1 << PC0)) )){
				
				flag[0]=0;
			}
			
			if(( t & (1 << PC1) )&&(!flag[1]))
			{
			
				if(checkCollision(dotR-1,dotC))
				{
					dotR--;
					if(!checkLife(dotR,dotC))
					{
						lives--;
					}
				}
				flag[1]=1;
				//_delay_ms(150);
				loop=0;
			}
			if(!( t & ((1 << PC1)) )){
				
				flag[1]=0;
			}
			if(( t & (1 << PC6) )&&(!flag[2])){
			
				if(checkCollision(dotR,dotC+1))
				{
					dotC++;
					if(!checkLife(dotR,dotC))
					{
						lives--;
					}
				}
				flag[2]=1;
				//_delay_ms(150);
				loop=0;
			}
			if(!( t & ((1 << PC6)) )){
				
				flag[2]=0;
			}
			if(( t & (1 << PC7) )&&(!flag[3])){
			
				if(checkCollision(dotR,dotC-1))
				{
					dotC--;if(!checkLife(dotR,dotC))
					{
						lives--;
					}
				}
				flag[3]=1;
				//_delay_ms(150);
				loop=0;
			}
			if(!( t & ((1 << PC7)) )){
				
				flag[3]=0;
			}
			
			}
			loop++;
			pinc = t;
			
			
			draw(maze[currentMaze], RED);	
			
			drawVLine(dotR, dotC, 1, GREEN); // the dot
			drawHLine(0, 6, lives, GREEN);
			drawHLine(0, 6, lives, RED);
			
			//if(life%10==0)
			//{
				if((c%100==0)&&!checkLife(dotR,dotC))
				{
					lives--;
					life=0;
				}
			//}
			//life++;
			
			 
			//draw the obstacles
			if (currentMaze == 1){
				
				if(firstEntranceToMaze){
					
					firstEntranceToMaze = 0;
					obst[0].len =  obst[1].len = obst[3].len =  obst[4].len =  4;
					obst[5].len =  obst[6].len = obst[7].len =  obst[8].len=obst[9].len= 1;
					obst[2].len = 2;
					obst[5].y=obst[0].y = 5;
					obst[6].y=obst[1].y = 10;
					obst[7].x=obst[2].x = 8;
					obst[8].y=obst[3].y = 10;
					obst[9].y=obst[4].y = 4;	
				}
					
				switch(c / 100){
					case 0:
						 obst[0].x = 1;
						 obst[1].x = 3;
						 obst[2].y = 10;
						 obst[3].x = 10;
						 obst[4].x = 11;
						 
						
						break;
					case 1:
					
						 obst[0].x = 2;
						 obst[1].x = 2;
						 obst[2].y = 11;
						 obst[3].x = 11;
						 obst[4].x = 10;
						break;
					case 2:
						 obst[0].x = 3;
						 obst[1].x = 1;
						 obst[2].y = 12;
						 obst[3].x = 10;
						 obst[4].x = 9;
						break;
					case 3:
						 obst[0].x = 4;
						 obst[1].x = 2;
						 obst[2].y = 13;
						 obst[3].x = 9;
						 obst[4].x = 10;
						break;
					case 4:
						 obst[0].x = 3;
						 obst[1].x = 3;
						 obst[2].y = 12;
						 obst[3].x = 10;
						 obst[4].x = 11;
						break;
					case 5:
						 obst[0].x = 2;
						 obst[1].x = 4;
						 obst[2].y = 11;
						 obst[3].x = 11;
						 obst[4].x = 10;
						break;
					
					default: c = 0;
				}
				if(c%100==0)
				{
					if(obst[5].y==1)
					{
						obst[5].y=obst[0].y-1;
						obst[5].x=obst[0].x+2;
					}
					else{
						obst[5].y--;
					}
					if(obst[6].y==15)
					{
						obst[6].y=obst[1].y+1;
						obst[6].x=obst[1].x+2;
					}
					else{
						obst[6].y++;
					}
					if(obst[7].x==1)
					{
						obst[7].x=obst[2].x-1;
						obst[7].y=obst[2].y+2;
					}
					else{
						obst[7].x--;
					}
					if(obst[8].y==15)
					{
						obst[8].y=obst[3].y+1;
						obst[8].x=obst[3].x+2;
					}
					else{
						obst[8].y++;
					}
					if(obst[9].y==1)
					{
						obst[9].y=obst[4].y-1;
						obst[9].x=obst[4].x+2;
					}
					else{
						obst[9].y--;
					}
					
				}
				drawVLine( obst[5].x,  obst[5].y,  obst[5].len,  RED);
				drawVLine( obst[6].x,  obst[6].y,  obst[5].len,  RED);
				drawVLine( obst[7].x,  obst[7].y,  obst[5].len,  RED);
				drawVLine( obst[8].x,  obst[8].y,  obst[5].len,  RED);
				drawVLine( obst[9].x,  obst[9].y,  obst[5].len,  RED);
				drawVLine( obst[0].x,  obst[0].y,  obst[0].len,  RED);
				drawVLine( obst[1].x,  obst[1].y,  obst[1].len,  RED);
				drawHLine( obst[2].x,  obst[2].y,  obst[2].len,  RED);
				drawVLine( obst[3].x,  obst[3].y,  obst[3].len,  RED);
				drawVLine( obst[4].x,  obst[4].y,  obst[4].len,  RED);
			}
			else if(currentMaze==0)
			{
					if(firstEntranceToMaze0){
					
					firstEntranceToMaze0 = 0;
					obst[0].len =  obst[1].len = 4;
					obst[2].len =  5;
					obst[2].x=6;
					obst[0].y=5;
					obst[1].y=10;					
				}
				switch(c / 100){
					case 0:
						 
						 obst[0].x = 10;
						 obst[1].x = 11;
						 obst[2].y = 7;
						 
						
						break;
					case 1:
					
						 
						 obst[0].x = 11;
						 obst[1].x = 10;
						 obst[2].y = 8;
						break;
					case 2:
						 
						 obst[0].x = 10;
						 obst[1].x = 9;
						 obst[2].y = 9;
						break;
					case 3:
						 
						 obst[0].x = 9;
						 obst[1].x = 8;
						 obst[2].y = 10;
						break;
					case 4:

						 obst[0].x = 8;
						 obst[1].x = 9;
						 obst[2].y = 11;
						break;
					case 5:
						 
						 obst[0].x = 7;
						 obst[1].x = 11;
						 obst[2].y = 10;
					case 6:
						 
						 obst[0].x = 8;
						 obst[1].x = 10;
						 obst[2].y = 9;

						break;
					
					default: c = 0;
				}
				drawVLine( obst[0].x,  obst[0].y,  obst[0].len,  RED);
				drawVLine( obst[1].x,  obst[1].y,  obst[1].len,  RED);
				drawHLine( obst[2].x,  obst[2].y,  obst[2].len,  RED);
			}
			
		}
		else if(currentScreen == GAME_OVER){
			switch(c / 200){
				case 0: 
					draw(game_over, RED);
					break;
				case 1:
					draw(game_over, GREEN);
					break;
				
				default:
					c = 0;
			}
			point = 0;
			calPoint(dotR,dotC);
			
			drawDigit((point / 10), 0);
			//drawDigit(8, 0);
			//drawDigit(9, 1);
			drawDigit(point % 10, 1);
		}
    }
}
