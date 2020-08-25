/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 25/8/2020                          */
/*****************************************************/
#ifndef SYSTICK_PRIVATE_H
#define SYSTICK_PRIVATE_H

/* Struct for the AFIO regsiter mapping */
typedef struct {
	volatile u32 CTRL      ;    /* SysTick control and status register */
	volatile u32 LOAD      ;    /* SysTick reload value register       */
	volatile u32 VAL       ;    /* SysTick reload value register       */
	volatile u32 CALIB     ;    /* SysTick calibration value register  */
}SysTick_t;


#define STK  ((volatile SysTick_t *) 0xE000E010)



#endif