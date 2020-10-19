/*

 * Parse.c
 *
 *  Created on: Oct 19, 2020
 *      Author: Mosad
 */

/* Library includes */
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/* MCAL */
#include "FPEC_interface.h"

u32 global_u32Address = 0x08000000 ;
u8 AsciToHex(u8 Copy_u8Asci)
{
	u8 Result;
	if ( (Copy_u8Asci >= 48) && (Copy_u8Asci <= 57) )
	{
		Result = Copy_u8Asci - 48;
	}

	else
	{
		Result = Copy_u8Asci - 55;
	}

	return Result;
}

void ParseData(u8* Copy_u8BufData)
{
	u8 Digit0 , Digit1 , Digit2 , Digit3 ;

	/* Get character count */
	u8 local_u8HighDigit = AsciToHex(Copy_u8BufData[1]);
	u8 local_u8LowDigit = AsciToHex(Copy_u8BufData[2]);
	u8 local_u8CharCount = local_u8LowDigit | (local_u8HighDigit <<4) ;

	/* Get Address */
	u32 local_u32Address ;
	Digit0 = AsciToHex(Copy_u8BufData[6]);
	Digit1 = AsciToHex(Copy_u8BufData[5]);
	Digit2 = AsciToHex(Copy_u8BufData[4]);
	Digit3 = AsciToHex(Copy_u8BufData[3]);
	local_u32Address = Digit0 | (Digit1 << 4) | (Digit2 << 8) | (Digit3 << 12);
	local_u32Address = local_u32Address | global_u32Address ;

	/* Get Data */
	u16 local_u16DataBuf[20] ;
	for (u8 i = 0 ; i < (local_u8CharCount/2) ; i++){
		Digit0 = AsciToHex(Copy_u8BufData[9+ i*4]);
		Digit1 = AsciToHex(Copy_u8BufData[10+ i*4]);
		Digit2 = AsciToHex(Copy_u8BufData[11+ i*4]);
		Digit3 = AsciToHex(Copy_u8BufData[12+ i*4]);
		local_u16DataBuf[i] = Digit1 | (Digit0 << 4) | (Digit2 << 12) | (Digit3 << 8);
	}

	/* Write data to flash */
	FPEC_voidWriteArea(local_u32Address , local_u16DataBuf ,(local_u8CharCount/2) );
}

void ParseUpperAddress(u8 *Copy_u8BufData){
	u8 Digit0 , Digit1 , Digit2 , Digit3 ;

	/* Get Address */
	u32 local_u32Address ;
	Digit0 = AsciToHex(Copy_u8BufData[12]);
	Digit1 = AsciToHex(Copy_u8BufData[11]);
	Digit2 = AsciToHex(Copy_u8BufData[10]);
	Digit3 = AsciToHex(Copy_u8BufData[9]);
	local_u32Address = Digit0 | (Digit1 << 4) | (Digit2 << 8) | (Digit3 << 12);
	global_u32Address = (local_u32Address << 16);
}

void Parser_voidParseRecord(u8* Copy_u8BufData)
{
	switch (Copy_u8BufData[8])
	{
	case '0': ParseData(Copy_u8BufData); break;
	case '4': ParseUpperAddress(Copy_u8BufData); break;
	}
}

