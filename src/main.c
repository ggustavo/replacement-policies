#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "policies/LRU.h"

void run_test(char *file_name, int capacity) {

	FILE * file = fopen(file_name, "r");
	if (file == NULL) {
		printf("Workload not found");
	}
	char operation;
	int pageID;

	initialize(capacity);

	while (fscanf(file, "%c;%d\n", &operation, &pageID) > 0) {
		insert_new_page(pageID, operation);
		print_policy();

	}
	print_buffer_result();
	fclose(file);
}

int main(int argc, char *argv[]) {
	struct timeval tv1, tv2;

	gettimeofday(&tv1, NULL);
	run_test("test-operations.txt", 6);
	gettimeofday(&tv2, NULL);

	printf("\n\nTotal time = %f seconds\n",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
	// getchar();
	return 0;
}

