#include "Lcd.h"

void LCD_Config(void)
{
	GPIO_InitTypeDef GPIO_PinLCD_InitStruction;                 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);               
	GPIO_PinLCD_InitStruction.GPIO_Mode = GPIO_Mode_Out_PP;              
	GPIO_PinLCD_InitStruction.GPIO_Pin = LCD_D4|LCD_D5|LCD_D6|LCD_D7;    
	GPIO_PinLCD_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;             
	GPIO_Init(GPIOA, &GPIO_PinLCD_InitStruction);  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinLCD_InitStruction.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_PinLCD_InitStruction.GPIO_Pin = LCD_RS|LCD_RW|LCD_EN;
	GPIO_PinLCD_InitStruction.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_PinLCD_InitStruction);
}	

void LCD_Init(void)
{
	LCD_Config();                               				/*Cau hinh cac chan cho Lcd*/
	LCD_Send_4Bits(0x00);                               /*Dat cac chan D4 D5 D6 D7 ve muc 0*/                                       
	GPIO_WriteBit(GPIOB, LCD_RS, LOW);         					/*Dat chan RS ve muc 0*/
	GPIO_WriteBit(GPIOB, LCD_RW, LOW);         					/*Dat chan RW ve muc 0*/
	
	delay_ms(20);                                       /*Doi sau khi nguon khoi dong*/
	
	LCD_Send_4Bits(0x03);                               /*Theo thuat toan giao tiep 4 bit trong HD44780 datasheet*/
	LCD_Enable();
	delay_ms(5);
	LCD_Send_4Bits(0x03);
	LCD_Enable();
	delay_us(100);
	LCD_Send_4Bits(0x03);
	LCD_Enable();
	
	LCD_Send_Command(0x02);															 /*Di chuyen con tro ve vi tri dau man hinh*/
	LCD_Send_Command(0x28);                              /*Giao tiep 4 bit cho Lcd*/
	LCD_Send_Command(0x0c);                              /*Bat va hien thi tat con tro*/
	LCD_Send_Command(0x06);                              /*Tu dong di chuyen con tro den vi tri kr tiep*/
	LCD_Send_Command(0x01);                              /*Xoa toan bo noi dung tren man hinh*/
	delay_ms(100);
}	

void LCD_Enable(void)
{
	GPIO_WriteBit(GPIOB, LCD_EN, HIGH);            			 /*Xung High*/          
	delay_us(3);                            						 /*co do rong la 3us*/
	GPIO_WriteBit(GPIOB, LCD_EN, LOW);          				 /*Xung Low*/
	delay_us(50);  
}	

void LCD_Send_4Bits(uint8_t Data)
{
	GPIO_WriteBit(GPIOA, LCD_D4, (BitAction) (Data & 0x01));         /*Ghi bit thu nhat tu bit ben phai cua Data*/
	GPIO_WriteBit(GPIOA, LCD_D5, (BitAction)((Data>>1)&1));          /*Ghi bit thu hai tu bit ben phai cua Data*/
	GPIO_WriteBit(GPIOA, LCD_D6, (BitAction)((Data>>2)&1));          /*Ghi bit thu ba tu bit ben phai cua Data*/ 
	GPIO_WriteBit(GPIOA, LCD_D7, (BitAction)((Data>>3)&1));          /*Ghi bit thu tu tu bit ben phai cua Data*/
}	

void LCD_Send_Command(uint8_t command)
{
	LCD_Send_4Bits(command >>4);                        /*Lay 4 bits cao*/
	LCD_Enable();                                       /*Tao xung tren chan E cho phep truyen lenh*/
	LCD_Send_4Bits(command);                            /*Lay 4 bits thap*/
	LCD_Enable();                                       /*Tao xung tren chan E cho phep truyen lenh*/
}	

void LCD_Clear(void)
{
	LCD_Send_Command(0x01);                              /*Xoa tat ca du lieu tren man hinh*/
}	

void LCD_Gotoxy(uint8_t x, uint8_t y)
{
	uint8_t address;                                    /*Dia chi cua con tro*/
	if(!x) address=(0x80+y);                            /*Di chuyen con tro ve dau dong 1*/
	else address=(0xc0+y);                              /*Di chuyen con tro ve dau dong 2*/   
	LCD_Send_Command(address);                          /*Truyen con tro toi vi tri mong muon*/  
}	

void LCD_Put_Char(uint8_t Data)
{
	GPIO_WriteBit(GPIOB, LCD_RS, HIGH);         /*Keo chan RS len muc 1*/
	LCD_Send_Command(Data);                             /*Gui du lieu*/
	GPIO_WriteBit(GPIOB, LCD_RS, LOW);         /*Keo chan RS len muc 0*/
}	

void LCD_Puts (uint8_t *s)
{
	while (*s)                                          /*Kiem tra do dai cua chuoi*/
	{
		LCD_Put_Char(*s);                                 /*Hien thi tung ky tu trong chuoi*/ 
		s++;
	}
}

void LCD_Printf(const char* str, ...)
{
  char stringArray[20];
	
  va_list args;
  va_start(args, str);
  vsprintf(stringArray, str, args);
  va_end(args);
	
  for(uint8_t i=0;  i<strlen(stringArray) && i<16; i++)
  {
    LCD_Put_Char((uint8_t)stringArray[i]);
  }
}

