# Makefile to compile Umix Programming Assignment 1 (pa1) [updated: 1/4/18]

LIBDIR = $(UMIXPUBDIR)/lib

CC 	= cc 
FLAGS 	= -g -L$(LIBDIR) -lumix1

PA1 =	pa1a pa1b pa1c pa1d pa1e pa1f

pa1:	$(PA1)

pa1a:	pa1a.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o pa1a pa1a.c mycode1.o

pa1b:	pa1b.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o pa1b pa1b.c mycode1.o

pa1c:	pa1c.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o pa1c pa1c.c mycode1.o

pa1d:	pa1d.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o pa1d pa1d.c mycode1.o

pa1e:	pa1e.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o pa1e pa1e.c mycode1.o

pa1f:	pa1f.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o pa1f pa1f.c mycode1.o

test1:	test_1.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o test1 test_1.c mycode1.o

test2:	test_2.c aux.h umix.h mycode1.o
	$(CC) $(FLAGS) -o test2 test_2.c mycode1.o
mycode1.o:	mycode1.c aux.h sys.h mycode1.h
	$(CC) $(FLAGS) -c mycode1.c

clean:
	rm -f *.o $(PA1)
