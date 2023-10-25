#include "button.h"



char button_status[BUTTON_NUMBER]=
{
		BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE
};

// get_button(GPIO,PIN,BUTTON번호)
// 완전히 눌렀다 때면은 BUTTON_RELEASE(1)를 리턴한다.
int get_button(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN,uint8_t button_number)
{
	unsigned char curr_state;

	curr_state = HAL_GPIO_ReadPin(GPIO, GPIO_PIN);//0,1

	if(curr_state == BUTTON_PRESS && button_status[button_number]== BUTTON_RELEASE)
	{
		HAL_Delay(80); //noise가 지나가기를 기다린다.
		button_status[button_number]=BUTTON_PRESS;
		return BUTTON_RELEASE; // 버튼이 눌려진 상태이나 아직은 noise상태로 인정
	}
	else if(curr_state == BUTTON_RELEASE && button_status[button_number]== BUTTON_PRESS)
	{	// 이전에 버튼이 눌려진 상태였고 지금은 버튼을 뗀 상태이면
		button_status[button_number] = BUTTON_RELEASE; // button_status table을 초기화하고
		HAL_Delay(30);
		return BUTTON_PRESS; // 버튼 1번을 눌렀다 뗀것으로 인정한다.
	}
	else
	return BUTTON_RELEASE;
}
