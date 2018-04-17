#include <stdlib.h>
#include <stdio.h>

#define BLOCK_SIZE 1024
#define BIN_FILE_PATH "file.bin"

int CAPACITY = 0;
int OPERATION_COUNTER = 0;
int READ_COUNTER = 0;
int WRITE_COUNTER = 0;
int MISS_COUNTER = 0;
int HIT_COUNTER = 0;
int SECONDARY_MEDIA_COUNTER = 0;

struct Page {
	int isDirty;
	struct Page* next;
	struct Page* prev;
	int id;
	char* content;
	struct List* list;
};

struct List{
	struct Page* head;
	struct Page* tail;
	int size;
};

//PAGE FUNCTIONS
struct Page * create_new_page(int id);
void set_operation(struct Page *x, char operation);
struct Page* flush(struct Page*x);
void print_buffer_result();

//FILE FUNCTIONS
void open_database_file();
void read_file(struct Page* x);
void write_file(struct Page* x);



struct Page * create_new_page(int id) {
	struct Page * newPage = (struct Page*) malloc(sizeof(struct Page));
	newPage->id = id;
	newPage->isDirty = 0;
	newPage->next = NULL;
	newPage->prev = NULL;
	read_file(newPage);
	return newPage;
}

void set_operation(struct Page *x, char operation) {
	if (operation == 'W') {
		x->isDirty = 1;
		WRITE_COUNTER++;
	} else {
		READ_COUNTER++;
	}
}
struct Page* flush(struct Page*x) {
	if (x != NULL) {
		if (x->isDirty == 1) {
			write_file(x);
			SECONDARY_MEDIA_COUNTER++;
		}
		free(x->content);
		x->content = NULL;
	}
	return x;
}

void print_buffer_result() {

	printf("\nCapacity: %d", CAPACITY);
	printf("\nOperation Counter: %d", OPERATION_COUNTER);
	printf("\nSecondary Media Counter: %d", SECONDARY_MEDIA_COUNTER);
	float aux1 = 100 * ((float) MISS_COUNTER / (float) OPERATION_COUNTER);
	float aux2 = 100 * (float) HIT_COUNTER / (float) OPERATION_COUNTER;
	printf("\nHit:    \t %d, %.1f percent \nMiss:     \t %d, %.1f percent", HIT_COUNTER, aux2, MISS_COUNTER, aux1);
	aux1 = 100 * ((float) READ_COUNTER / (float) OPERATION_COUNTER);
	aux2 = 100 * ((float) WRITE_COUNTER / (float) OPERATION_COUNTER);
	printf("\nREAD:\t %d, %.1f percent \nWRITE:\t %d, %.1f percent", READ_COUNTER, aux1, WRITE_COUNTER, aux2);
	printf("\n ");
}


//FILE FUNCTIONS

FILE *file;
void open_database_file(){
    file = fopen(BIN_FILE_PATH, "r+");
}

void read_file(struct Page* x){
	//x->content = (char*)malloc(BYTES);
	char * xs = (char*)malloc(BLOCK_SIZE);
	fseek(file, (x->id/100000)*BLOCK_SIZE, SEEK_SET);
	fread(xs, sizeof(char),BLOCK_SIZE ,file);
	x->content = xs;
	//printf("Page ID = %d HASH = %d LEITURA == %s \n",  pageID, (pageID/100000)*BYTES, x);
}

void write_file(struct Page* x){
	fseek(file, (x->id/100000)*BLOCK_SIZE, SEEK_SET);
	fwrite(x->content, sizeof(char), BLOCK_SIZE ,file);
	fflush(file);
}
