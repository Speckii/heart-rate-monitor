/**
  ******************************************************************************
  * @file    main.c 
  * @author  Israel Kuye and Riley Griffith
  * @version V1.0.1
  * @date    14-May-2025
  * @brief   This file provides main program functions and methods for GPIO, Clock, ADC, and GLCD.
  ******************************************************************************
  */
	/** @addtogroup CORE
  * @{
  */

/** @defgroup MAIN
* @brief Main file.
* @{
*/ 

#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "Board_LED.h"
/** @defgroup MAIN_Private_Defines
* @brief defines for Delay and Clock
* @{
*/ 

#define wait_delay HAL_Delay
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
}
#endif
/**
* @}
*/ 



/** @defgroup Configuration_Functions
* @{
*/ 

/**
* @brief  Clock Configuration function
* @param  None
* @retval None
*/
//System Clock Configuration
void SystemClock(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
* @brief  ADC Configuration function for ADC handler
* @param  None
* @retval None
*/
//ADC Configuration
ADC_HandleTypeDef hadc;
static void Analogue_Convert(void) {
	ADC_ChannelConfTypeDef sConfig;
	__HAL_RCC_ADC1_CLK_ENABLE();

	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.Resolution = ADC_RESOLUTION_10B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.ScanConvMode = ENABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	HAL_ADC_Init(&hadc);

	sConfig.Rank = 1;
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
}

/**
* @brief  GPIO Initialisation Function
* @param  None
* @retval None
*/
//GPIO Init for ADC
/**
 * @brief  ADC handler used for analog input from sensor.
 */
static void Pin_Initialisaton(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~(0x03 << (0 * 2)); // Reset GPIO Mode on Pin A0
    GPIOA->MODER |=  (0x03 << (0 * 2)); // Set GPIO Mode to "Analog" on Pin A0
	GPIOA->PUPDR &= ~(0x03 << (0 * 2)); // Set "No Pull" on Pin A0
}

/**
* @brief  GLCD Screen function
* @param  None
* @retval None
*/
//Page Design
static void createHeart(void){
    GLCD_SetBackgroundColor(GLCD_COLOR_RED);
    GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
    GLCD_DrawString(0, 0, "Heart Rate Monitor");        
    GLCD_DrawString(3*24, 4*15, "  ");
    GLCD_DrawString(6*24, 4*15, "  ");
    GLCD_DrawString(2*24, 5*15, "  ");
    GLCD_DrawString(1*24, 6*15, "  ");
    GLCD_DrawString(1*24, 7*15, "  ");
    GLCD_DrawString(1*24, 8*15, "  ");
    GLCD_DrawString(2*24, 9*15, "  ");
    GLCD_DrawString(2*24, 10*15, "  ");
    GLCD_DrawString(3*24, 11*15, "  ");
    GLCD_DrawString(4*24, 12.5*15, "    ");
    GLCD_DrawString(4.6*24, 13.5*15, "  ");
    GLCD_DrawString(6*24, 11*15, "  ");
    GLCD_DrawString(7*24, 10*15, "  ");
    GLCD_DrawString(7*24, 9*15, "  ");
    GLCD_DrawString(8*24, 8*15, "  ");
    GLCD_DrawString(8*24, 7*15, "  ");
    GLCD_DrawString(8*24, 6*15, "  ");
    GLCD_DrawString(7*24, 5*15, "  ");
    GLCD_DrawString(4.2*24, 5*15, "   ");
    GLCD_DrawString(4.5*24, 6*15, "  ");
  GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
  GLCD_SetForegroundColor(GLCD_COLOR_RED);
}

/**
* @}
*/ 

/** @defgroup Main_Application_Functions
* @{
*/ 
/**
* @brief  Main function
* @param  None
* @retval None
*/

//Main Application
int main(void) {

/** @defgroup MAIN_Private_Variables
* @{
*/

// BPM variables
	/**
 * @brief  Initialise variables for heart rate computation from ADC.
 */
	uint32_t CURRENT_TIME, LAST_TIME = 0;
	uint32_t DIFFERENCE, BPM = 0;
	uint32_t beat_intervals[5] = {1000, 1000, 1000, 1000, 1000};
	uint8_t beat_index = 0;
	uint32_t total_interval = 5000;
	/**
 * @brief  Initialise variables for noise filtration and pulse detection.
 */
// ADC and signal filtering
	uint16_t raw_adc;
	float filtered_adc = 0;
	float previous_adc = 0;
	bool pulse_detected = false;
/**
* @}
*/ 


// Display buffer
	char buffer[100];
	// Initialise LED and Methods
	LED_Initialize();
	LED_Off(0U);
	HAL_Init();
	SystemClock();
	Pin_Initialisaton();
	Analogue_Convert();

	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	createHeart();
	wait_delay(2000);
	
//Application Begin
	while (1) {
		
		CURRENT_TIME = HAL_GetTick();
		raw_adc = HAL_ADC_GetValue(&hadc);

		// Low-pass filter for noise cancellation
		filtered_adc = 0.85f * filtered_adc + 0.15f * raw_adc;

		// Detect pulse between the parameters
		if (filtered_adc > previous_adc && !pulse_detected &&
			(CURRENT_TIME - LAST_TIME) > 300 && filtered_adc > 100) {
			
			pulse_detected = true;
			DIFFERENCE = CURRENT_TIME - LAST_TIME;
			LAST_TIME = CURRENT_TIME;

			// Update average (BPM)
			total_interval -= beat_intervals[beat_index];
			beat_intervals[beat_index] = DIFFERENCE;
			total_interval += beat_intervals[beat_index];
			beat_index = (beat_index + 1) % 5;

			BPM = (60000 / (total_interval / 5))/2;
		}

		// Reset pulse flag when signal drops (falling edge)
		if (filtered_adc < previous_adc) {
			pulse_detected = false;
		}

		previous_adc = filtered_adc;

		// Display BPM
		if (filtered_adc > 575) {
			GLCD_SetForegroundColor(GLCD_COLOR_RED);
			sprintf(buffer, "%u", BPM);
			GLCD_DrawString(4.5*24, 8*15, buffer);
			
		//LED Turns off if heart rate is healthy, otherwise turns on
			if (BPM > 100 || BPM < 50) {
			LED_On(0U);
			GLCD_DrawString(4.5*24, 16*15, "Heart Rate Abnormal");
			}else{
			LED_Off(0U);
			GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
			GLCD_DrawString(4.5*24, 16*15, "																				");
			}
		}
		
		// Display Blanks for Noise
		else {
			GLCD_SetForegroundColor(GLCD_COLOR_RED);
			GLCD_DrawString(4.5*24, 8*15, "--");
			GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
			GLCD_DrawString(4.5*24, 16*15, "																				");
		}

		wait_delay(100);
	}
}
/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 
  