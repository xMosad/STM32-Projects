/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/* Library */
#include "stdlib.h"
#include "STD_TYPES.h"

/* Services layer */
#include "OS_interface.h"

/* APP layer */
#include "SnakeAPP_interface.h"

void main(void){

	/* Initialize the system */
	SnakeAPP_voidInitSys();

	/* Initialize the OS */
	OS_voidStart();

	while(1){
		/* Call dispatcher*/
		OS_voidDispatcher();
	}
}



