/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 20/9/2020                          */
/*****************************************************/
#ifndef UART_CONFIG_H
#define UART_CONFIG_H


/* Max number of UART channels */
#define      UART_MAX_CH          3 

#define      APB1_CLK             8000000
#define      APB2_CLK             8000000

/********* Configuration data ********/
typedef enum {
	UART_ENABLE ,
	UART_DISABLE
}State_t;

typedef enum {
	DATA_8BIT ,
	DATA_9BIT
}WordLeng_t;

typedef enum {
	EVEN_PARITY ,
	ODD_PARITY ,
	NO_PARITY
}ParityMode_t;

typedef enum {
	ENABLE_RECEIVER ,
	ENABLE_TRANSMITTER,
	ENABLE_BOTH
}RTControl_t;

/*  Configuration table data type */
typedef struct {
	State_t moduleEnable ;
	WordLeng_t wordLenght;
	ParityMode_t parityMode ;
	RTControl_t RTControl ;
	u32 u32BusFreq ;
	u32 u32Baudrate ;
}UART_config_t;

UART_config_t UART_configTable [UART_MAX_CH] = {
   // Module 	Date          parity         Rec / trans     Bus       baudrate
   // enable	bits          mode           enable          clk
	{UART_ENABLE   , DATA_8BIT   , NO_PARITY    , ENABLE_BOTH   , APB2_CLK , 9600},
	{UART_DISABLE  , DATA_8BIT   , NO_PARITY    , ENABLE_BOTH   , APB1_CLK , 9600},
	{UART_DISABLE  , DATA_8BIT   , NO_PARITY    , ENABLE_BOTH   , APB1_CLK , 9600}
};


#endif
