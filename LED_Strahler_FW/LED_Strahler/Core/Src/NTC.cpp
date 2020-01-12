/*
 * NTC.c
 *
 *  Created on: 03.11.2019
 *      Author: KaiLi
 */

#include "NTC.h"



extern ADC_HandleTypeDef hadc;



//Array with values from -40 to +125°C in 5°C steps
const uint16_t NTC_Lookup[] =
{
	4031,
	4002,
	3961,
	3905,
	3830,
	3733,
	3608,
	3454,
	3270,
	3059,
	2824,
	2571,
	2309,
	2048,
	1794,
	1556,
	1339,
	1143,
	972,
	822,
	694,
	586,
	496,
	420,
	357,
	304,
	260,
	223,
	191,
	165,
	143,
	124,
	108,
	94
};



void NTC_Init(void)
{
	HAL_ADC_Start(&NTC_ADC_HANDLE);
	HAL_Delay(2); //Give ADC some time to get at least one sample
}



float NTC_Get_Temp(void)
{
	uint8_t i;
	uint16_t adc_val = (uint16_t)HAL_ADC_GetValue(&NTC_ADC_HANDLE);

	for(i = 0; i < (sizeof(NTC_Lookup)/sizeof(uint16_t)); i++)
	{
		if(adc_val > NTC_Lookup[i])
		{
			break;
		}
	}

	if((i == 0) || (i == sizeof(NTC_Lookup)/sizeof(uint16_t)))
	{
		return ((((float)i) * 5.0) - 40.0);
	}

	return ((((float)i) * 5.0) - 45.0 - (5.0 * ((float)(adc_val - NTC_Lookup[i])) / ((float)(NTC_Lookup[i - 1] - NTC_Lookup[i]))));
}
