/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 9/10/2020                          */
/*****************************************************/

		
#ifndef FPEC_INTERFACE_H
#define FPEC_INTERFACE_H

#define    WRITE_DONE    1
#define    WRITE_ERROR   0


/* 
  * FPEC_u8WriteHalfWord - > write half word at specific location
  * I/P : (u32) the address , (u16) data 
  * O/P : u8 (Error state)
*/
u8 FPEC_u8WriteHalfWord( u32 copy_u32Address, u16 copy_u16Data);

/* 
  * FPEC_u8WriteHalfWord - > write half word at specific location
  * I/P : (u32) the address , (u16) data , (u16) Length of data
*/
void FPEC_voidWriteArea (u32 copy_u32Address , u16 *copy_u16Data , u16 copy_u16Length);

/*
  * FPEC_voidErasePage - > Erase a page 
  * I/P : (u32) the address  
*/
void FPEC_voidErasePage( u8 copy_u8PageNumber );

/* 
  * FPEC_voidErasePage - > Erase a the whole user pages  
*/
void FPEC_voidMassErase(void);





#endif
