
#include "common.h"
#include "led.h"
#include "usart.h"
#include "systick.h"
#include "usbcommon.h"
#include "usb_lib.h"
#include "esp8266.h"
#include "i2c.h"
#include "board.h"
#include "bh1750.h"
#include "bmp180.h"

static void Init()
{
	SysTick_Init();
	LED_Config();
	USARTx_Config(USART_DBG, 115200);
	USBCommon_Init();
	ESP8266_Init();
	ESP8266_Enable_CDC_Forwarding();
}

static void ExtInit()
{
#ifdef ENABLE_DHT11
#endif
#ifdef ENABLE_BH1750
	BH1750_Init();
#endif
#ifdef ENABLE_BMP180
	BMP180_Init();
#endif
}

int main(void)
{

	Init();
	ExtInit();

	LED_GREEN(true);

	DBG_MSG( "Usb Init Started");
	USB_Init();

	DBG_MSG( "Usb Init Succeeded");
	LED_BLUE(true);

	while (1)
	{
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
