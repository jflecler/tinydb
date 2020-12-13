#ifndef _GET_TIME_H
#define _GET_TIME_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* Required to work with strptime, which is OS-dependent */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

double time_for_exe();

void get_timestamp(char* tmstp);

#endif