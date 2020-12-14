/*Author :  fname = Julien
      lname = Leclercq
      id[ULB] = 000479336
  Date :  06/12/20 */
#include "db.h"

bool cmp_value_and_db_arg(int field, student_t s, char* value) {
  if (((field == 1) && (strcmp(s.fname, value) == 0))
  || ((field == 2) && (strcmp(s.lname, value) == 0))
  || ((field == 3) && (s.id == atoi(value)))
  || ((field == 4) && (strcmp(s.section, value) == 0))) {
    return true;
  }

  if (field == 5) {
    int day, month, year;
    sscanf(value, "%d/%d/%d", &day, &month, &year);
    struct tm b = s.birthdate;
    if (year>1000) {
      if (year>2000) {
        year -= 2000;
      } else {
        year -= 1900;
      }
    }
    return (b.tm_mday == day && (b.tm_mon+1) == month && b.tm_year == year);
  }
  return false;
}

student_t** db_search(int *match_size, database_t *db, char* field, char* value) {
  int answer = parse_field(field);
  student_t **ptr;
  if (answer != 0) {
    size_t loop, size=0;
    //Calcul size for alloc memory
    for(loop=0; loop<db->lsize; ++loop) {
      if (cmp_value_and_db_arg(answer, db->data[loop], value)) {
        ++size;}
    }
    ptr = malloc(sizeof(student_t*) * size);
    *match_size = size;
    size=0; //reboot size + 1 size for false student
    //Get list of people(s)
    for(loop=0; loop<db->lsize; ++loop) {
      if (cmp_value_and_db_arg(answer, db->data[loop], value)) {
        ptr[size] = &db->data[loop];
        ++size;
      }
    }
  } else {
    *match_size = -1;
  }
  return ptr;
}

void db_add(database_t *db, student_t s) {
  db->psize += sizeof(student_t);
  db->data = realloc(db->data, db->psize);
  db->data[db->lsize] = s;
  db->lsize += 1;
}

void db_delete(database_t *db, student_t **match_list, int match_size) {
  int found = 0;
  for(int i=0; i<db->lsize; i++) {
    if (found>0) {
      db->data[i-found] = db->data[i];
    }
    if ((found<match_size) && student_equals(&db->data[i], match_list[found])) {
      found++;
    }
  }
  db->lsize -= match_size;
  db->psize -= sizeof(student_t)*match_size;
}

void db_save(database_t *db, const char *path) {
  //get memory for open file and do
  FILE* f;
  f = fopen(path, "wb");
  //write db on file
  fwrite(db->data, db->psize, 1, f);
  //close file
  fclose(f);
}

void db_load(database_t *db, const char *path) {
  //get memory for open file and do
  FILE* f;
  f = fopen(path, "rb");

  rewind(f); //start at the beginning of the file

  char buffer[1000];
  unsigned long int counter;
  counter = 0;
  while (ftell(f) != db->psize){
    student_t s; //create a new struct student
    //complete information
    fread(&(s.id), sizeof(unsigned int), 1, f);
    fread(&(s.fname), sizeof(char[64]), 1, f);
    fread(&(s.lname), sizeof(char[64]), 1, f);
    fread(&(s.section), sizeof(char[64]), 1, f);
    // fread(&(s.birthdate), sizeof(char[60]), 1, f);

    //get birthday on a buffer for a better assignment after
    fread(buffer, 60, 1, f);
    s.birthdate.tm_mday = (int) buffer[16];
    s.birthdate.tm_mon = (int) buffer[20];
    s.birthdate.tm_year = (int) buffer[24];

    db->data[counter] = s;
    ++counter;
  }
  fclose(f);
}

bool db_init(database_t *db, FILE* f) {
  int endFile, rowSize = sizeof(student_t);

  //Find End Of File + Size of File & List
  fseek(f, 0L, SEEK_END); //go to end of the file
  endFile = ftell(f); //get position on file = size of file
  rewind(f);

  db->lsize = endFile/rowSize;
  db->psize = endFile;
  db->data = malloc(db->psize);
  return true;
}
