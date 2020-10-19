/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 9/10/2020                          */
/*****************************************************/
/* Library includes */
#include "BIT_MATH.h"
#include "STD_TYPES.h"

/* Module includes */
#include "FPEC_interface.h"
#include "FPEC_config.h"
#include "FPEC_private.h"


u8 FPEC_u8WriteHalfWord( u32 copy_u32Address, u16 copy_u16Data)
{
	u8 local_u8Error ; 

	/* Wait if read or write operation is under execution */
	while (GIT_BIT ((FPEC -> FLASH_SR ), BSY ));

	/* Check on the lock bit */
	if (GIT_BIT ((FPEC -> FLASH_CR ), LOCK )){
		/* if FPEC locked perform a lock sequence */
		FPEC -> FLASH_KEYR = FPEC_KEY1 ;
		FPEC -> FLASH_KEYR = FPEC_KEY2 ;
	}
	
	/* Enable program */
	SET_BIT ((FPEC -> FLASH_CR ), PG );
	
	/* Write byte at the desired place */
	 (*(u16 *)copy_u32Address) = copy_u16Data ;
	
	/* Wait till the wirte is done */
	while (GIT_BIT ((FPEC -> FLASH_SR ), BSY ));
	
	/* Disable PG */
	CLEAR_BIT ((FPEC -> FLASH_CR ), PG );

	/* Check on Data */
	if ((*(u16 *)copy_u32Address) == copy_u16Data){
		local_u8Error = WRITE_DONE ;
	}
	else {
		local_u8Error = WRITE_ERROR ;
	}
	
	return local_u8Error ;
}

void FPEC_voidWriteArea (u32 copy_u32Address , u16 *copy_u16Data , u16 copy_u16Length)
{
	/* Wait if read or write operation is under execution */
	while (GIT_BIT ((FPEC -> FLASH_SR ), BSY ));

	/* Check on the lock bit */
	if (GIT_BIT ((FPEC -> FLASH_CR ), LOCK )){
		/* if FPEC locked perform a lock sequence */
		FPEC -> FLASH_KEYR = FPEC_KEY1 ;
		FPEC -> FLASH_KEYR = FPEC_KEY2 ;
	}

	for (u8 i = 0 ; i <copy_u16Length ; i++ ){
		/* Enable program */
		SET_BIT ((FPEC -> FLASH_CR ), PG );

		/* Write byte at the desired place */
		 (*(u16 *)copy_u32Address) = copy_u16Data[i] ;

		 /* Shift to the next address */
		 copy_u32Address += 2;

		/* Wait till the wirte is done */
		while (GIT_BIT ((FPEC -> FLASH_SR ), BSY ));
	}

	/* Disable PG */
	CLEAR_BIT ((FPEC -> FLASH_CR ), PG );

}

void FPEC_voidErasePage( u8 copy_u8PageNumber )
{ 
	/* Check on the lock bit */
	if (GIT_BIT ((FPEC -> FLASH_CR ), LOCK )){
		/* if FPEC locked perform a lock sequence */
		FPEC -> FLASH_KEYR = FPEC_KEY1 ;
		FPEC -> FLASH_KEYR = FPEC_KEY2 ;
	}
	
	/* Enable erase for page */
	SET_BIT ((FPEC -> FLASH_CR ), PER );
	
	/* Set Address for page */
	FPEC -> FLASH_AR = FPEC_FLASH_START_ADDRESS + (u32) (copy_u8PageNumber * 1024) ;
	
	/* Start erase */
	SET_BIT ((FPEC -> FLASH_CR ), STRT );

	/* Wait till the erase is done */
	while (GIT_BIT ((FPEC -> FLASH_SR ), BSY ));

	/* Disable erase for page */
	CLEAR_BIT ((FPEC -> FLASH_CR ), PER );
	
}

void FPEC_voidMassErase(void)
{
	/* Check on the lock bit */
	if (GIT_BIT ((FPEC -> FLASH_CR ), LOCK )){
		/* if FPEC locked perform a lock sequence */
		FPEC -> FLASH_KEYR = FPEC_KEY1 ;
		FPEC -> FLASH_KEYR = FPEC_KEY2 ;
	}
	
	/* Enable erase for page */
	SET_BIT ((FPEC -> FLASH_CR ), MER );
	
	/* Start erase */
	SET_BIT ((FPEC -> FLASH_CR ), STRT );

	/* Wait till the erase is done */
	while (GIT_BIT ((FPEC -> FLASH_SR ), BSY ));

}




