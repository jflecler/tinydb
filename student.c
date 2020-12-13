#include "student.h"

void update_student(student_t *s, char* field, char* value) {
	int answer = parse_field(field);
	if (answer == 1) {strcpy(s->fname, value);}
	else if (answer == 2) {strcpy(s->lname, value);}
	else if (answer == 3) {s->id = atoi(value);}
	else if (answer == 4) {strcpy(s->section, value);}
	else if (answer == 5) {
		value[16] = (int) s->birthdate.tm_mday;
	    value[20] = (int) s->birthdate.tm_mon;
	    value[24] = (int) s->birthdate.tm_year;
	}
}

void student_to_str(char* buffer, student_t* s) {
    int day = s->birthdate.tm_mday;
    int month = s->birthdate.tm_mon;
    int year = s->birthdate.tm_year;
    sprintf(buffer, "%d: %s %s in section %s, born on the %d/%d/%d\n", s->id, s->fname, s->lname, s->section, day, month + 1, year + 1900);
}

int student_equals(student_t* s1, student_t* s2) {
    return s1->id == s2->id && strcmp(s1->fname, s2->fname) == 0 && strcmp(s1->lname, s2->lname) == 0 && strcmp(s1->section, s2->section) == 0 && s1->birthdate.tm_mday == s2->birthdate.tm_mday && s1->birthdate.tm_mon == s2->birthdate.tm_mon && s1->birthdate.tm_year == s2->birthdate.tm_year;
}