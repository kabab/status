OBJS = main.o rabbitmq.o mongoose.o ttl.o db.o server.o
CC = gcc
DEBUG = -ggdb
CFLAGS = -I/usr/local/include 
LFLAGS = -L/usr/local/lib -lrabbitmq -lpthread

all: producer

%.o: %.c
	$(CC) -c -o $@ $< 

producer: $(OBJS)
	gcc -o producer $^ $(CFLAGS) $(LFLAGS)

clean:
	rm -rf *.o
	rm -rf producer