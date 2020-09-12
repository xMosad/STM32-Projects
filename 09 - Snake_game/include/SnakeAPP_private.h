/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 12/9/2020                          */
/*****************************************************/
#ifndef SNAKEAPP_PRIVATE_H
#define SNAKEAPP_PRIVATE_H

typedef enum {
	TO_RIGHT ,
	TO_LEFT  ,
	TO_UP    ,
	TO_DOWN
}directionState_t;

/* Helper functions */
static void SnakeAPP_voidUpdatePosition(void);
static void SnakeAPP_voidAbortGame(void);
static void SnakeAPP_voidStartGameOver (void);

/* Tasks */
void SnakeAPP_voidUpdateScreen (void);
void SnakeAPP_voidCheckRemote(void);


#endif
