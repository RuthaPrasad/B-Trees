a.out: asgn3_client.o asgn3_implementation.o
	gcc asgn3_client.o asgn3_implementation.o
asgn3_client.o: asgn3_client.c asgn3_header.h
	gcc $(CFLAGS) -c asgn3_client.c asgn3_header.h -std=gnu99
asgn3_implementation.o: asgn3_implementation.c asgn3_header.h
	gcc $(CFLAGS) -c asgn3_implementation.c asgn3_header.h -std=gnu99

