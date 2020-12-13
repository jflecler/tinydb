#include "get_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

double time_for_exe() {
	clock_t time = clock();
	return (double) time / CLOCKS_PER_SEC;
}

void get_timestamp(char* tmstp) {
	sprintf(tmstp, "%ld", time(NULL));
}