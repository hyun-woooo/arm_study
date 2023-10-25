#include "button.h"
#include "dcmotor.h"


extern TIM_HandleTypeDef htim4;
extern void led_all_off(void);

void dcmotor_pwm_control(void);
void dcmoter_init(void);

// BUTTON0 : start/stop : LED1 (toggle)
// BUTTON1 : SPEED-UP : LED2 (toggle)
// BUTTON2 : SPEED-DOWN : LED3 (toggle)

uint8_t pwm_start_flag=0; // dcmotor start/stop indicator flag
uint16_t CCR_UP_Value = 0; // PWM UP control
uint16_t CCR_DOWN_Value = 0; // PWM DOWN control
uint8_t auto_flag =0; // default = manual 기능을 선택했을 때 1을 온한다.
uint8_t led_count =0;
uint8_t toggle_flag=0;
GPIO_PinState bitstatus; // blue button 사용


void dcmoter_init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPOI_PIN_13,GPIO_PIN_RESET);
}

void dcmotor_pwm_control(void)
{
	//start/stop : button 0
	// defalut

	bitstatus ==HAL_GPIO_ReadPin(GPIOC,GPOI_PIN_13);
	if(bitstatus == GPIO_PIN_RESET)
	{
		auto_flag=0;
		bitstatus = GPIO_PIN_SET;
	}
	else
	{
		auto_flag =1;
		bitstatus = GPIO_PIN_RESET;
	}


	if(get_button(BUTTON0_GPIO_Port,BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);

		if(!pwm_start_flag)
		{
			pwm_start_flag =1;
			HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);


		}
		else
		{
			pwm_start_flag=0;
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
			led_all_off();
			led_count = 0;
		}
	}
	// speed-up: button 1
	if(get_button(BUTTON1_GPIO_Port,BUTTON1_Pin, 1) == BUTTON_PRESS)
	{
		CCR_UP_Value = __HAL_TIM_GET_COMPARE(&htim4,TIM_CHANNEL_1);
		CCR_UP_Value += 4;

		if(led_count > 8)
		{
			led_count = 7;
		}
		else
		{

			HAL_GPIO_WritePin(GPIOD, 0x01<<led_count, 1);
			led_count++;
			printf("led count : %d",led_count);
		}

		printf("CCR_UP_Value : %d",CCR_UP_Value);
		if(CCR_UP_Value > 100) CCR_UP_Value= 100;
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,CCR_UP_Value);
	}
	// speed-down : button 2
	if(get_button(BUTTON2_GPIO_Port,BUTTON2_Pin, 2) == BUTTON_PRESS)
		{
			CCR_DOWN_Value = __HAL_TIM_GET_COMPARE(&htim4,TIM_CHANNEL_1);
			CCR_DOWN_Value -= 4;
			if(led_count < 0)
			{
				led_count =0;
			}
			else
			{
				led_count--;
				HAL_GPIO_WritePin(GPIOD, 0x01<<led_count, 0);

			}

			printf("CCR_DOWN_Value : %d",CCR_DOWN_Value);
			if(CCR_DOWN_Value < 60) CCR_DOWN_Value= 60;
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,CCR_DOWN_Value);
		}
	if(get_button(BUTTON3_GPIO_Port,BUTTON3_Pin, 3) == BUTTON_PRESS)
		{
			if(auto_flag==1)
			{
				if(toggle_flag =1)
				{
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
					toggle_flag = 0;
				}
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);

				auto_flag = 0;
			}
			else if(auto_flag==0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

				auto_flag = 1;
				toggle_flag = 1;
			}
	}
}
