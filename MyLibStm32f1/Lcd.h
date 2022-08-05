/**
  ******************************************************************************
  * @file    Lcd.h
  * @author  Chu Xuan Quang
  * @date    29-1-2022
  * @brief   Lcd program body
  ******************************************************************************
  * @attention 
  ******************************************************************************
*/

#ifndef LCD_H
#define LCD_H

#include "stm32f10x.h"
#include "Delay.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define LCD_RS  GPIO_Pin_14
#define LCD_RW  GPIO_Pin_13
#define LCD_EN  GPIO_Pin_12
#define LCD_D4  GPIO_Pin_0
#define LCD_D5  GPIO_Pin_1
#define LCD_D6  GPIO_Pin_2
#define LCD_D7  GPIO_Pin_3
#define HIGH    (BitAction)1
#define LOW     (BitAction)0

void LCD_Config(void);                             
void LCD_Init(void);                                       
void LCD_Enable(void);                                     
void LCD_Send_4Bits(uint8_t Data);                     
void LCD_Send_Command(uint8_t command);               
void LCD_Clear(void);                                      
void LCD_Gotoxy(uint8_t x, uint8_t y);         
void LCD_Put_Char(uint8_t Data);                      
void LCD_Puts (uint8_t *s);   
void LCD_Printf(const char* str, ...);

#endif
