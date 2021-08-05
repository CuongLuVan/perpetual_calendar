#include <msp430.h>
#include "default_canleder.h"
extern int vitrikytu,contro;
extern char buffer[30];


#define CONTROLDEVICE1 "espi"
#define CONTROLDEVICE2 "emax"
#define CONTROLDEVICE3 "ecik"
#define CONTROLDEVICE4 "numv"
#define CONTROLCONECT "cnex"
#define CONTROLREQUEST "request"


#define CONTROLEND "endi"





typedef enum {
OPENDEVICE1=1,
OPENDEVICE2=2,
OPENDEVICE3=4,
OPENDEVICE4=8	
} OPENDEVICE;

extern int datasave[10];

long getJulius(long intNgay, long intThang, long intNam);
long getNewMoonDay(long k);
long getSunLongitude(long jdn);
long getLunarMonthll(long intNam);
long getLeapMonthOffset(double a11);
int convertSolar2Lunar(int intNgay, int intThang, int intNam);



