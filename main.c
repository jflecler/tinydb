/*Author : 	fname = Julien
			lname = Leclercq
			id[ULB] = 000479336
  Date : 	06/12/20 */
#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "db.h"
#include "parsing.h"
#include "student.h"
#include "get_time.h"
#include "file.h"

void menu(database_t *db, char* query) {
    char* rest = query; //useless but for alloc enought memory
    char* command = strtok_r(query, " ", &rest);
    double start, end; //to measure the running time
    student_t **match_list = NULL;
    int match_size = 0;

    //Parse query
    int req = parse_command(command);
    //Do query
    start = time_for_exe(); //Set begin of chronometer
	  char field_search[10], value_search[64]; //field[9] because bigger field is 'birthdate'
    if (req == 1) { //select
    	//Parse argv
    	parse_selectors(rest, field_search, value_search);
    	//Search all peoples match with value's field
		  match_list = db_search(&match_size, db, field_search, value_search);
    } else if (req == 2) { //update
    	char field_to_update[9], update_value[64];
    	//Parse argv
    	parse_update(rest, field_search, value_search, field_to_update, update_value);
    	//Search all peoples match with value's field
		  match_list = db_search(&match_size, db, field_search, value_search);
    	//Update people(s)
      for(int i=0; i<match_size; i++) {
          update_student(match_list[i], field_to_update, update_value);
      }
    } else if (req == 3) { //delete
    	//Parse argv
    	parse_selectors(rest, field_search, value_search);
    	//Search all peoples match with value's field
		  match_list = db_search(&match_size, db, field_search, value_search);

      FILE* f = create_log_file(command); //Open log file for answer the query
      end = time_for_exe(); //Set end of chronometer
      title_for_file(f, query, field_search, value_search,
          end-start, match_size);
      write_on_file(f, match_list, match_size);

    	//Delete people(s) and copy it for print in log file
      db_delete(db, match_list, match_size);
    } else if (req == 4) { //insert
    	//Create new student
    	student_t s;
    	//Parse argv
      parse_insert(rest, s.fname, s.lname, &s.id, s.section, &s.birthdate);
      if ((signed) s.id < 0) {match_size = -2;}
      else {
          db_add(db, s);
          //Get new student in DB for verify it's good & use his id because it's unique
          strcpy(field_search, "id");
          sprintf(value_search, "%d", s.id);
      	  match_list = db_search(&match_size, db, field_search, value_search);
      }
    } else { //error
    	printf("Error for understand request. Please retry.\n");
    }
    if (match_size == -1) {
      printf("Error for understand field filter. Please retry.\n");
    } else if (match_size == -2) {
      printf("Error, id cannot be negative. Please retry.\n");
    } else if (req != 3) { //End of request -> same for all
      FILE* f = create_log_file(command); //Open log file for answer the query
      end = time_for_exe(); //Set end of chronometer
      title_for_file(f, query, field_search, value_search,
          end-start, match_size);
      write_on_file(f, match_list, match_size); //write all reply value on file & close it
    }
    free(match_list);
}

int main(int argc, char* argv[]) {
	printf("\nWelcome to the Tiny Database!\n");

	//Get DataBase Binary File
	FILE* f;
	char nom_fichier[64];
	if (argc<1) {
		printf("Loading the database with default path...\n");
		strcpy(nom_fichier, "students/students.bin");}
	else {
		printf("Loading the database provided...\n");
		strcpy(nom_fichier, argv[1]);
  }

	f = fopen(nom_fichier, "rb");
	//Init & load database informations
	database_t db;
	bool res = db_init(&db, f);
	fclose(f);
	if (!res) {
		perror("Error for allocation memory. Program aborted.\n");
		exit(0);
  } else {
		db_load(&db, nom_fichier);
		printf("Done!\n");
  }

	//Infinity loop for entry all querys. Stop it with Ctrl+C
  char requested[256] = "";
	while (!feof(stdin)) {
		printf("\nPlease enter your requests.\n");
		printf("> ");
		fgets(requested, sizeof(requested), stdin);
    menu(&db, requested);
    db_save(&db, nom_fichier);
    printf("Done!");
	}
  free(db.data);
	return 0;
}
