#@Makefile
TARGET=main

CC=g++
OS=$(shell uname | cut -d_ -f1)

PATH_LIB = /usr/local/lib
PATH_INC = /usr/local/include

LDFLAGS = -L$(PATH_LIB) -lbcm2835 -lwiringPi -lpthread
CFLAGS = -I$(PATH_INC)

PROGRAM=$(addsuffix .cpp, $(TARGET))
OBJS = adcread.o servo.o

#$(TARGET): $(PROGRAM)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(PROGRAM) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(TARGET)
	rm *.o

#adcread: adcread.c
#    g++ -L/usr/local/lib adcread.cpp -lbcm2835
#    g++ -I/usr/local/include -L/usr/local/lib servo.cpp -lwiringPi
