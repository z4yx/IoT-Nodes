
#include "common.h"
#include "led.h"
#include "usart.h"
#include "systick.h"
#include "usbcommon.h"
#include "usb_lib.h"
#include "esp8266.h"
#include "iot_node.h"
#include "board.h"
#include "func.h"

static void Init()
{
	SysTick_Init();
	LED_Config();
	USARTx_Config(USART_DBG, 115200);
	USBCommon_Init();
}

int main(void)
{
	bool upgradeMode = ESP8266_UpgradeModeDetected();

	Init();

	DBG_MSG("----- IoT-Node Power On -----");
	DBG_MSG("Clock Source: %d", RCC_GetSYSCLKSource());
	ESP8266_Init(upgradeMode);

#if defined(ESP_DBG_CDC)
	if(USBDevice_PlugIn())
#else
	if(upgradeMode && USBDevice_PlugIn())
#endif
	{
		ESP8266_Enable_CDC_Forwarding();
		DBG_MSG( "Usb Init Started");
		USB_Init();
		Delay_ms(1000);
		INFO_MSG("Usb Init Succeeded");
	}
	
	Delay_ms(200);

	LED_BLUE(true);

	if(upgradeMode){
		INFO_MSG("ESP8266 Upgrade Mode...");
		while(true){
			LED_BLUE(true);
			Delay_ms(200);
			LED_BLUE(false);
			Delay_ms(200);
		}
	}
	else{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_GPIOClockCmd(GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		// IoTNode_Begin();
		for(;;){
			int pin_val = ESP8266_ReadIO2();
			GPIO_WriteBit(GPIOB, GPIO_Pin_11, pin_val);
		}
	}
}

#ifdef  USE_FULL_ASSERT

/**
	* @brief  Reports the name of the source file and the source line number
	*         where the assert_param error has occurred.
	* @param  file: pointer to the source file name
	* @param  line: assert_param error line source number
	* @retval None
	*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}

#endif
