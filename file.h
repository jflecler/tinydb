/*Author : 	fname = Julien
			lname = Leclercq
			id[ULB] = 000479336
  Date : 	06/12/20 */
#ifndef _FILE_H
#define _FILE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "get_time.h" //for timestamp
#include "student.h" //for write student_to_str()

FILE* create_log_file(char* query);

void title_for_file(FILE* f, char* query, char* field, char* value,
	double time, int nbre_result);

void write_on_file(FILE* f, student_t **match_list, int match_size);

#endif
