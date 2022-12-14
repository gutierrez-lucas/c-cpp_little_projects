#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum suits{
    club, diamond, heart, spade
};
typedef struct list{
	enum suits suit;
	short pip;
	struct list* next;
    struct list* prev;
}list;

list* array_to_list(short new_pip[], short new_suit[], int size, list** tail);
list* add_card_to_front(short new_pip, short new_suit, list* h);
list* create_card_list(short new_pip, short new_suit);
list* get_list_element(list* h, int element);
int swap_element_in_list(list** h, int target_position);
int count_list(list *h);
void print_list_foward(list *head, char *title);
void print_list_backward(list *tail, char *title);
int remove_list_element(list **element);
int bubble_sort_list(list** head, list** tail);

list* get_list_element(list* h, int element){
	int size_of_list = count_list(h) - 1;
	int position=0;

	if(element > size_of_list){return NULL;}

	while(position++ < element){
		h = h -> next;
	}

	return(h);
}

int main(){
	list* head = NULL;
	list* tail = NULL;

	short pip_vector[52];
	short suit_vector[52];

	for(int i=0; i<=3; i++){
		for(int j=1; j<=13; j++){
			pip_vector[(14*i)+(j-1*(i+1))]  = j;
			suit_vector[(14*i)+(j-1*(i+1))] = i;
		}
	}

	head = array_to_list(pip_vector, suit_vector, 52, &tail);
	print_list_foward(head, "data in head: \n");
	print_list_backward(tail, "data in head: \n");

	list* auxiliar;
	int removed_elements = 0;
	printf("elements in list: %d\n", count_list(head));
	while(removed_elements<=24){
		auxiliar = get_list_element(head, rand()%51);
		if(auxiliar!=NULL){
			removed_elements++;
			remove_list_element(&auxiliar);
		}
	}
	printf("elements in list: %d\n", count_list(head));
	print_list_backward(tail, "data in head: \n");

	printf("\n");
	return(0);
}

int bubble_sort_list(list** head, list** tail){
// in order to modify the content of first element (without only swaping its data)
// we must pass as parameter the direction of the first element and not only a copy
// of its direction. thats why we have a pointer to pointer	
	list* first;
	list* last;
	int position = 0;
	int size_of_list = count_list(*head) - 1;
 
	int swapped; // did a swap happen ?
	do{
		first = (*head); 
// as head is a pointer to pointer it must be dereferenced to be used
		last = first -> next;
        last -> prev = (*head);
		position = 0;
		swapped = 0;
		while(position <= size_of_list){
			if((first -> pip) > (last -> pip)){
				swap_element_in_list(head, position);
				swapped = 1;
// if a swapped had happen position must be keep still to, or it will jump one position on 
// next swap				
			}else if((first -> pip) == (last -> pip)){
// FIND DUPLICATE AND REMOVE IT                
                if(position == 0){
// if the duplicate is relative to the first element, we must use the real location of the head to relocate it
                    remove_list_element(head);
                }else if(position == size_of_list){
// if it is in the last element, we must use the real direction of the tail to relocate it                    
                    remove_list_element(tail);
                }else{
                    remove_list_element(&first);
                }
                size_of_list--;
			}else{
				position++;
			}
			if(last -> next == NULL){break;}
// if we reach end of list then break, else it will try to access it on the next pass
// and will seg-fault as last wont be allocated			
				first = last;
				last = last -> next;
                last -> prev = first;
		}
	}while(swapped);
    *tail = last; // reasign tail,because its direction may be lost
// optimization for bubble sort: instead of force a double scan for every element O(N^2)	
// stop if in one pass there has been no swap operations
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
        (*h) -> prev = NULL;
        first -> prev = *h;
		first -> next = last;
	}else{
		while(current_position++ != target_position - 1){
			first = first -> next;
			middle = middle -> next;
			last = last -> next;
		}
		first  -> next = last;
		middle -> next = last -> next;
        middle -> prev = last;
		last -> next = middle;
        last -> prev = first;
	}
}

int remove_list_element(list **element){
    list* to_remove;

	if(to_remove==NULL) return 0;
    to_remove = *element;

    if(to_remove -> next == NULL){
        // remove the head        
        to_remove -> prev -> next = NULL;
        *element = to_remove -> prev;
    }else if(to_remove -> prev == NULL){
        // remove the tail
        to_remove -> next -> prev = NULL;
        *element = to_remove -> next;
    }else{
        to_remove -> next -> prev = to_remove -> prev;
        to_remove -> prev -> next = to_remove -> next;
    }
    free(to_remove); // free the removed element from heap

	return 1;
}

void print_list_backward(list *tail, char *title){
	int position = 0;
	printf("%s", title);
	while(tail != NULL){
		printf("%02d of", tail -> pip);
		switch (tail -> suit){
		case(club):
			printf(" clubs");
			break;
		case(heart):
			printf(" hearts");
			break;
		case(diamond):
			printf(" diamonds");
			break;
		case(spade):
			printf(" spades");
			break;		
		default:
			printf("unknown suit: %d", tail ->suit);
			break;
		}
		printf("\n");
		tail = tail -> prev;
		if(++position>12){
			position = 0;
			printf("\n");
		}
	}
	printf("\n");
}

void print_list_foward(list *head, char *title){
	int position = 0;
	printf("%s", title);
	while(head != NULL){
		printf("%02d of", head -> pip);
		switch (head -> suit){
		case(club):
			printf(" clubs");
			break;
		case(heart):
			printf(" hearts");
			break;
		case(diamond):
			printf(" diamonds");
			break;
		case(spade):
			printf(" spades");
			break;		
		default:
			printf("unknown suit: %d", head ->suit);
			break;
		}
		printf("\n");
		head = head -> next;
		if(++position>12){
			position = 0;
			printf("\n");
		}
	}
	printf("\n");
}

list* create_card_list(short new_pip, short new_suit){
	list* list_head;
	list_head = malloc(sizeof(list));
	list_head -> pip  = new_pip;
	list_head -> suit = new_suit;
	list_head -> next = NULL;
	list_head -> prev = NULL;
	return list_head;
}

list* add_card_to_front(short new_pip, short new_suit, list* h){
	list* head = create_card_list(new_pip, new_suit);
	head -> next = h;
    h -> prev = head;
	return head;
}

list* array_to_list(short new_pip[], short new_suit[], int size, list** tail){
	list* head = create_card_list(new_pip[0], new_suit[0]);
    *tail = head;
	for(int i=1; i < size; i++){
		head = add_card_to_front(new_pip[i], new_suit[i], head);
	}
	return(head);
}

int count_list(list *h){
	if(h == NULL) return 0;
	else return(1+count_list(h->next));
}

