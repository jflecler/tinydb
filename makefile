FLAGS=-pthread -std=c11 -Wall -Wpedantic #-Werror

run:
	make main && ./tinydb tests/data/test_db.bin

main: main.c parsing student db file get_time clean
	gcc -o tinydb main.c parsing.o student.o db.o get_time.o file.o ${FLAGS}

parsing: parsing.c parsing.h
	gcc -c parsing.c

student: student.c student.h
	gcc -c student.c

db: db.c db.h
	gcc -c db.c

get_time: get_time.c get_time.h
	gcc -c get_time.c

file: file.c file.h
	gcc -c file.c

tests: tests/run_tests.py
	./tests/run_tests.py

clean:
	touch logs/tmp
	rm logs/*
