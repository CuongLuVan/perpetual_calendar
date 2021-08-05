
#ifndef __msp430x22x2_h__  // Change this header file to include the MSP430 uC you are using.
	#include <msp430x22x2.h> // Header must only be included if none of the files linking to
	#define __msp430x22x2_h__ // this file include it already
#endif


// Note on I/face
// Connections from uC to LCD_module
// Please change the code to suit the H/W config
// you are using

/*	
		MSP430uc	LCD_Module
		P12			RS 
		P13			E 
		P14			D4
		P15			D5
		P16			D6
		P17			D7
		GND			GND
	
// Connections to LCD_module
   Your LCD_module in all probabilty powers it's onboard controller off a 5V supply.
   So U'll have to provide a 5V regulated supply seperately to the LCD_module
   Remember MSP430 runs at 3.6 V (max) ... U'll spare urself the hassle if you
   have a LCD_nodule which can work at lower voltages. Also this program only writes
   to the LCD, if you want to read from the LCD, U must use one more of the uC pins,
   in the current setup we have tied R/W signal of the LCD_module to GND, also 
   if your uC and LCD_module are running at different Vdd levels, it is advisable
   to use a level converter while reading from the LCD_module.
   
   R/W signal of the LCD_module is tied to GND

   You must connect a pot to vary the voltage at the contrast pin of the LCD_module

*/	
#define LCD_RS			BIT5
#define LCD_EN			BIT4

#define LCD_D4          BIT7
#define LCD_D5          BIT6
#define LCD_D6          BIT5
#define LCD_D7          BIT4


void delayLCD();
void ghilenh(unsigned char lenh);
void ghikt(char kt);
void lcd_puts(char *sttt);
void khoitao(void);
void lcd_clear(void);
 char taa;

//_________________________hàm tre LCD ____________________________________________________
void delayLCD()
{

    _delay_cycles(2000);
}										   

//_________________________Ham ghi lenh ra LCD:______________________________________________

void ghilenh(unsigned char lenh)
{   unsigned char tg,full;
	// ghi 4 bit cao
    P4DIR|=0xf0;
    // P2OUT=8;
    tg=lenh&0xf0;
    full=tg>>4;
    if((full&0x08)==0) P4OUT &=~LCD_D7; else P4OUT|=LCD_D7;
    if((full&0x04)==0) P4OUT &=~LCD_D6; else P4OUT|=LCD_D6;
    if((full&0x02)==0) P4OUT &=~LCD_D5; else P4OUT|=LCD_D5;
    if((full&0x01)==0) P4OUT &=~LCD_D4; else P4OUT|=LCD_D4;
    P3OUT &= ~LCD_RS;
	P3OUT |= LCD_EN;
	//delay_us(3);
	_delay_cycles(3);
	P3OUT &= ~LCD_EN;
      delayLCD();
	//ghi 4 bit thap
    full=lenh&0x0f;
    if((full&0x08)==0) P4OUT &=~LCD_D7; else P4OUT|=LCD_D7;
    if((full&0x04)==0) P4OUT &=~LCD_D6; else P4OUT|=LCD_D6;
    if((full&0x02)==0) P4OUT &=~LCD_D5; else P4OUT|=LCD_D5;
    if((full&0x01)==0) P4OUT &=~LCD_D4; else P4OUT|=LCD_D4;
     P3OUT |= LCD_EN;
    	//delay_us(3);
    	_delay_cycles(3);
     P3OUT &= ~LCD_EN;
          delayLCD();
	  
}												 
//__________________________ham ghi ki tu ra LCD:_____________________________________________

void ghikt(char lenh)
{   unsigned char tg,full;
  P4DIR|=0xf0;
   // P2OUT=8;
   tg=lenh&0xf0;
   full=tg>>4;
   if((full&0x08)==0) P4OUT &=~LCD_D7; else P4OUT|=LCD_D7;
   if((full&0x04)==0) P4OUT &=~LCD_D6; else P4OUT|=LCD_D6;
   if((full&0x02)==0) P4OUT &=~LCD_D5; else P4OUT|=LCD_D5;
   if((full&0x01)==0) P4OUT &=~LCD_D4; else P4OUT|=LCD_D4;
   P3OUT |= LCD_RS;
	P3OUT |= LCD_EN;
	//delay_us(3);
	_delay_cycles(3);
	P3OUT &= ~LCD_EN;
     delayLCD();
	//ghi 4 bit thap
   full=lenh&0x0f;
   if((full&0x08)==0) P4OUT &=~LCD_D7; else P4OUT|=LCD_D7;
   if((full&0x04)==0) P4OUT &=~LCD_D6; else P4OUT|=LCD_D6;
   if((full&0x02)==0) P4OUT &=~LCD_D5; else P4OUT|=LCD_D5;
   if((full&0x01)==0) P4OUT &=~LCD_D4; else P4OUT|=LCD_D4;
    P3OUT |= LCD_EN;
   	//delay_us(3);
   	_delay_cycles(3);
   	P3OUT &= ~LCD_EN;
         delayLCD();
}
//_________________________Ham hien thi 1 xau ra lcd:___________________________________________
void lcd_puts(char *sttt)
{
   taa=0;
   while(*sttt) {taa++;
   	if (taa==17) ghilenh(0xc0);
   	
    ghikt(*sttt);
    *sttt++;
    }
} 
//__________________________Ham khoi tao LCD:____________________________________________________
void khoitao(void)
{
	P4DIR|=0xf0;
	P3DIR|=0x30;
	taa=0;
    ghilenh(0x03);
    ghilenh(0x03);
    ghilenh(0x03);
    ghilenh(0x32);
    ghilenh(0x28);    //  4-bit, 2 hang, ma tran 5x7
    ghilenh(0x0C);    // bat hien thi, tat con tro
    ghilenh(0x06);    // dich con tro sang phai
    ghilenh(0x01);    // xoa man hinh
}
void lcd_clear(void){
ghilenh(0x01);
taa=0;}




