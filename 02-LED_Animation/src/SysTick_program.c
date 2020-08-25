/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 25/8/2020                          */
/*****************************************************/
/* Library includes */
#include "BIT_MATH.h"
#include "STD_TYPES.h"

/* Module includes */
#include "SysTick_interface.h"
#include "SysTick_private.h"
#include "SysTick_config.h"


void SysTick_voidInit(void){
	STK->CTRL = 0x03 ;
	STK->LOAD = 100000; // 100 ms
}








