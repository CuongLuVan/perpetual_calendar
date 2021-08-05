
#include "commucation.h"
#include "conmon.h"
#include <stdio.h>
/*
 * main.c
 */

/*
0  c0
1 F9
2 A4
3 b0
4 99
5 92
6 82
7 F8
8 80
9 90

*/

extern char status;
extern int datasave[10];
int main(void) {
	char dem;
	int lanluot;
	char linepoint[6]={0,STATE_CHANGE_HOUR,STATE_CHANGE_MINUTH,STATE_CHANGE_DAY,STATE_CHANGE_MOUNTH ,STATE_CHANGE_YEAR};
	int day,dat,year;
	char button;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P2DIR|=(~0xE7);
    P3DIR|=(~0x3F);
    P1DIR=0xff;
    init_I2C();
    dem=0;
    lanluot=0;

    //resetcasse();
    //setupcase();
    status=0;
    read_ALL_Times();
    getcanlerder();
    show_All_Time_In_HMI();
    /*while(1){
    	show_All_Time_In_HMI();
    	dem++;

    		timeshow(dem);


    		if(dem>4) dem=0;

    }*/
    status=0;
    day=0;
while(1){
	button=readinput();
	// neu la 0x0011 1111 va chua co trang thai nut bam
	if((button==0x3f)&&(status==0))
	{
			lanluot++;
			if(lanluot>4000){
				read_ALL_Times();
				show_All_Time_In_HMI();
				lanluot=0;
			}
			dem++;
			timeshow(dem);
			if(dem>5) dem=0;
			if((day!=datasave[2])||(dat!=datasave[3])||(year!=datasave[4])){
				day=datasave[2];
				dat=datasave[3];
				year=datasave[4];
				getcanlerder();
				show_All_Time_In_HMI();

			}
	}
	else {
		putdata_canlander(button,status);
		// hien thi o vi tri status
		show_Point_in_time_HMI(linepoint[status]);
	}
}
	
	return 0;
}
