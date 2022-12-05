#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct list{
	int data;
	struct list* next;
}list;

int swap_element_in_list(list** h, int target_position);
int count_list(list *h);
list* array_to_list(int d[], int size);
list* add_to_front(int data, list* h);
list* create_list(int new_data);
void print_list_foward(list *h, char *title);

int bubble_sort_list(list** h){
// in order to modify the content of first element (without only swaping its data)
// we must pass as parameter the direction of the first element and not only a copy
// of its direction. thats why we have a pointer to pointer	
	list* first;
	list* last;
	int position = 0;
	int size_of_list = count_list(*h) - 1;
 
	int swapped; // did a swap happen ?
	do{
		first = (*h); 
// as h is a pointer to pointer it must be dereferenced to be used
		last = first -> next;
		position = 0;
		swapped = 0;
		while(position <= size_of_list){
			if((first -> data) > (last -> data)){
				swap_element_in_list(h, position);
				swapped = 1;
// if a swapped had happen position must be keep still to, or it will jump one position on 
// next swap				
			}else{
				position++;
			}
			if(last -> next == NULL){break;}
// if we reach end of list then break, else it will try to access it on the next pass
// and will seg-fault as last wont be allocated			
				first = last;
				last = last -> next;
		}
	}while(swapped);
// optimization for bubble sort: instead of force a double scan for every element O(N^2)	
// stop if in one pass there has been no swap operations
}

int main(){
	list* head = NULL;
	
	int data[100];
	for(int i=0; i<sizeof(data)/sizeof(int);i++){
// fill data with 100 random numbers between 0 and 100
		data[i] = rand()%100;
	}

	head = array_to_list(data, sizeof(data)/sizeof(int));
	print_list_foward(head, "data in head: \n");
	bubble_sort_list(&head);
// must pass direction of pointer in order to be able to modify its direction
// this is only necessary if the first element must be swaped	
	print_list_foward(head, "data sorted: \n");
	printf("\n");
	return(0);
}

void print_list_foward(list *h, char *title){
	int position = 0;
	printf("%s", title);
	while(h != NULL){
		printf("%02d :", h -> data);
		h = h -> next;
		if(++position>4){
			position = 0;
			printf("\n");
		}
	}
	printf("\n");
}

list* create_list(int new_data){
	list* list_head;
	list_head = malloc(sizeof(list));
	list_head -> data = new_data;
	list_head -> next = NULL;
	return list_head;
}

list* add_to_front(int data, list* h){
	list* head = create_list(data);
	head -> next = h;
	return head;
}

list* array_to_list(int d[], int size){
	list* head = create_list(d[0]);
	for(int i=1; i< size; i++){
		head = add_to_front(d[i],head);
	}
	return(head);
}

int count_list(list *h){
	if(h == NULL) return 0;
	else return(1+count_list(h->next));
}

int swap_element_in_list(list** h, int target_position){
//
// this will swap the list index = target_postion with the next one
//
// need to user pointer to pointer in first parameter because we need the real direction of
// h and not a copy of it, so in case of need to modify the direction of h (when target_postion == 0)	
// it will remember the modification after leaving the function
	int current_position = 0;
	list* first;
	list* middle;
	list* last;
	int size_of_list = count_list(*h) - 1;

	assert(size_of_list > target_position);

	first = *h;
	middle = (*h) -> next;
	last = middle -> next;
	
	if( target_position == 0){		
		*h = middle;
		(*h) -> next = first;
		first -> next = last;
	}else{
		while(current_position++ != target_position -1){
			first = first -> next;
			middle = middle -> next;
			last = last -> next;
		}
		first  -> next = last;
		middle -> next = last -> next;
		last -> next = middle;
	}
}