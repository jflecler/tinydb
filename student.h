/*Author : 	fname = Julien
			lname = Leclercq
			id[ULB] = 000479336
  Date : 	06/12/20 */
#ifndef _STUDENT_H
#define _STUDENT_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parsing.h"

/**
 * Student structure type.
 **/
typedef struct {
    unsigned id;         /** Unique ID **/
    char fname[64];      /** Firstname **/
    char lname[64];      /** Lastname **/
    char section[64];    /** Section **/
    struct tm birthdate; /** Birth date **/
} student_t;

void update_student(student_t *s, char* field, char* value);

/**
 * Convert a student to a human-readlable string.
 **/
void student_to_str(char* buffer, student_t* s);

/**
 * Return whether two students are equal or not.
 * Two students are equal when all their fields are equal.
 **/
int student_equals(student_t* s1, student_t* s2);

#endif
