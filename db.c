#include "db.h"

bool cmp_value_and_db_arg(int field, student_t people, char* value) {
  if (((field == 1) && (strcmp(people.fname, value) == 0))
  || ((field == 2) && (strcmp(people.lname, value) == 0))
  || ((field == 3) && (people.id == atoi(value)))
  || ((field == 4) && (strcmp(people.section, value) == 0))) {
    return true;
  }

  if (field == 5) {
    student_t s;
    s.birthdate.tm_mday = (int) value[16];
    s.birthdate.tm_mon = (int) value[20];
    s.birthdate.tm_year = (int) value[24];
    struct tm s1 = s.birthdate;
    struct tm s2 = people.birthdate;
    return (s1.tm_mday == s2.tm_mday && s1.tm_mon == s2.tm_mon && s1.tm_year == s2.tm_year);
  }
  return false;
}

student_t* db_search(database_t *db, char* field, char* value) {
  int answer = parse_field(field);
  if (answer != 0) {
    student_t *ptr;
    size_t loop, size=0;
    //Calcul size for alloc memory
    for(loop=0; loop<=db->lsize; ++loop) {
      if (cmp_value_and_db_arg(answer, db->data[loop], value) == true) {
        ++size;}
    }
    ++size; //for save size
    ptr = malloc(sizeof(student_t) * size);
    student_t s;
    s.id = size; //stock size in a student object, in the id place.
    ptr[0] = s; //push this false student in first position.
    size=1; //reboot size + 1 size for false student
    //Get list of people(s)
    for(loop=0; loop<=db->lsize; ++loop) {
      if (cmp_value_and_db_arg(answer, db->data[loop], value) == true) {
        ptr[size] = db->data[loop];
        ++size;}
    }
    return ptr;
  }
  else {
    student_t s;
    s.id = -1; //impossible to set a student id minder 0
    student_t *ptr = &s;
    return ptr;
  }
}

void db_add(database_t *db, student_t s) {
  db->psize += sizeof(student_t);
  db->data = realloc(db->data, db->psize);
  db->data[db->lsize] = s;
  db->lsize += 1;
}

void db_delete(database_t *db, student_t *s) {
  for(int i=0; i<db->lsize; i++) {
    if (student_equals(&db->data[i], s) == 0) {
      db->data[i] = db->data[i+1];
      db->lsize -= 1;
      db->psize -= sizeof(student_t);
      db->data = realloc(db->data, db->psize);
    }
  }
}

void db_save(database_t *db, const char *path) {
  //get memory for open file and do
  strcpy(char *path, "tests/data/test.txt");
  FILE* f;
  f = malloc(sizeof(path));
  f = fopen(path, "w");
  //write db on file
  for(int i=0; i<db->lsize; i++) {
    fwrite(&db->data[i].fname, 1, sizeof(db->data[i].fname), f);
    fwrite(&db->data[i].lname, 1, sizeof(db->data[i].lname), f);
    fwrite(&db->data[i].id, 1, sizeof(db->data[i].id), f);
    fwrite(&db->data[i].section, 1, sizeof(db->data[i].section), f);
    fwrite(&db->data[i].birthdate.tm_mday, 1, sizeof(db->data[i].birthdate.tm_mday), f);
    fwrite(&db->data[i].birthdate.tm_mon, 1, sizeof(db->data[i].birthdate.tm_mon), f);
    fwrite(&db->data[i].birthdate.tm_year, 1, sizeof(db->data[i].birthdate.tm_year), f);
  }
  printf("%d %d\n", db->psize, db->lsize);
  //close file
  fclose(f);
}

void db_load(database_t *db, const char *path) {
  //get memory for open file and do
  FILE* f;
  f = malloc(sizeof(path));
  f = fopen(path, "rb");

  rewind(f); //start at the beginning of the file

  char buffer[64];
  unsigned long int counter;
  counter = 0;
  while (ftell(f) != db->psize){
    student_t s; //create a new struct student
    //complete information
    fread(&(s.id), sizeof(unsigned int), 1, f);
    fread(&(s.fname), sizeof(char[64]), 1, f);
    fread(&(s.lname), sizeof(char[64]), 1, f);
    fread(&(s.section), sizeof(char[64]), 1, f);
    student_to_str(buffer, &s);
    printf("%s\n", buffer);
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
  size_t endFile, rowSize = sizeof(student_t);

  //Find End Of File + Size of File & List
  fseek(f, 0L, SEEK_END); //go to end of the file
  endFile = ftell(f); //get position on file = size of file
  rewind(f);

  db->lsize = endFile/rowSize;
  db->psize = endFile;
  db->data = malloc(db->psize);
  return true;
}
