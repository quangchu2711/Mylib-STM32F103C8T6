#include "Keypad.h"

ButtonManagement Button[16];

uint8_t Keypad4x4_Read (void)
{
	/* Make ROW 1 LOW and all other ROWs HIGH */
	GPIO_WriteBit (R1_PORT, R1_PIN, GPIO_PIN_RESET);  //Pull the R1 low
	GPIO_WriteBit (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	GPIO_WriteBit (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	GPIO_WriteBit (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High

	if (Button_OnPress(C1_PORT, C1_PIN, &Button[0]))   // if the Col 1 is low
	{
		return 1;
	}

	if (Button_OnPress(C2_PORT, C2_PIN, &Button[1]))   // if the Col 2 is low
	{
		return 2;
	}
	
	if (Button_OnPress(C3_PORT, C3_PIN, &Button[2]))   // if the Col 3 is low
	{
		return 3;
	}
	
	if (Button_OnPress(C4_PORT, C4_PIN, &Button[3]))    // if the Col 4 is low
	{
		return 4;
	}
	
	/* Make ROW 2 LOW and all other ROWs HIGH */
	GPIO_WriteBit (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	GPIO_WriteBit (R2_PORT, R2_PIN, GPIO_PIN_RESET);  // Pull the R2 High
	GPIO_WriteBit (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	GPIO_WriteBit (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	
	if (Button_OnPress(C1_PORT, C1_PIN, &Button[4]))   // if the Col 1 is low
	{
		return 5;
	}

	if (Button_OnPress(C2_PORT, C2_PIN, &Button[5]))   // if the Col 2 is low
	{
		return 6;
	}
	
	if (Button_OnPress(C3_PORT, C3_PIN, &Button[6]))   // if the Col 3 is low
	{
		return 7;
	}
	
	if (Button_OnPress(C4_PORT, C4_PIN, &Button[7]))    // if the Col 4 is low
	{
		return 8;
	}
		
	/* Make ROW 3 LOW and all other ROWs HIGH */
	GPIO_WriteBit (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	GPIO_WriteBit (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	GPIO_WriteBit (R3_PORT, R3_PIN, GPIO_PIN_RESET);  // Pull the R3 High
	GPIO_WriteBit (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	
	if (Button_OnPress(C1_PORT, C1_PIN, &Button[8]))   // if the Col 1 is low
	{
		return 9;
	}

	if (Button_OnPress(C2_PORT, C2_PIN, &Button[9]))   // if the Col 2 is low
	{
		return 10;
	}
	
	if (Button_OnPress(C3_PORT, C3_PIN, &Button[10]))   // if the Col 3 is low
	{
		return 11;
	}
	
	if (Button_OnPress(C4_PORT, C4_PIN, &Button[11]))    // if the Col 4 is low
	{
		return 12;
	}	
		
	/* Make ROW 4 LOW and all other ROWs HIGH */
	GPIO_WriteBit (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	GPIO_WriteBit (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	GPIO_WriteBit (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	GPIO_WriteBit (R4_PORT, R4_PIN, GPIO_PIN_RESET);  // Pull the R4 High
	
	if (Button_OnPress(C1_PORT, C1_PIN, &Button[12]))   // if the Col 1 is low
	{
		return 13;
	}

	if (Button_OnPress(C2_PORT, C2_PIN, &Button[13]))   // if the Col 2 is low
	{
		return 14;
	}
	
	if (Button_OnPress(C3_PORT, C3_PIN, &Button[14]))   // if the Col 3 is low
	{
		return 15;
	}
	
	if (Button_OnPress(C4_PORT, C4_PIN, &Button[15]))    // if the Col 4 is low
	{
		return 16;
	}
	
	return 0;
}

void Keypad4x4_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	/*Configuration GPIO pin*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*Configuration GPIO pin*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	for (uint8_t i = 0; i < 16; i++)
	{
		Button[i].changeTime = 0;
		Button[i].flags = 0x00;
		Button[i].holdTime = 1000;
	}
}
	

