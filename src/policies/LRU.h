#include <stdlib.h>
#include <stdio.h>
#include "policy.h"

struct List * lru;

int insert_new_page(int id, char operation) {

	OPERATION_COUNTER++;
	struct Page * x = seek_list(lru,id);


	if (x != NULL) {

		HIT_COUNTER++;
        set_operation(x, operation);
		insert_list(lru,remove_list(lru,x));
		return 1;

	} else {

		MISS_COUNTER++;
		x = create_new_page(id);
		set_operation(x, operation);

		if(lru->size == CAPACITY){
			struct Page* removed = remove_list(lru,lru->tail);
			free(flush(removed));
			removed = NULL;
		}
		insert_list(lru,x);
	}

	return 0;
}

void print_policy(){

    printf("MRU->");
	struct Page * x = lru->head;
	while(x != NULL){
		printf("[%d]",x->id);
		x = x->next;
	}
	printf(" <-LRU\n");

}



void initialize(int capacity) {
	open_database_file();
	CAPACITY = capacity;
	lru = create_new_list();
}


