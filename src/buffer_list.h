#include <stdlib.h>
#include <stdio.h>
#include "buffer_manager.h"


struct Page *seek_list(struct List * list, int user_id);
void insert_list(struct List * list, struct Page *x);
struct Page * remove_list(struct List * list, struct Page*x);
struct List * create_new_list();


struct Page *seek_list(struct List * list, int page_id) {
	struct Page * x = list->head;
	while(x != NULL){
		if(x->id == page_id){
			return x;
		}
		x = x->next;
	}
	return x;
}

void insert_list(struct List * list, struct Page *x) {
	if (x != NULL) {
		x->next = NULL;
		x->prev = NULL;
		if (list->head == NULL) {
			list->head = x;
			list->tail = x;
		} else {
			x->next = list->head;
			list->head->prev = x;
			list->head = x;
		}
		list->size++;
		x->list = list;

	}
}

struct Page * remove_list(struct List * list, struct Page*x) {
	if (x != NULL) {
		if (list->tail == list->head) {
			list->tail = list->head = NULL;
		} else if (x->next != NULL && x->prev != NULL) {
			x->prev->next = x->next;
			x->next->prev = x->prev;
		} else if (x == list->head) {
			if (list->head->next != NULL) {
				list->head = x->next;
				list->head->prev = NULL;
			} else {
				list->head = NULL;
			}
		} else if (x == list->tail) {
			list->tail = x->prev;
			list->tail->next = NULL;
		}
		list->size--;
		x->list = NULL;
		x->next = NULL;
		x->prev = NULL;
	}
	return x;
}

struct List * create_new_list() {
	struct List * list = (struct List*) malloc(sizeof(struct List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;
}
