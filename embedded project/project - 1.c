#include<project - 1.h>
#byte lcd    = 0x06
#byte tlcd   = 0x86
#bit rs      = 0x07.0
#bit trs     = 0x87.0
#bit en      = 0x07.1
#bit ten     = 0x87.1

#byte adcon0 = 0x1F
#byte adcon1 = 0x9F
#byte adresh = 0x1e
#byte adresl = 0x9e
#bit godone  = 0x1F.2

#byte TXSTA=0x98
#byte RCSTA=0x18
#byte SPBRG=0x99
#byte TXREG=0x19
#bit  TXIF=0x0c.4

#byte SSPCON=0X14
#byte SSPSTAT=0X94
#byte SSPBUF=0X13
#bit  SSPIF=0X0C.3

#bit  sdo=0x07.5
#bit  sdi=0x07.4
#bit  clk=0x07.3
#bit  cs =0x06.0

#bit  tsdo=0x87.5
#bit  tsdi=0x87.4
#bit  tclk=0x087.3
#bit  tcs=0x86.0

void display(char x, int y)
{
   lcd = x;
   rs  = y;
   en  = 1;
   delay_ms(10);
   en  = 0;
   delay_ms(10);
}
void main()
{
  tlcd = trs = ten = 0;
  long int result, i, my_adresh = 0;
  char my_data[10];
  float percentage;
  char rx;
  
  adcon0 = 0x01;
  adcon1 = 0x8e;
  godone = 1;
  
   TXSTA = 0x24;
   RCSTA = 0x90;
   SPBRG = 64;
   
   tsdo=tclk=tcs=0;
   tsdi=1;
   SSPCON=0X20;
   SSPSTAT=0X00;
   cs=1;
   delay_ms(10);
   
  
   display(0x0c,0);  
   display(0x38,0); 
   display(0x80,0); 
 
   while(TRUE)
 {
   while(godone == 1);
   godone = 1;
   my_adresh = adresh;
   result = adresl|my_adresh<<8;
   percentage = (result * 100.0)/1023.0;
   sprintf(my_data, "volt:%04.1f\r\n", percentage);
   display(0x80,0);
   
   for(i=0; my_data[i]!='\0'; i++)
   {
      display(my_data[i],1);
      while(!TXIF);
      TXREG = my_data[i];
      delay_ms(50);
   }
     
     for(int i=0;my_data[i]!='\0'; i++)
    { cs=0;
       SSPBUF=my_data[i];//write enable.
   while(SSPIF==0); //DEFAULT==0 ==1
   
       rx = SSPBUF;
       
         SSPIF=0;
         cs=1;
         
         display(rx,1);
    }
      
         delay_ms(1000);
 }
}
