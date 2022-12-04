#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef struct list{
	int data;
	struct list* next;
}list;

int is_empty(const list *l){return (l==NULL);}

void print_list(list *h, char *title){
	printf("%s", title);
	while(h != NULL){
		printf("%d :", h -> data);
		h = h -> next;
	}
		if( h == NULL){
		printf(": X");
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

void concat_list(list *h1, list *h2){
	if(h1 == NULL){
		h1 = h2;
	}else if(h1->next==NULL){
		h1 -> next = h2;
	}else{
		concat_list(h1->next, h2);
	}
}

void insert_in_list(list** h, list* to_insert, int target_position){
// need to user pointer to pointer in first parameter because we need the real direction of
// h and not a copy of it, so in case of need to modify the direction of h (when target_postion == 0)	
// it will remember the modification after leaving the function
	int current_position = 0;
	list* first;
	list* last;

	first = *h;
	if( target_position == 0){
		to_insert -> next = first;
		*h = to_insert;
		(*h) -> next = first;
	}else{
		while(current_position++ != target_position - 1){
			assert(first -> next != NULL);
			first = first -> next;		
		}
		last = first -> next;
		first -> next = to_insert; 
		to_insert -> next = last;
	}
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

int bubble_sort_list(list** h){
	list* first;
	list* last;
	int position = 0;
	int size_of_list = count_list(*h) - 1;
 
	// for(int i=0;i<size_of_list;i++){
	int swapped = 0;
	do{
		first = (*h);
		last = first -> next;
		position = 0;
		swapped = 0;
		while(position <= size_of_list){
			if((first -> data) > (last -> data)){
				swap_element_in_list(h, position);
				swapped=1;
			}else{
				position++;
			}
			if(last -> next == NULL){break;}
				first = last;
				last = last -> next;
		}
	}while(swapped);
}

int main(){
	list* head = NULL;
	
	int data[100];
	int comienzo = 1, final = 29;

	for(int i=0; i<sizeof(data)/sizeof(int);i++){
		data[i] = rand()%100;
	}

	head = array_to_list(data, sizeof(data)/sizeof(int));
	print_list(head, "data in head: ");
	bubble_sort_list(&head);
	print_list(head, "data sorted: ");
	printf("\n");
	return(0);
}
