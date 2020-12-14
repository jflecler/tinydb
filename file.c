/*Author : 	fname = Julien
			lname = Leclercq
			id[ULB] = 000479336
  Date : 	06/12/20 */
#include "file.h"

FILE* create_log_file(char* query) {
	char filename[30];
	char tmstp[25];
	get_timestamp(tmstp);

	strcpy(filename, "logs/");
	strcat(tmstp, "-");
	strcat(tmstp, query);
	strcat(tmstp, ".txt");
	strcat(filename, tmstp);
	FILE* f = fopen(filename, "w");
	printf("%s\n", filename);
	return f;
}

void title_for_file(FILE* f, char* query, char* field, char* value, double time, int nbre_result) {
	fprintf(f, "Query \"%s %s=%s\" completed in %fms with %d results.\n",
		query, field, value, time, nbre_result);
}

void write_on_file(FILE* f, student_t **match_list, int match_size) {
	unsigned i = 0;
	char buffer[256];
	for(i=0; i<match_size; i++) {
    student_to_str(buffer, match_list[i]);
    fputs(buffer, f);
  }
  fclose(f);
}
