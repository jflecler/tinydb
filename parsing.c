#include "parsing.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "student.h"

int parse_command(char* command) {
    char buffer[7];
    strcpy(buffer, "select");
    if (strcmp(command, buffer) == 0) {
        return 1;
    }
    strcpy(buffer, "update");
    if (strcmp(command, buffer) == 0) {
        return 2;
    }
    strcpy(buffer, "delete");
    if (strcmp(command, buffer) == 0) {
        return 3;
    }
    strcpy(buffer, "insert");
    if (strcmp(command, buffer) == 0) {
        return 4;
    }
    return 0;
}

int parse_field(char* field) {
    char buffer[10];
    strcpy(buffer, "fname");
    if (strcmp(field, buffer) == 0) {
        return 1;
    }
    strcpy(buffer, "lname");
    if (strcmp(field, buffer) == 0) {
        return 2;
    }
    strcpy(buffer, "id");
    if (strcmp(field, buffer) == 0) {
        return 3;
    }
    strcpy(buffer, "section");
    if (strcmp(field, buffer) == 0) {
        return 4;
    }
    strcpy(buffer, "birthdate");
    if (strcmp(field, buffer) == 0) {
        return 5;
    }
    return 0;
}

bool parse_update(char* query, char* field_filter, char* value_filter, char* field_to_update, char* update_value) {
    char* key_val_filter = strtok_r(query, " ", &query);  // key=val filter
    if (key_val_filter == NULL) {
        return false;
    }
    if (strtok_r(query, " ", &query) == NULL) {  // discard the "set" keyword
        return false;
    }

    char* key_val_update = strtok_r(query, " ", &query);  // key=val updated value
    if (key_val_update == NULL) {
        return false;
    }

    if (parse_selectors(key_val_filter, field_filter, value_filter) == 0) {
        return false;
    }
    if (parse_selectors(key_val_update, field_to_update, update_value) == 0) {
        return false;
    }
    return true;
}

bool parse_insert(char* query, char* fname, char* lname, unsigned* id, char* section, struct tm* birthdate) {
    char* token = strtok_r(query, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(fname, token);
    token = strtok_r(query, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(lname, token);
    token = strtok_r(query, " ", &query);
    if (token == NULL) {
        return false;
    }
    *id = (unsigned)atol(token);
    token = strtok_r(query, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(section, token);
    token = strtok_r(query, " ", &query);
    if (token == NULL) {
        return false;
    }
    if (strptime(token, "%d/%m/%Y", birthdate) == NULL) {
        return false;
    }
    return true;
}

bool parse_selectors(char* query, char* field, char* value) {
    char* token = strtok_r(query, "=", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(field, token);
    token = strtok_r(query, "\n", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(value, token);
    return true;
}