#include "main.h"

extern TIM_HandleTypeDef htim2;
void servo_motor_test_main(void);

 // 8400000Hz / 1680 = 50000HZ
 // T = 1/f = 1/50000HZ -> 0.00002sec (20us)
// 2ms (180도) : 0.00002 x 100개
// 1.5ms(90도) : 0.00002 * 75개
// 1ms(0도) : 0.00002 * 50개
void servo_motor_test_main(void)
{
	while(1)
	{
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,140);
		HAL_Delay(1000);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1);
		HAL_Delay(1000);

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,70);
		HAL_Delay(1000);
	}
}
