#include <stdlib.h>
#include <stdio.h>

#define DEBUG_MODE 1
#define NUMBER_OF_PAGES 40 //The page IDs will vary from 0 to NUMBER_OF_PAGES
#define TOTAL_OPERATIONS 100 //Number of operations that will be generated (it must be an even number)
#define TOTAL_WRITE_OPERATIONS_PERCENT 60
#define OPERATIONS_BURST_LENGHT 10
#define TYPE_BURST 'W'
#define OPERATIONS_BURST_START 35



int burst_state = 0;
int counter = 0;
int page_id_burst_mod;
float total_read_operations;
float total_write_operations;
float total_read_operationsRemaining;
float total_write_operations_remaining;
int read_count_remaining = 0;
int write_count_remaining = 0;
char op_type;
int pageId;
int i;


void showOperation(int* chosenPageId, char* chosenOpType, char opType, int pageId) {


	//pageId, opType
	//Here it is possible to capture the values of the page id's and type of the operation (pageId and opType)

	if(DEBUG_MODE == 1){

		printf("[%c(%i)][Remaining %iW-%iR]", opType, pageId,(int) write_count_remaining, (int) read_count_remaining);

		printf("\n");
	}
	if (read_count_remaining == 0 || write_count_remaining == 0) {
		burst_state = 0;
		counter = 0;
	}

	*chosenOpType = opType;
	*chosenPageId = pageId;
}


// Get Random Number
int random_in_range(unsigned int min, unsigned int max) {
	int base_random = rand(); /* in [0, RAND_MAX] */
	if (RAND_MAX == base_random)
		return random_in_range(min, max);
	/* now guaranteed to be in [0, RAND_MAX) */
	int range = max - min, remainder = RAND_MAX % range, bucket = RAND_MAX
			/ range;
	/* There are range buckets, plus one smaller interval
	 within remainder of RAND_MAX */
	if (base_random < RAND_MAX - remainder) {
		return min + base_random / bucket;
	} else {
		return random_in_range(min, max);
	}
}

unsigned int randr(unsigned int min, unsigned int max) {
	double scaled = (double) rand() / RAND_MAX;
	return (max - min + 1) * scaled + min;
}

int get_random_number(int min_range, int max_range, int cod_random_generator) {
	switch (cod_random_generator) {
	case 1:
		return randr(min_range, max_range);
		break;
	case 2:
		return random_in_range(min_range, max_range);
		break;
	}
	return 0;
}


char get_operation_type(int write_affinity) {
	int num;
	char opType;
	num = rand() % 100;
	if (num <= write_affinity) {
		opType = 'W';
	} else
		opType = 'R';

	return opType;
}

void init_remaining_operations() {
	total_write_operations = (int) (TOTAL_OPERATIONS * (TOTAL_WRITE_OPERATIONS_PERCENT / 100));
	total_read_operations = (int) (TOTAL_OPERATIONS - total_write_operations);

	read_count_remaining = total_read_operations;
	write_count_remaining = total_write_operations;
}



int generate_operations(int* chosen_page_id, char* chosen_op_type, char flag_burst) {

	total_write_operations = (int) (TOTAL_OPERATIONS * (TOTAL_WRITE_OPERATIONS_PERCENT / 100));
	total_read_operations = (int) (TOTAL_OPERATIONS - total_write_operations);

	if (burst_state == 1) {
		page_id_burst_mod++;
		pageId = page_id_burst_mod;
	} else {
		pageId = get_random_number(0, NUMBER_OF_PAGES, 2);
	}

	op_type = get_operation_type(TOTAL_WRITE_OPERATIONS_PERCENT);

	if (flag_burst == 'T' && counter != 0) {
		if (TYPE_BURST == 'W') {
			if (((write_count_remaining < OPERATIONS_BURST_LENGHT)
					&& burst_state == 0)) {

				// printf("BURST W - Not Allowed (write_count_remaining < OPERATIONS_BURST)\n");
				if (read_count_remaining == 0) {
					op_type = 'W';
				} else if (write_count_remaining == 0) {
					op_type = 'R';
				}
				if (op_type == 'R') {
					read_count_remaining--;
				} else if (op_type == 'W') {
					write_count_remaining--;
				}
				showOperation(chosen_page_id, chosen_op_type, op_type, pageId);
			} else {
				write_count_remaining--;
				showOperation(chosen_page_id, chosen_op_type, TYPE_BURST, pageId);
			}
		} else if (TYPE_BURST == 'R') {
			if (((read_count_remaining < OPERATIONS_BURST_LENGHT)
					&& burst_state == 0)) {

				// printf("BURST R -Not Allowed (reads_count_remaining < OPERATIONS_BURST)\n");
				if (read_count_remaining == 0) {
					op_type = 'W';
				} else if (write_count_remaining == 0) {
					op_type = 'R';
				}
				if (op_type == 'R') {
					read_count_remaining--;
				} else if (op_type == 'W') {
					write_count_remaining--;
				}
				showOperation(chosen_page_id, chosen_op_type, op_type, pageId);
			} else {
				read_count_remaining--;
				showOperation(chosen_page_id, chosen_op_type, TYPE_BURST, pageId);
			}
		}
	} else {
		if (read_count_remaining == 0) {
			op_type = 'W';
		} else if (write_count_remaining == 0) {
			op_type = 'R';
		}

		if (op_type == 'R') {
			read_count_remaining--;
		} else if (op_type == 'W') {
			write_count_remaining--;
		}
		showOperation(chosen_page_id, chosen_op_type, op_type, pageId);
	}

	return EXIT_SUCCESS;

}


char burst_test(float i, float operations_burst_start) {

	float valor = i / operations_burst_start;
	float partefracionaria = valor - (int) valor;

	if (((int) i % (int) operations_burst_start == 0 && partefracionaria == 0)
			&& i != 0) {
		if (burst_state != 1) {
			counter = -1;
		}
		return 'T';
	} else
		return 'F';
}




void execute() {

	int i;
	char* randOP = malloc(sizeof(char));
	int* randGID = malloc(sizeof(int));

	srand( (int)(time(NULL)));

	init_remaining_operations();

	for (i = 1; i <= TOTAL_OPERATIONS; i++) {
		if(DEBUG_MODE == 1){
		printf("[OP %i]\t[BURST: %i]\t", i, burst_state);

		}

		char resultTest = burst_test(i, OPERATIONS_BURST_START);
		if ((resultTest == 'T' && counter != 0) || counter != 0) {

			if (resultTest == 'T' && counter == -1) {

				if ((TYPE_BURST == 'R' && read_count_remaining == 0)
						|| (TYPE_BURST == 'R' && write_count_remaining == 0)) {
					counter = 0;
				} else if ((TYPE_BURST == 'W' && read_count_remaining == 0)
						|| (TYPE_BURST == 'W' && write_count_remaining == 0)) {
					counter = 0;
				} else {
					counter = OPERATIONS_BURST_LENGHT;
					burst_state = 1;
					page_id_burst_mod = get_random_number(0, NUMBER_OF_PAGES, 2);
				}
			}

			generate_operations(randGID, randOP, 'T');

			if (burst_state == 1) {
				counter--;
			}

			if (burst_state == 1 && counter == 0) {
				burst_state = 0;
				page_id_burst_mod = -1;
			}
		} else {
			generate_operations(randGID, randOP, 'F');
		}
	}


}
