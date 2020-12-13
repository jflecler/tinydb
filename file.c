#include "file.h"

FILE* create_log_file(char* query) {
	char filename[26];
	char tmstp[21];

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

void title_for_file(char* buffer, char* query, char* field, char* value, double time, int nbre_result) {
	sprintf(buffer, "Query \"%s %s=%s\" completed in %fms with %d results.\n", 
		query, field, value, time, nbre_result);
}

void write_on_file(FILE* f, student_t *match_list) {
	unsigned i = 0;
	char buffer[64];
	for(i=1; i<match_list[0].id; i++) {
        	student_to_str(buffer, &match_list[i]);
        	fputs(buffer, f);
        }
    fclose(f);
}