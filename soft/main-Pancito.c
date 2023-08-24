#include "main.h"
#include "hw.h"
#include "flex_lcd.c"

int1 on = 1;
int1 opressed = 0;
int1 pressed = 0;
int1 tick = 0;
int8 c1=0;
int8 c2=0;
int8 mode = 0;
int8 maxt = 75;
int8 mint = 25;
int8 maxh = 75;
int8 minh = 25;
float oldtemp = 0;
float oldhum = 0;
int8 selectedo = 0;
int8 line = 0;

int8 CheckBut(int8 selectedb=100){
   
   int8 a = input_c();
   int8 b = 0;
   int8 i; 
   if(selectedb == 100){
      for(i = 3; i<8;i++){
      if(bit_test(a,i)){
         b = i-2;
         break;
         }
      }
   }
   else{
      switch(selectedb){
      case 9:
         b=(input(PIN_D7));
         break;
      default:
         b = bit_test(a,selectedb+2);
         break;
      }
   }
   return(b);
}

void WriteConfig(int8 selectedo, int8 lines, int8 v){
            if (selectedo==lines) printf(LCD_PUTC,"< ");
            printf (LCD_PUTC "%u", v);
            if (selectedo==lines) printf(LCD_PUTC," >");
            printf (LCD_PUTC, "\n");
}

void Update(int8 but = 0){
   
   //Default
   switch(mode){
      case 0:
         set_adc_channel(0);
         delay_us(10);
         float temp = (float)read_adc()* 100 / 255;
         set_adc_channel(1);
         delay_us(10);
         float hum = (float)read_adc()* 100 / 255;
         //Regar
         if(hum<minh)output_bit(PIN_B0,1);
         else if(hum>maxh) output_bit(PIN_B0,0);
         if(temp<mint)output_bit(PIN_B1,1);
         else if(temp>maxt) output_bit(PIN_B1,0);
         if(temp != oldtemp || hum != oldhum){
         
            lcd_gotoxy(1,1);
            printf(LCD_PUTC,"\f");
            printf(LCD_PUTC,"C: %f \nHumedad: %f%%", temp, hum);
            oldtemp= temp;
            oldhum = hum;
         }
            if(but && !pressed){
               printf(LCD_PUTC,"\f");
               pressed = 1;
               mode = 1;
            }
      break;
      
      //Config
      case 1:
         if(but){
            int8 selectedv = 0;
            switch(but){
               case 1:
                  if(!pressed){
                     if(line>0) line -=1;
                     if(selectedo>0){
                        selectedo -=1;
                     }
                  }
               break;
               case 5:
                  if(!pressed){
                     if(line<2) line +=1;
                     if(selectedo<3){
                        selectedo +=1;
                     }
                  }
               break;
               case 2:
                  selectedv =-1;
               break;
               case 4:
                  selectedv =1;
               break;
               
               case 3:
                  if(!pressed){
                     pressed = 1;
                     mode = 0;
                     oldtemp= 0;
                     oldhum = 0;
                  }
               break;
            }
            switch(selectedo){
               case 0: maxt+=selectedv;
               break;
               case 1: mint+=selectedv;
               break;
               case 2: maxh+=selectedv;
               break;
               case 3: minh+=selectedv;
               break;
               pressed = 1;
               printf(LCD_PUTC,"\f");
            }
            if(line<1){
               printf(LCD_PUTC,"MaxT: ");
               WriteConfig(selectedo,0,maxt);
            }
            if(line<2){
               printf(LCD_PUTC,"MinT: ");
               WriteConfig(selectedo,1,mint);
            }
            if(line < 3 && line >  0){
               printf(LCD_PUTC,"MaxH: ");
               WriteConfig(selectedo,2,maxh);
            }
            if(line < 4 && line >  1){
               printf(LCD_PUTC,"MinH: ");
               WriteConfig(selectedo,3,minh);
            }
            lcd_gotoxy(1,1);
         }
         break;
         //OFF
      
         //Debug
         case 3:
         
         break;
         
}
   
}



   
//100us
#int_TIMER2
void  TIMER2_isr(void) 
{
   if (c1 >= 9){
   //1ms
      c1=0;
         if (c2 >= 99){
            c2 = 0;
            tick = 1;
         }
         else c2++;
   }
   else c1++;
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
   lcd_gotoxy(1,1);
   while(1){
   
      int8 but = CheckBut();
      if(on && tick){
         Update(but);
         
      tick=0;
      }
//!      else {
//!      if(CheckBut(9) && !opressed){
//!         on = 1;
//!         opressed = 1;
//!         
//!         lcd_gotoxy(1,1);
//!         printf(LCD_PUTC,"�f");
//!         printf(LCD_PUTC,"C: %f \nHumedad: %f%%", oldtemp, oldhum);
//!      }}
      int1 onb=CheckBut(9);
      if(!opressed){
         if(onb){
         on=!on;
         opressed = 1;
         }
      }
      else if(!onb) opressed=0;
      if(but == 0 && pressed){
         pressed = 0;
      }
      
   }

}
