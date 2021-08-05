#include "commucation.h"
#include "lcd.h"
unsigned char Temp[9];
// trang thai
char status=0;
// thanh ghi thoi gian
// datasave[0] gio
// datasave[1] phut
// datasave[2] ngay
// datasave[3] thang
// datasave[4] nam
// datasave[5] ngay am
// datasave[6] tang am
int datasave[10];
char mdate_in_month[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//int8_t txnhan;

void checkall(void){
// check hour
if(datasave[0]>23) datasave[0]=0; else if(datasave[0]<0) datasave[0]=23;
if(datasave[1]>59) datasave[1]=0; else if(datasave[1]<0) datasave[1]=59;

// check moth and day
if(datasave[3]>12) datasave[3]=1; else if(datasave[3]<1) datasave[3]=12;
if(datasave[2]<1) datasave[2]=1;
if(datasave[4]<0) datasave[4]=0;
if(datasave[3]==2){
	if((datasave[4]%4)==0) {
		if(datasave[2]>29) datasave[2]=1; else if(datasave[2]<1) datasave[2]=29;
	} else {
		if(datasave[2]>28) datasave[2]=1; else if(datasave[2]<1) datasave[2]=28;
	}
}

}



void I2C_USCI_Init(unsigned char addr)
{

		P3SEL |= BIT1 + BIT2;                     // Assign I2C pins to USCI_B0
		//P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
		UCB0CTL1 |= UCSWRST;                      // Enable SW reset
		UCB0CTL0 = UCMST+UCMODE_3+UCSYNC;         // I2C Master, synchronous mode
	  	UCB0CTL1 = UCSSEL_2+UCSWRST;              // Use SMCLK, keep SW reset
		UCB0BR0 = 40;                             // fSCL = SMCLK/40 = ~400kHz
		UCB0BR1 = 0;
		UCB0I2CSA = addr;                         // Set slave address
		UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
}

void I2C_USCI_Set_Address(unsigned char addr)
{
  	UCB0CTL1 |= UCSWRST;    
  	UCB0I2CSA = addr;                     		// Set slave address
  	UCB0CTL1 &= ~UCSWRST;                 		// Clear SW reset, resume operation 	
}

unsigned char I2C_USCI_Read_Byte(unsigned char address)
{ 	
	while (UCB0CTL1 & UCTXSTP);             	// Cho tin hieu I2C STT duoc gui di
	UCB0CTL1 |= UCTR + UCTXSTT;             	// I2C TX,START

	while (!(IFG2&UCB0TXIFG));					// Cho gui xong
	UCB0TXBUF = address;                      	// Dia chi luu gia tri Seconds

	while (!(IFG2&UCB0TXIFG));					// Cho gui xong

	UCB0CTL1 &= ~UCTR;                      // I2C RX
	UCB0CTL1 |= UCTXSTT;                    // I2C RESTART
	IFG2 &= ~UCB0TXIFG;                     // Xoa co ngat USCI_B0 TX

	while (UCB0CTL1 & UCTXSTT);             // Cho den khi I2C STT duoc gui di
	UCB0CTL1 |= UCTXSTP;                    // Gui bit STOP
	return UCB0RXBUF;
}


//doc 1 mang tu DS
unsigned char I2C_USCI_Read_Word(unsigned char Addr_Data,unsigned char *Data, unsigned char Length)
{ 	
	unsigned char i=0;
	while (UCB0CTL1 & UCTXSTP);             // Loop until I2C STT is sent
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

	while (!(IFG2&UCB0TXIFG));
	IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
	if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi
	UCB0TXBUF = Addr_Data;                      	// Dia chi luu gia tri Seconds

	while (!(IFG2&UCB0TXIFG));
	if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi

	UCB0CTL1 &= ~UCTR;                      // I2C RX
	UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
	while (UCB0CTL1 & UCTXSTT);             // Loop until I2C STT is sent
	for(i=0;i<(Length-1);i++)
	{
		while (!(IFG2&UCB0RXIFG));
		IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
		Data[i] = UCB0RXBUF;
	}
	while (!(IFG2&UCB0RXIFG));
	IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
	UCB0CTL1 |= UCTXSTP;                    // I2C stop condition after 1st TX
	Data[Length-1] = UCB0RXBUF;
	IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
	return 0;
}

unsigned char I2C_USCI_Write_Byte(unsigned char address, unsigned char data)
{
	while (UCB0CTL1 & UCTXSTP);             // Cho den khi tin hieu STT duoc gui xong
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, Gui bit START

	while (!(IFG2&UCB0TXIFG));				// Cho cho bit START gui xong
	if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi thì thoat khoi ham
	UCB0TXBUF = address;					// Gui dia chi thanh ghi can ghi

////////////////////
	while (!(IFG2&UCB0TXIFG));				// Cho gui xong
	if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi thì thoat khoi ham
	UCB0TXBUF = data;						// Gui du lieu
///////////////////
	while (!(IFG2&UCB0TXIFG));				// Cho gui xong
	if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi thì thoat khoi ham
	UCB0CTL1 |= UCTXSTP;                    // Gui bit STOP
	IFG2 &= ~UCB0TXIFG;                     // Xoa co USCI_B0 TX
	return 0;
}


//ghi 1 mang tu DS
unsigned char I2C_USCI_Write_Word(unsigned char Addr_Data,unsigned char *Data, unsigned char Length)
{
	unsigned char i=0;
	while (UCB0CTL1 & UCTXSTP);             // Loop until I2C STT is sent
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

	while (!(IFG2&UCB0TXIFG));
	IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
	if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi
	UCB0TXBUF = Addr_Data;                      	// Dia chi luu gia tri Seconds

	for(i=0;i<(Length);i++)
	{
		while (!(IFG2&UCB0TXIFG));				// Cho gui xong
		if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi thì thoat khoi ham
		UCB0TXBUF = Data[i];						// Gui du lieu
	}
	while (!(IFG2&UCB0TXIFG));				// Cho gui xong
	if(UCB0STAT & UCNACKIFG) return UCB0STAT;	//Neu bao loi thì thoat khoi ham
	UCB0CTL1 |= UCTXSTP;                    // Gui bit STOP
	IFG2 &= ~UCB0TXIFG;                     // Xoa co USCI_B0 TX
	return 0;
}
// chuyen o sang gan giong dang hexa
// vi du 12=0x0c sang 0x12
unsigned char  Dec2BCD(int dec){
	 unsigned char  L,H;
	L=dec%10;
	H=(dec/10)<<4;
	return (H+L);
}

void resetcasse(void){
	char a;
	for(a=0;a<8;a++){
		datasave[a]=5;
	}

}

// cai dat gio
void setupcase(void){
	 unsigned char  data[7];

		lcd_clear();
		lcd_puts("please wait ");

	 data[0]=0;
	 // phut
	 data[1]=Dec2BCD(datasave[1]);
	 // gio
	 data[2]=Dec2BCD(datasave[0]);
	 data[3]=2;
	 // ngay
	 data[4]=Dec2BCD(datasave[2]);
	 // thang
	 data[5]=Dec2BCD(datasave[3]);
	 // nam
	 data[6]=Dec2BCD(datasave[4]);
	 I2C_USCI_Write_Word(0,data,7);
	convertSolar2Lunar(datasave[2],datasave[3],2000+ datasave[4]);

}
// khoi tao i2c
void init_I2C(void){
unsigned char giay,phut,gio,ngay,thang,nam;

WDTCTL = WDTPW | WDTHOLD;
// khoi tao LCD
khoitao();
_delay_cycles(1000000);
lcd_puts("Ngay ");
DCOCTL = CALDCO_1MHZ;
BCSCTL1 = CALBC1_1MHZ;
BCSCTL2 = SELM_0 ;//DIVMx=0
BCSCTL3 = XCAP_3 + LFXT1S_0;
_delay_cycles(1000000);
//Stop Watchdog 
//Select_Clock(DCO_1MHZ);
//Khoi tao cong giao tiep I2C Master,Slave la DS1307	
I2C_USCI_Init(DS1307_ADDRESS);	//Khoi tao 


//LCD	LCD_Init(); //Cai dat chuc nang cho DS1307 xuat xung 1Hz
I2C_USCI_Write_Byte(0x07,BIT4); //Doc gia tri tu DS1307,tinh cac thong so thoi gian 
I2C_USCI_Read_Word(0,Temp,7);	
giay = ((Temp[0]&0x70)>>4)*10+(Temp[0]&0x0F);	
phut = ((Temp[1]&0x70)>>4)*10+(Temp[1]&0x0F);	
gio  = ((Temp[2]&0x30)>>4)*10+(Temp[2]&0x0F);
ngay = ((Temp[4]&0x30)>>4)*10+(Temp[4]&0x0F);
thang= ((Temp[5]&0x10)>>4)*10+(Temp[5]&0x0F);	
nam  = ((Temp[6]&0xF0)>>4)*10+(Temp[6]&0x0F);	//Khoi tao ngat ngoai cho chan P1.0
datasave[0]=gio;
datasave[1]=phut;
datasave[2]=ngay;
datasave[3]=thang;
datasave[4]=nam;
lcd_puts("Begin connect ");
//__bis_SR_register(GIE);

}

// show =1 cai gio
// show =2 cai phut
// show =3 cai ngay
// show =4 cai thang
// show =5 cai nam

void putdata_canlander(char input,char show){
	switch (input){
	case KEYUP: {
		if(show>0)
		datasave[show-1]++;
		break;
		}
	case KEYDOWN: {
		if(show>0)
		datasave[show-1]--;
		break;
		}
	case KEYNEWXT: {
		if(show>0)
		status++;
		if(status>5) status=1;
		break;
		}
	case KEYUPKEYNENU: {
		status=1;
		break;
		}
	case KEYUPOK: {
		//datasavve
		setupcase();
		status=0;
		break;
		}
	case KEYCANCAL: {
			//datasavve
	status=0;
			break;
			}
	default :
		break;
	}
	checkall();
	_delay_cycles(100000);

}




void action(char input,char state){
if(state==STATEHOUR){
// setting time how
putdata_canlander(input,state);
}else if(state==STATEMINUTH){
// setting time minus
putdata_canlander(input,state);
}else if(state==STATEDAY){
// setting time day
putdata_canlander(input,state);
}else if(state==STATEMOUNT){
// setting time mouth
putdata_canlander(input,state);
}else if(state==STATEYEAR){
// setting time year
putdata_canlander(input,state);
}

}
// chuyen so sang ky tu
void gettime(char time,char *number){
	char data;
	data=(time/10)+'0';
	*number=data;
	number=number+1;
	data=(time%10)+'0';
	*number=data;
}


void resetcasses(int t1,int t2,int t3,int t4,int t5,int t6,int t7){
		datasave[0]=t1;
		datasave[1]=t2;
		datasave[2]=t3;
		datasave[3]=t4;
		datasave[4]=t5;
		datasave[5]=t6;
		datasave[6]=t7;
}


void read_ALL_Times(void){
	I2C_USCI_Read_Word(0,Temp,9);
	datasave[0]=((Temp[3]&0xF0)>>4)*10+(Temp[3]&0x0F);
	datasave[1]=((Temp[2]&0xF0)>>4)*10+(Temp[2]&0x0F);
	datasave[2]=((Temp[5]&0xF0)>>4)*10+(Temp[5]&0x0F);
	datasave[3]=((Temp[6]&0xF0)>>4)*10+(Temp[6]&0x0F);
	datasave[4]=((Temp[7]&0xF0)>>4)*10+(Temp[7]&0x0F);
}

// hien thi cai dat thoi gian
void show_Point_in_time_HMI(char line){
	char time[17];
	char count=0;
	// hien thi thoi gian  trong thanh ghi data save
		lcd_clear();
		gettime(datasave[STATEHOUR],&time[0]);
		time[2]=':';
		gettime(datasave[STATEMINUTH],&time[3]);
		time[5]=' ';
		gettime(datasave[STATEDAY],&time[6]);
		time[8]='/';
		gettime(datasave[STATEMOUNT],&time[9]);
		time[11]='/';
		time[12]='2';
		time[13]='0';
		gettime(datasave[STATEYEAR],&time[14]);
		time[16]='\0';
		lcd_puts(time);
		// suong dong hien thi vi tri can tro nhap nhay
		ghilenh(0xc0);
		for(count=0;count<line;count++) time[count]=' ';
		time[count]='_';
		count++;
		time[count]='_';
		count++;
		time[count]='\0';
		lcd_puts(time);
}


void show_All_Time_In_HMI(void)
{
	char time[12];
	lcd_clear();

	lcd_puts("Ngay ");
	gettime(datasave[STATEDAY],&time[0]);
	time[2]='/';
	gettime(datasave[STATEMOUNT],&time[3]);
	time[5]='/';
	time[6]='2';
	time[7]='0';
	gettime(datasave[STATEYEAR],&time[8]);
	time[10]=' ';
	time[11]=0;
	lcd_puts(time);
	ghilenh(0xc0);
	lcd_puts("Am Lich ");
	gettime(datasave[STATEDAYLUNAR],&time[0]);
	time[2]='/';
	gettime(datasave[STATEMOUNTLUNAR],&time[3]);
	time[5]=0;
	lcd_puts(time);
}
char font_data[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void getShowTimeBLDC(char time,char *number){
	char data;
	data=font_data[time/10];
	*number=data;
	number=number+1;
	data=font_data[time%10];
	*number=data;
}
// hiên thi thoi gian gio phut

void timeshow(char port){
char time[4];
getShowTimeBLDC(datasave[STATEHOUR],&time[0]);
getShowTimeBLDC(datasave[STATEMINUTH],&time[2]);

_delay_cycles(1000);
P2DIR|=(~0xE7);
P3DIR|=(~0x3F);
P1DIR=0xff;
P1DIR=0xff;
P2OUT&=0xE7;
P3OUT&=0x3F;
// hien thi thanh ghi phan don vi phut
if(port==1){
P1OUT=time[3];
P2OUT|=0x10;
}else {
	if(port==2){
		// hien thi thanh ghi phan chuc phut
		P1OUT=time[2];
		P2OUT|=0x08;
	}else {
		if(port==3){
			// hien thi thanh ghi phan don vi gio
			P1OUT=(time[1]&0x7f);
			P3OUT|=0x80;
		} else{
			// hien thi thanh ghi phan chuc cua gio
			P1OUT=time[0];
			P3OUT|=0x40;
		}
	}
}

}

void timeshows(char port){
_delay_cycles(1000);

P1DIR=0xff;
P2OUT&=0xE7;
P3OUT&=0x3F;
P1OUT=port;
P2OUT|=0x10;


}


char checktex(char hex){
	char heli;
	if(hex<10) heli= '0'+hex;
	else heli= 'A'+hex-10;
	return heli;
}

void showhexa(char hex){
	char tex;
	tex=hex/0x10;
	if(tex<10)
	lcd_clear();
	_delay_cycles(1000);
	lcd_clear();
	_delay_cycles(1000);
	lcd_clear();
	ghikt(checktex(tex));
	tex=hex%0x10;
	_delay_cycles(1000);
	ghikt(checktex(tex));
}

char readinput(void){
char data;
P2DIR&=0xf8;
P3DIR&=0xf6;
P4DIR&=0xfE;
P2OUT|=0x07;
P3OUT|=0x09;
P4OUT|=0x01;
// phep toan giup cho chuyen cac port co vi tri khac nhau thanh 1 chuan chung 1 byte 0~0x3f
data=(P2IN&0x07)|(P3IN&0x08)|((P3IN&0x01)<<4)|((P4IN&0x01)<<5);
/*if(data!=0x3f) {
	lcd_clear();
	showhexa(data);
	_delay_cycles(1000000);
}*/
return data;
}
void getcanlerder(void){
	convertSolar2Lunar(datasave[2],datasave[3],2000+ datasave[4]);
}
