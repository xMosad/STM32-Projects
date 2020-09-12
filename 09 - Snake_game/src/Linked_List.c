
/* Library */
#include "stdlib.h"
#include "STD_TYPES.h"

/* Module includes */
#include "Linked_List.h"


 Snake_point_t* head = NULL ;
 Snake_point_t* tail = NULL ;

void appendNode(u8 copy_u8X , u8 copy_u8Y  )
{
    Snake_point_t *node = (Snake_point_t*) malloc(sizeof(Snake_point_t));
    node->next = NULL ;
    node->x_position = copy_u8X;
    node->y_position = copy_u8Y;


    if (head == NULL){
        head = node ;
        tail = node ;
    }
    else {
        tail->next=node;
        tail=node;
    }

}

u8 updateNodes(u8 copy_u8X , u8 copy_u8Y )
{
	Snake_point_t *node = '\0' ;
	u8 local_u8Flag = 0;
	for(node= (head->next)  ; node!= NULL; node=node->next){
		if (node->next == NULL){
			node->x_position = copy_u8X ;
			node->y_position = copy_u8Y ;
		}
		else {
			Snake_point_t *next_node = node->next ;
			node->x_position = next_node->x_position ;
			node->y_position = next_node->y_position ;
		}
	}
	if ((tail->x_position == head->x_position ) && (tail->y_position == head->y_position ) ){
		local_u8Flag = 1;
		u8 x = 8;
		u8 y = 8 ;
		while (x > 7 ){
		x = rand() / 500 ;
		}
		while (y > 7 ){
			y = rand() / 500 ;
		}
		head->x_position = x;
		head->y_position = y;
	}
	return local_u8Flag ;
}

void getFrame (u8 *copy_u8arr)
{
	for (u8 i = 0 ; i < 8 ; i++){
		copy_u8arr[i] = 0 ;
	}
	Snake_point_t *node = '\0' ;
	for(node=head; node!= NULL; node=node->next){
		copy_u8arr[node->y_position] += (1 << node->x_position);
	}
}

u8 checkForCollison(void){
	Snake_point_t *node = '\0' ;
	u8 local_u8Flag = 0 ;
	for(node= (head->next); node!= tail ; node=node->next){
		if ((tail->x_position == node->x_position ) && (tail->y_position == node->y_position )){
			local_u8Flag = 1 ;
		}
	}
	return local_u8Flag ;
}

void restorInitSnake(){
	Snake_point_t *node = '\0' ;
	Snake_point_t *temp = '\0' ;
	for(node=head; node!= NULL ; node=node->next){
		temp = node ;
		free(temp);
	}
	head = NULL;
	appendNode (5 , 5);
	appendNode (0 , 0);
}







