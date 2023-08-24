#ifndef _HW_H
#define _HW_H
//=============================================================================
// Definicion 
#define LCD_DB4   PIN_D3
#define LCD_DB5   PIN_D2
#define LCD_DB6   PIN_D1
#define LCD_DB7   PIN_D0
//
#define LCD_RS    PIN_C0
#define LCD_RW    PIN_C1
#define LCD_E     PIN_C2

//=============================================================================
#endif
#include "main.h"
#include "hw.h"
#include "flex_lcd.c"

int8 c1=0;
int8 c2=0;
int1 rele = 1;


#int_TIMER2
void  TIMER2_isr(void) 
{
  if(c1<100){
      c1++;
  }
  else{
      c1 = 0;
      c2++;
      if(c2 >= 50 ){
         c2 = 0;
         rele = !rele;
         output_bit(PIN_B3,rele);
      }
  }
}

void Update(){
  
}


void main()
{

   setup_adc_ports(AN0_TO_AN1|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2|ADC_TAD_MUL_0);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_1,199,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(GLOBAL);
   setup_oscillator(OSC_8MHZ|OSC_INTRC|OSC_NORMAL|OSC_PLL_OFF);
   
   lcd_init();
   lcd_gotoxy(1,1);
   output_bit(PIN_B0,0);
   output_bit(PIN_B1,0);
   output_bit(PIN_B3,rele);
while(1){
   Update();
   }
   // TODO: USER CODE!!

}
