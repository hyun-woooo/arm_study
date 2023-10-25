#include "uart.h"

extern uint8_t rx_data;
extern uint8_t bt_rx_data;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

// HW와 SW의 만나는 약속장소 : call back function
// move from HAL_UART_RxCpltCallback of stm32f4xx_hal_uart to here1
// uart로 1 byte가 수신되면 H/W가 call을 해 준다.
// UART RX INT가 발생이 되면 이곳으로 자동적으로 들어온다.
// comport master
// ledallon\n
// ledalloff\n
// led_toggle\n
// 문자 하나를 받을 때마다 콜백 함수에 들어간다.
// 숙제 1번
// 2차원 array circular queue를 구성하여 overflow가 발생 되지 않도록 구현해야한다.


#define COMMAND_LENGTH 40
#define MAX_COMMAND 20

extern void led_all_on(void);
extern void led_all_off(void);
extern void led_on_up(void);
extern void led_on_down(void);
extern void keepon_up(void);
extern void keepon_down(void);
extern void flower_on(void);
extern void flower_off(void);

void bt_command_processing(void);
void pc_command_processing(void);

volatile unsigned char rx_queue[MAX_COMMAND][COMMAND_LENGTH];
// UART3으로부터 수신된 char를 저장 하는 공간 (\n을 만날때까지)
volatile int rx_index=0; //rx_buff의 save 위치
volatile int newline_detect_flag=0; // new line을 만났을 때의 indicator 예) ledallon\n
volatile int rx_num=0;


volatile unsigned char bt_rx_buff[COMMAND_LENGTH]; // UART6으로부터 수신된 char를 저장 하는 공간 (\n을 만날때까지)
volatile int bt_rx_index=0; //bt_rx_buff의 save 위치
volatile int bt_newline_detect_flag=0; // new line을 만났을 때의 indicator 예) ledallon\n
volatile int rear =0; // 함수가 들어오면 rear는 늘어난다
volatile int front =0; // 함수가 실행이 되면 -가 되고


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // 함수를 받고 저장하는 과정
{
	if(huart == &huart3) // comport master와 연결된 uart
	{

		if(front == rear +1)
		{
			printf("over flow\n");
		}
		else
		{
			if(rx_index < COMMAND_LENGTH) // 현재까지 들어온 byte가 40byte를 넘지 않으면
													// 함수가 추가 되었을 때
			{
				if(rx_data == '\n' || rx_data == '\r')
				{
					rx_queue[rear][rx_index] = 0; // '\0'
					rx_index = 0; // 다음 message 저장을 위해서 rx_index값을 0으로 한다.
					printf("%s reserve\n" ,rx_queue[rear]);
					rear++; // rear을 증감시킴으로써 다음 저장을 기다린다.
					rear %= MAX_COMMAND; // 선형 큐를 사용하기때문에 나눠줌
				}
				else
				{
					rx_queue[rear][rx_index++]=rx_data;
				}
			}
			else
				{
					rx_index=0;
					printf("Message Overflow !!! \n");
				}
		}

		HAL_UART_Receive_IT(&huart3, &rx_data, 1);
	}




	if(huart == &huart6) // BT 와 연결된 uart
		{
			if(bt_rx_index < COMMAND_LENGTH) // 현재까지 들어온 byte가 40byte를 넘지 않으면
			{
				if(bt_rx_data == '\n' || bt_rx_data == '\r')
				{
					bt_rx_buff[bt_rx_index] = 0; // '\0'
					bt_newline_detect_flag = 1; // new line을 만났다는 flag를 set한다.
					bt_rx_index = 0; // 다음 message 저장을 위해서 rx_index값을 0으로 한다.
				}
				else
				{
					bt_rx_buff[bt_rx_index++]=bt_rx_data;
				}
			}
			else
				{
						bt_rx_index=0;
						printf("BT Message Overflow !!! \n");
				}
			HAL_UART_Receive_IT(&huart6, &bt_rx_data, 1);
		}



}


void pc_command_processing(void)
{
	if(front==rear)
	{
		return;
	}
	else
	{
		if(!strncmp(rx_queue[front],"led_all_on",strlen("led_all_on")))
		{
			led_all_on();
			printf("%s excute : %d\n",rx_queue[front],front);

			front++;
			front %= MAX_COMMAND;
			return;
		}
		if(!strncmp(rx_queue[front],"led_all_off",strlen("led_all_off")))
		{
			led_all_off();
			printf("%s excute : %d\n",rx_queue[front],front);

			front++;
			front %= MAX_COMMAND;
			return;
		}
		if(!strncmp(rx_queue[front],"led_on_down",strlen("led_on_down")))
		{
			led_on_down();
			printf("%s excute : %d\n",rx_queue[front],front);

			front++;
			front %= MAX_COMMAND;
			return;
		}
		if(!strncmp(rx_queue[front],"led_on_up",strlen("led_on_up")))
		{
			led_on_up();
			printf("%s excute : %d\n",rx_queue[front],front);

			front++;
			front %= MAX_COMMAND;
			return;
		}
		if(!strncmp(rx_queue[front],"keepon_up",strlen("keepon_up")))
		{
			keepon_up();
			printf("%s excute : %d\n",rx_queue[front],front);

			front++;
			front %= MAX_COMMAND;
			return;
		}
		if(!strncmp(rx_queue[front],"keepon_down",strlen("keepon_down")))
		{
			keepon_down();
			printf("%s excute : %d\n",rx_queue[front],front);

			front++;
			front %= MAX_COMMAND;
			return;
		}
		if(!strncmp(rx_queue[front],"flower_on",strlen("flower_on")))
		{
			flower_on();
			printf("%s excute : %d\n",rx_queue[front],front);

			front++;
			front %= MAX_COMMAND;
			return;
		}
		if(!strncmp(rx_queue[front],"flower_off",strlen("flower_off")))
		{
			flower_off();
			printf("%s excute : %d\n",rx_queue[front],front);
			front++;
			front %= MAX_COMMAND;

			return;
		}
	}
}


void bt_command_processing(void)
{
	if(bt_newline_detect_flag)//BT로 부터 완전한 문장이 들어오면 (\n을 만나면)
	{
		bt_newline_detect_flag =0;
		printf("%s\n",bt_rx_buff);
		if(!strncmp(bt_rx_buff,"led_all_on",strlen("led_all_on")))
		{
			led_all_on();
			return;
		}
		if(!strncmp(bt_rx_buff,"led_all_off",strlen("led_all_off")))
		{
			led_all_off();
			return;
		}
		if(!strncmp(bt_rx_buff,"led_on_down",strlen("led_on_down")))
		{
			led_on_down();
			return;
		}
		if(!strncmp(bt_rx_buff,"led_on_up",strlen("led_on_up")))
		{
			led_on_up();
			return;
		}
		if(!strncmp(bt_rx_buff,"keepon_up",strlen("keepon_up")))
		{
			keepon_up();
			return;
		}
		if(!strncmp(bt_rx_buff,"keepon_down",strlen("keepon_down")))
		{
			keepon_down();
			return;
		}
		if(!strncmp(bt_rx_buff,"flower_on",strlen("flower_on")))
		{
			flower_on();
			return;
		}
		if(!strncmp(bt_rx_buff,"flower_off",strlen("flower_off")))
		{
			flower_off();
			return;
		}

	}
}
