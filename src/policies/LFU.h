#include <stdlib.h>
#include <stdio.h>
#include "policy.h"

struct List * lfu;

int insert_new_page(int id, char operation) {

	OPERATION_COUNTER++;
	struct Page * x = seek_list(lfu,id);


	if (x != NULL) { //achou no buffer

		HIT_COUNTER++;
        set_operation(x, operation);
		insert_list(lfu,remove_list(lfu,x));
		x.frequency += 1;
		return 1;

	} else {

		MISS_COUNTER++;
		x = create_new_page(id);
		set_operation(x, operation);

		if(lfu->size == CAPACITY){
			
			struct Page* removed = remove_list(lfu,search_lower_frequency(lfu));
			free(flush(removed));
			removed = NULL;
		}
		insert_list(lfu,x);
	}

	return 0;
}

//Imprimir pela ordem de frequencia
void print_policy(){

    printf("MRU->");
	struct Page * x = lfu->head;
	while(x != NULL){
		printf("[%d]",x->id);
		x = x->next;
	}
	printf(" <-lfu\n");

}



void initialize(int capacity) {
	open_database_file();
	CAPACITY = capacity;
	lfu = create_new_list();
}


