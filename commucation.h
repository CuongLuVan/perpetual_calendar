#include <msp430.h>
#include "default_canleder.h"
extern char status;
//int8_t datasave[30];
//int8_t txnhan;
#define KEYUP   0x3e
#define KEYDOWN 0x3d
#define KEYNEWXT 0x3b
#define KEYUPOK 0x37
#define KEYUPKEYNENU 0x2f
#define KEYCANCAL 0x1f




#define DS1307_ADDRESS	0x68


/******************************************************************************\
*					Prototype (nguyen mau ham)    						       *
\******************************************************************************/

void checkall(void);
//note in commucation in funtion
void I2C_USCI_Init(unsigned char addr);	//init I2C
void I2C_USCI_Set_Address(unsigned char addr);	// change adress  Slave
unsigned char I2C_USCI_Read_Byte(unsigned char address);	//read 1 byte
//read mutie Byte
unsigned char I2C_USCI_Read_Word(unsigned char Addr_Data,unsigned char *Data, unsigned char Length);
//read and write 1 Byte
unsigned char I2C_USCI_Write_Byte(unsigned char address, unsigned char Data);
unsigned char I2C_USCI_Write_Word(unsigned char Addr_Data,unsigned char *Data, unsigned char Length);
void resetcasse(void);
void setupcase(void);
void init_I2C(void);
void read_ALL_Times(void);
void putdata_canlander(char input,char show);
void action(char input,char state);
void gettime(char time,char *number);
void show_Point_in_time_HMI(char line);
void show_All_Time_In_HMI(void);
void getShowTimeBLDC(char time,char *number);
void timeshow(char port);
void timeshows(char port);
char checktex(char hex);
void showhexa(char hex);
char readinput(void);
void getcanlerder(void);









