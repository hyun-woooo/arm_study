#include "led.h"

void led_main(void);
void led_all_on(void);
void led_all_off(void);
void led_on_up(void);
void led_on_down(void);
void keepon_up(void);
void keepon_down(void);
void flower_on(void);
void flower_off(void);

void led_main(void)

{
	while(1)
	{
		flower_on();
		flower_off();
		//phase#4
//		keepon_up();
//		led_all_off();
//		keepon_down();
//		led_all_off();
		//phase#3
		//led_on_up();
		//led_on_down();
		//phase#2
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_All);
		//HAL_Delay(500);
		//phase#1
//		led_all_on();
//		HAL_Delay(300);
//		led_all_off();
//		HAL_Delay(300);

		// led_array();

	}
}
void flower_on(void)
{
	int num = 1;
	int i = 4;
	int j = 3;
	while(1)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01<<i, 1);
		HAL_GPIO_WritePin(GPIOD, 0x01<<j, 1);
		i++;
		j--;
		if(j<-1)
		{
			break;
		}
		HAL_Delay(num*100);
		num++;
	}
}
void flower_off(void)
{
	int num =4;
	int i = 0;
	int j = 7;

	while(1)
	{

		HAL_GPIO_WritePin(GPIOD, 0x01<<i, 0);
		HAL_GPIO_WritePin(GPIOD, 0x01<<j, 0);
		i++;
		j--;
		HAL_Delay(200);
		if(j==2)
		{
			break;
		}
		HAL_Delay(num*100);
		num--;
	}
}
void keepon_up(void)
{
	for(int i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01<<i, 1);
		HAL_Delay(200);
	}


}
void keepon_down(void)
{
	for(int i=7;i>=0;i--)
	{

		HAL_GPIO_WritePin(GPIOD, 0x01<<i, 1);
		HAL_Delay(200);
	}
}
void led_on_up(void)
{


	for(int i=0;i<8;i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x01<<i, 1);
		HAL_Delay(200);
	}
}

void led_on_down(void)
{
	for(int i=7;i>=0;i--)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x01<<i, 0);
		HAL_Delay(200);
	}
}

void led_all_on(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
//			GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1); // 1 = set 0 = reset
	HAL_GPIO_WritePin(GPIOD,0xff,1);
}
void led_all_off(void)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
//			GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);  // 0 = reset
	HAL_GPIO_WritePin(GPIOD,0xff,0);
}

